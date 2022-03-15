# Steps to generate markdown table for GitLab Wiki

1. Execute `funs_list_gen.py` to get `funs_list.psv`
2. Execute `test_file_compilers_info_gen.py` to get `test_file_compilers_info.psv`
3. Execute `test_impl_info_gen` to get `test_impl_info.psv`
4. Execute `test_impl_info_psv_to_md` to get `test_impl_info.md`
5. The markdown table in `test_impl_info.md` can be copied and pasted into the GitLab Wiki Page

> Note: 
> - `psv (pipe separated values)` is being used instead of the usual `csv (comma separated values)` because some columns in the data already contain values with `,` inside them. For example, (comma-separated) argument list for a given intrinsic function.
> - the data from `table 16.1` on `pages 332-337` of [F2018 Standard](https://j3-fortran.org/doc/year/18/18-007r1.pdf) was manually copied and pasted (single copy-paste for each page) into `table_16.1_raw_data.txt` file.

# Requirements
Following python libraries are needed to execute the above mentioned scripts
- regex
- numpy
- pandas
- tabulate