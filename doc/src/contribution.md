# Contributing

## Documentation



First, make sure that you have forked and cloned the [LFortran main repository](https://gitlab.com/lfortran/lfortran). Git automatically creates a folder with the repository name `lfortran` and downloads all files there. 


Then, create a new environment for building the documentation (you can choose any name, here we chose `docs`) and activate it. 

```bash
conda create -n docs mkdocs
conda activate docs
```


:material-lightbulb-outline: <em>Note that the `mkdocs` package requires a recent version of Python and the the Python package manager, `pip`, to be installed on your system.</em>








Next, deploy the documentation by:
```bash
cd doc
mkdocs serve
```

If nothing goes wrong, the terminal would prompt `Serving on http://127.0.0.1:8000/`, which means that the built documentation becomes available locally at `http://127.0.0.1:8000/`.

## 
