"""
This file generates a markdown table which lists all the intrinsic functions in fortran along with the basic tests 
and implementation status of each intrinsic function. For each intrinsic function, it also lists all the test files which
test the intrinsic function and for each test file, it also mentions if the test file is supported in llvm or not

Input: funs_list.psv, test_file_compilers_info.psv, integration_tests/*.f90
Output: funs_test_impl_info.md
"""

import re
import os
import pandas as pd

integration_tests_loc = ".."

test_files = []
not_in_cache_makelist = []
test_file_compilers_info = pd.read_csv("test_file_compilers_info.psv", sep = "|").set_index("File Name")
funs_test_impl_info = pd.read_csv("funs_list.psv", sep = "|")

# while reading the funs_list.psv, pandas is converting the function name `null` to `nan`, so the `null` is restored back here
funs_test_impl_info.loc[144, "Intrinsic Procedure"] = "null"

def is_fortran_file(file_name):
    return len(file_name) > 4 and file_name[-4:] == ".f90"

for file_name in sorted(os.listdir(integration_tests_loc)):
    if is_fortran_file(file_name):
        test_files.append(file_name[:-4]) # append the file name without the file extension

def get_tested_in_files(func_name):
    tested_in_files = [] # list of files where the given function has been tested
    for test_file_name in test_files:
        with open(f"{integration_tests_loc}/{test_file_name}.f90", "r") as test_file:

            # regex to search for function call, a non-word character followed by the function name followed by '('
            pattern_func_name = f"\W{func_name}\("
            if re.findall(pattern_func_name, test_file.read()):
                tested_in_files.append(test_file_name)
    return tested_in_files

def get_llvm_supp_info_info(tested_in_files):
    test_in_files_llvm_supp_info = [] # for each file in tested_in_files, it adds a tag indicating whether the test_file is supported by llvm or not
    is_basic_test = False
    is_basic_impl = False
    for file_name in tested_in_files:
        try:
            if file_name in not_in_cache_makelist:
                test_in_files_llvm_supp_info.append(file_name + " :grey_question:")
            elif test_file_compilers_info.loc[file_name, "isLLVM"]:
                test_in_files_llvm_supp_info.append(file_name + " :white_check_mark:")
                is_basic_test = True
                is_basic_impl = True
            else:
                test_in_files_llvm_supp_info.append(file_name + " :x:")
                is_basic_test = True
        except:
            not_in_cache_makelist.append(file_name)
            test_in_files_llvm_supp_info.append(file_name + " :grey_question:")
    return test_in_files_llvm_supp_info, is_basic_test, is_basic_impl


# for each function in the funs_test_impl_info, find which all files it is tested in along with whether the files support llvm or not
for i in range(funs_test_impl_info.shape[0]):
    tested_in_files = get_tested_in_files(funs_test_impl_info.loc[i, "Intrinsic Procedure"])
    llvm_supp_info, is_basic_test, is_basic_impl = get_llvm_supp_info_info(tested_in_files)
    funs_test_impl_info.loc[i, "Tested in, llvm status"] = "\n".join(llvm_supp_info)
    funs_test_impl_info.loc[i, "Basic Tests"] = ["", ":white_check_mark:"][int(is_basic_test)]
    funs_test_impl_info.loc[i, "Basic Impl"] = ["", ":white_check_mark:"][int(is_basic_impl)]

funs_test_impl_info.index = range(1, funs_test_impl_info.shape[0] + 1) # indexing is changed to start from 1 instead of 0
with open("funs_test_impl_info.md", "w") as funs_test_impl_info_md_file:
    funs_test_impl_info_md_file.write(funs_test_impl_info.to_markdown()) # saving to markdown file

print("files not in cache make list: ", sorted(not_in_cache_makelist)) # these files test some intrinsic function but they are not registered in CMakeLists.txt