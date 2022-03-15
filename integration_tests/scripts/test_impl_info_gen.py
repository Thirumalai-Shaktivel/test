"""
This file generates a psv which lists all the intrinsic functions in fortran along with the basic tests 
and implementation status of each intrinsic function. For each intrinsic function, it also lists all the test files which
test the intrinsic function and for each test file, it also mentions if the test file is supported in llvm or not

Input: funs_list.psv, test_file_compilers_info.psv, all the test files inside the 'integration_tests' directory
Output: test_impl_info.psv
"""

import re
import os
import pandas as pd

integration_tests_loc = ".."

test_files = []
not_in_cache_makelist = []
test_file_compilers_info = pd.read_csv("test_file_compilers_info.psv", sep = "|").set_index("File Name")
funs_list = pd.read_csv("funs_list.psv", sep = "|")
funs_list.loc[144, "Intrinsic Procedure"] = "null" # pandas is converting the function name null to nan, so the null is restored here

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
            pattern_func_name = f"[^\w]{func_name}\("
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


# for each function in the funs_list, find which all files it is tested in along with whether the files support llvm or no
for i in range(funs_list.shape[0]):
    tested_in_files = get_tested_in_files(funs_list.loc[i, "Intrinsic Procedure"])
    llvm_supp_info, is_basic_test, is_basic_impl = get_llvm_supp_info_info(tested_in_files)
    funs_list.loc[i, "Tested in, llvm status"] = "\n".join(llvm_supp_info)
    funs_list.loc[i, "Basic Tests"] = ["", ":white_check_mark:"][int(is_basic_test)]
    funs_list.loc[i, "Basic Impl"] = ["", ":white_check_mark:"][int(is_basic_impl)]

funs_list.to_csv("test_impl_info.psv", sep = "|", index = False)

print("files not in cache make list: ", sorted(not_in_cache_makelist))