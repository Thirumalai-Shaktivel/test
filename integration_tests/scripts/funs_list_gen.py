"""
This file parsers the raw table data present in 'table_16.1_raw_data.txt' and generates a psv of the list of intrinsic functions in fortran

Input: table_16.1_raw_data.txt
Output: funs_list.psv
"""

import re

with open("table_16.1_raw_data.txt", "r") as raw_funs_list_file:
    raw_funs_list = raw_funs_list_file.read()

# we first split across the character ".\n" to get list of functions
# the individual elements of the new list are processed later
funs_list = raw_funs_list.split(".\n")

pattern_class = R"\s(A|C|E|ES|I|PS|S|T)\s" # regex pattern to find class (example of class: " A ")

sep = "|" # seperator to be used while saving processed information

with open("funs_list.psv", "w") as funs_list_file:
    funs_list_file.write(f"Intrinsic Procedure{sep}Arguments{sep}Class{sep}Description\n")
    for func_info in funs_list:

        # everything before the first '(' is part of the name of the func and the rest is args+class+desc that is details
        name, details = func_info.replace("\n", " ").split(" (", maxsplit = 1) 
        name = name.replace("-", "").replace(" ", "") # some fun names are split across lines in the pdf table and have "-" or " " which is removed here
        args, class_, desc = re.split(pattern_class, details, maxsplit = 1)
        
        funs_list_file.write(f"{name.lower()}{sep}({args.lower()}{sep}{class_.strip()}{sep}{desc.capitalize()}\n")