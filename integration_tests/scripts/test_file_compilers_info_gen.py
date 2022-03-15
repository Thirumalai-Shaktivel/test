"""
This file parsers the 'CMakeLists.txt' file present in 'integration_tests' directory and generates a psv of the list of test files
and their supported compilers

Input: CMakeLists.txt
Output: test_file_compilers_info.psv
"""

import re
import pandas as pd

integration_tests_loc = ".."

def remove_comments_and_newlines(text):
	without_comments = re.sub(r"#.*\n", "", text)
	return without_comments.replace("\n", " ")

command_pattern = r"RUN\(NAME\s+[a-zA-z][\w\s]+LABELS[.\w\s]*\)"
with open(integration_tests_loc + "/CMakeLists.txt", "r") as cmake_list_file: # read CMakeLists.txt file and fetch RUN commands from it 
	processed_text = remove_comments_and_newlines(cmake_list_file.read())
	commands = re.findall(command_pattern, processed_text)

test_file_compilers_info = []
pattern_file_name = r"(?<=RUN\(NAME\s)\w+" # (look behind positive) regex to find the file name
pattern_labels = r"(?<=LABELS\s)[.\w\s]*" # (look behind positive) regex to find labels

# (look behind positive and look ahead positive) regex to find labels when there is 'EXTRAFILES' in the command
pattern_labels_extrafiles = r"(?<=LABELS\s)[.\w\s]*(?=EXTRAFILES)"

for cmd in commands:
	file_name = re.search(pattern_file_name, cmd).group()
	if "EXTRAFILES" in cmd:
		labels = re.search(pattern_labels_extrafiles, cmd).group().strip().split(" ")
	else:
		labels = re.search(pattern_labels, cmd).group().strip().split(" ")
	
	test_file_compilers_info.append([file_name, ",".join(labels)])

test_file_compilers_info.sort(key = lambda info: info[0]) # sort in increasing order according to file name

df = pd.DataFrame(test_file_compilers_info, columns = ["File Name", "Compilers"]) # convert the 2D array to dataframe

# add a new column called "isLLVM" to the dataframe which indicates whether llvm is supported or not
for i in range(df.shape[0]):
    df.loc[i, "isLLVM"] = ("llvm" in (df.loc[i, "Compilers"].split(",")))

df.to_csv("test_file_compilers_info.psv", sep = "|", index = False) # save the dataframe to psv file