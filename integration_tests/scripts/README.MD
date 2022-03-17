# Steps to generate markdown table for GitLab Wiki

1. Execute `test_file_compilers_info_gen.py` to get `test_file_compilers_info.psv`
2. Execute `funs_test_impl_info_md_gen.py` to get `funs_test_impl_info.md`
3. The markdown table in `funs_test_impl_info.md` can be copied and pasted into the GitLab Wiki Page

> Note: 
> - `psv (pipe separated values)` is being used instead of the usual `csv (comma separated values)` because some columns in the data already contain values with `,` inside them. For example, (comma-separated) argument list for a given intrinsic function.

# Requirements
Following python libraries are needed to execute the above mentioned scripts
- regex
- numpy
- pandas
- tabulate