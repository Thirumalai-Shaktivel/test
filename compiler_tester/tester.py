import hashlib
import json
import logging
import os
import pathlib
import shutil
import subprocess
import sys
from typing import Any, Mapping, List, Union

level = logging.DEBUG
log = logging.getLogger(__name__)
handler = logging.StreamHandler(sys.stdout)
handler.setFormatter(logging.Formatter('%(message)s'))
handler.setLevel(level)
log.addHandler(handler)
log.setLevel(level)


class RunException(Exception):
    pass


class ExecuteException(Exception):
    pass


class style:
    reset = 0
    bold = 1
    dim = 2
    italic = 3
    underline = 4
    blink = 5
    rblink = 6
    reversed = 7
    conceal = 8
    crossed = 9


class fg:
    black = 30
    red = 31
    green = 32
    yellow = 33
    blue = 34
    magenta = 35
    cyan = 36
    gray = 37
    reset = 39


def color(value):
    return "\033[" + str(int(value)) + "m"


def check():
    return f"{(color(fg.green)+color(style.bold))}✓ {color(fg.reset)+color(style.reset)}"


def bname(base, cmd, filename):
    hstring = cmd
    if filename:
        hstring += filename
    h = hashlib.sha224(hstring.encode()).hexdigest()[:7]
    if filename:
        bname = os.path.basename(filename)
        bname, _ = os.path.splitext(bname)
        return f"{base}-{bname}-{h}"
    else:
        return f"{base}-{h}"


def _compare_eq_dict(
    left: Mapping[Any, Any], right: Mapping[Any, Any], verbose: int = 0
) -> List[str]:
    explanation = []  # type: List[str]
    set_left = set(left)
    set_right = set(right)
    common = set_left.intersection(set_right)
    same = {k: left[k] for k in common if left[k] == right[k]}
    if same and verbose < 2:
        explanation += ["Omitting %s identical items" % len(same)]
    elif same:
        explanation += ["Common items:"]
        explanation += pprint.pformat(same).splitlines()
    diff = {k for k in common if left[k] != right[k]}
    if diff:
        explanation += ["Differing items:"]
        for k in diff:
            explanation += [repr({k: left[k]}) + " != " + repr({k: right[k]})]
    extra_left = set_left - set_right
    len_extra_left = len(extra_left)
    if len_extra_left:
        explanation.append(
            "Left contains %d more item%s:"
            % (len_extra_left, "" if len_extra_left == 1 else "s")
        )
        explanation.extend(
            pprint.pformat({k: left[k] for k in extra_left}).splitlines()
        )
    extra_right = set_right - set_left
    len_extra_right = len(extra_right)
    if len_extra_right:
        explanation.append(
            "Right contains %d more item%s:"
            % (len_extra_right, "" if len_extra_right == 1 else "s")
        )
        explanation.extend(
            pprint.pformat({k: right[k] for k in extra_right}).splitlines()
        )
    return explanation


def fixdir(s: str) -> str:
    local_dir = os.getcwd()
    return s.replace(local_dir.encode(), "$DIR".encode())


def run(basename: str, cmd: Union[pathlib.Path, str],
        out_dir: Union[pathlib.Path, str], infile=None, extra_args=None):
    """
    Runs the `cmd` and collects stdout, stderr, exit code.

    The stdout, stderr and outfile are saved in the `out_dir` directory and
    all metadata is saved in a json file, whose path is returned from the
    function.

    The idea is to use this function to test the compiler by running it with
    an option to save the AST, ASR or LLVM IR or binary, and then ensure that
    the output does not change.

    Arguments:

    basename ... name of the run
    cmd ........ command to run, can use {infile} and {outfile}
    out_dir .... output directory to store output
    infile ..... optional input file. If present, it will check that it exists
                 and hash it.
    extra_args . extra arguments, not part of the hash

    Examples:

    >>> run("cat2", "cat tests/cat.txt > {outfile}", "output", "tests/cat.txt")
    >>> run("ls4", "ls --wrong-option", "output")

    """
    assert basename is not None and basename != ""
    pathlib.Path(out_dir).mkdir(parents=True, exist_ok=True)
    if infile and not os.path.exists(infile):
        raise RunException("The input file does not exist")
    outfile = os.path.join(out_dir, basename + "." + "out")
    cmd2 = cmd.format(infile=infile, outfile=outfile)
    if extra_args:
        cmd2 += " " + extra_args
    r = subprocess.run(cmd2, shell=True,
                       stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE)
    if not os.path.exists(outfile):
        outfile = None
    if len(r.stdout):
        stdout_file = os.path.join(out_dir, basename + "." + "stdout")
        open(stdout_file, "wb").write(fixdir(r.stdout))
    else:
        stdout_file = None
    if len(r.stderr):
        stderr_file = os.path.join(out_dir, basename + "." + "stderr")
        open(stderr_file, "wb").write(fixdir(r.stderr))
    else:
        stderr_file = None

    if infile:
        infile_hash = hashlib.sha224(open(infile, "rb").read()).hexdigest()
    else:
        infile_hash = None
    if outfile:
        outfile_hash = hashlib.sha224(open(outfile, "rb").read()).hexdigest()
        outfile = os.path.basename(outfile)
    else:
        outfile_hash = None
    if stdout_file:
        stdout_hash = hashlib.sha224(
            open(stdout_file, "rb").read()).hexdigest()
        stdout_file = os.path.basename(stdout_file)
    else:
        stdout_hash = None
    if stderr_file:
        stderr_hash = hashlib.sha224(
            open(stderr_file, "rb").read()).hexdigest()
        stderr_file = os.path.basename(stderr_file)
    else:
        stderr_hash = None
    data = {
        "basename": basename,
        "cmd": cmd,
        "infile": infile,
        "infile_hash": infile_hash,
        "outfile": outfile,
        "outfile_hash": outfile_hash,
        "stdout": stdout_file,
        "stdout_hash": stdout_hash,
        "stderr": stderr_file,
        "stderr_hash": stderr_hash,
        "returncode": r.returncode,
    }
    json_file = os.path.join(out_dir, basename + "." + "json")
    json.dump(data, open(json_file, "w"), indent=4)
    return json_file


def get_error_diff(reference_file, output_file, full_err_str) -> str:
    diff_list = subprocess.Popen(
        f"diff {reference_file} {output_file}",
        stdout=subprocess.PIPE,
        shell=True,
        encoding='utf-8')
    diff_str = ""
    diffs = diff_list.stdout.readlines()
    for d in diffs:
        diff_str += d
    full_err_str += f"\nDiff against: {reference_file}\n"
    full_err_str += diff_str
    return full_err_str


def do_update_reference(jo, jr, do):
    shutil.copyfile(jo, jr)
    for f in ["outfile", "stdout", "stderr"]:
        if do[f]:
            f_o = os.path.join(os.path.dirname(jo), do[f])
            f_r = os.path.join(os.path.dirname(jr), do[f])
            shutil.copyfile(f_o, f_r)


def run_test(testname, basename, cmd, infile=None, update_reference=False,
             extra_args=None):
    """
    Runs the test `cmd` and compare against reference results.

    The `cmd` is executed via `run` (passing in `basename` and `infile`) and
    the output is saved in the `output` directory. The generated json file is
    then compared against reference results and if it differs, the
    RunException is thrown.

    Arguments:

    basename ........... name of the run
    cmd ................ command to run, can use {infile} and {outfile}
    infile ............. optional input file. If present, it will check that
                         it exists and hash it.
    update_reference ... if True, it will copy the output into the reference
                         directory as reference results, overwriting old ones
    extra_args ......... Extra arguments to append to the command that are not
                         part of the hash

    Examples:

    >>> run_test("cat12", "cat {infile} > {outfile}", "cat.txt",
    ...     update_reference=True)
    >>> run_test("cat12", "cat {infile} > {outfile}", "cat.txt")
    """
    s = f"{testname} * {basename}"
    basename = bname(basename, cmd, infile)
    if infile:
        infile = os.path.join("tests", infile)
    jo = run(basename, cmd, os.path.join("tests", "output"), infile=infile,
             extra_args=extra_args)
    jr = os.path.join("tests", "reference", os.path.basename(jo))
    if not os.path.exists(jo):
        raise FileNotFoundError(
            f"The output json file '{jo}' for {testname} does not exist")

    do = json.load(open(jo))
    if update_reference:
        do_update_reference(jo, jr, do)
        return

    if not os.path.exists(jr):
        raise FileNotFoundError(
            f"The reference json file '{jr}' for {testname} does not exist")

    dr = json.load(open(jr))
    if do != dr:
        # This string builds up the error message. Print test name in red in the beginning.
        # More information is added afterwards.
        full_err_str = f"\n{(color(fg.red)+color(style.bold))}{s}{color(fg.reset)+color(style.reset)}\n"
        e = _compare_eq_dict(do, dr)
        full_err_str += "The JSON metadata differs against reference results\n"
        full_err_str += "Reference JSON: " + jr + "\n"
        full_err_str += "Output JSON:    " + jo + "\n"
        full_err_str += "\n".join(e)

        for field in ["outfile", "stdout", "stderr"]:
            hash_field = field + "_hash"
            if not do[hash_field] and dr[hash_field]:
                full_err_str += f"No output {hash_field} available for {testname}\n"
                break
            if not dr[hash_field] and do[hash_field]:
                full_err_str += f"No reference {hash_field} available for {testname}\n"
                break
            if do[hash_field] != dr[hash_field]:
                output_file = os.path.join("tests", "output", do[field])
                reference_file = os.path.join("tests", "reference", dr[field])
                full_err_str = get_error_diff(
                    reference_file, output_file, full_err_str)
                break
        raise RunException(
            "Testing with reference output failed." +
            full_err_str)
    log.debug(s + " " + check())
