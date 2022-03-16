"""
This file converts the 'test_impl_info.psv' to markdown format which can be added to the GitLab Wiki Page

Input: test_impl_info.psv
Output: test_impl_info.md
"""

import numpy as np
import pandas as pd

test_impl_info = pd.read_csv("test_impl_info.psv", sep = "|")

# while reading the psv file, pandas is converting the function name null to nan, so the null is restored here
test_impl_info.loc[144, "Intrinsic Procedure"] = "null"

# while reading the psv file, pandas is converting empty strings to 'nan' and so the change is reverted here
test_impl_info = test_impl_info.replace(np.nan, '', regex=True)

test_impl_info.index = range(1, test_impl_info.shape[0] + 1) # indexing is changed to start from 1 instead of 0
with open("test_impl_info.md", "w") as test_impl_info_md_file:
    test_impl_info_md_file.write(test_impl_info.to_markdown()) # saving to markdown file