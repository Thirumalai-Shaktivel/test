## Contributing

We welcome contributions from anyone, even if you are new to open source. It
might sound daunting to contribute to a compiler at first, but please do, it is
not complicated. We will help you with any technical issues and help improve
your contribution so that it can be merged.

To contribute, submit a Merge Request (MR) against our repository at:

https://gitlab.com/lfortran/lfortran

If you have any questions or need help, please ask as at our
[mailinglist](https://groups.io/g/lfortran) or a
[chat](https://lfortran.zulipchat.com/).

Please note that all participants of this project are expected to follow our
Code of Conduct. By participating in this project you agree to abide by its
terms. See [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).

By submitting an MR you agree to license your contribution under
the LFortran's BSD [license](LICENSE) unless explicitly noted otherwise.

## How to create a new Merge Request

### Setup Git

Setup git, see any tutorial online such as:

* https://github.com/sympy/sympy/wiki/Development-workflow#set-up-git

Make sure you setup:

* Your username + email
* Your `~/.gitconfig`
* Your shell prompt to show the current branch name

### Fork LFortran

1. Create Fork online
2. Setup your ssh key with gitlab.com
3. Clone LFortran and setup your remotes

```
git clone https://gitlab.com/lfortran/lfortran.git
cd lfortran
git remote add some_name git@gitlab.com:GITLAB_ID/lfortran.git
```
Where `some_name` is any name you like, for example your first name. The
`GITLAB_ID` is your gitlab.com ID. You can use `git remote -v` to check that
the new remote is there.

### Send a new Merge Request

1. Create a new branch
```
git checkout -b fix1
```
2. Create some changes in files
3. Commit them:
```
git add FILE1 FILE2
git commit
```
Write a good commit message
4. Check that your changes look good:
```
git gl
```
5. Send a Merge Request
```
git push some_name fix1
```

This will push the new branch `fix1` into your remote `some_name`. And it will
also print a link that you can click to open a new Merge Request. Write a title
and description and click the "Create" button.
