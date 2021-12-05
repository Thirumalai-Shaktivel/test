#!/usr/bin/env bash

set -e
set -x

deploy_repo_pull="https://github.com/lfortran/tarballs"
deploy_repo_push="git@github.com:lfortran/tarballs.git"

if [[ $CI_COMMIT_TAG == "" ]]; then
    # Development version
    dest_dir="dev"
else
    # Release version
    dest_dir="release"
fi

lfortran_version=$(<version)

mkdir ~/.ssh
chmod 700 ~/.ssh
ssh-keyscan gitlab.com >> ~/.ssh/known_hosts
ssh-keyscan github.com >> ~/.ssh/known_hosts

eval "$(ssh-agent -s)"


D=`pwd`

mkdir $HOME/repos
cd $HOME/repos

git clone ${deploy_repo_pull} tarballs
cd tarballs/docs
mkdir -p ${dest_dir}
cp $D/dist/lfortran-${lfortran_version}.tar.gz ${dest_dir}/

python $D/ci/tarball_update_json.py ${dest_dir} ${lfortran_version} ${CI_COMMIT_SHA}

git config user.name "Deploy"
git config user.email "noreply@deploy"
COMMIT_MESSAGE="Deploying on $(date "+%Y-%m-%d %H:%M:%S")"

git add .
git commit -m "${COMMIT_MESSAGE}"

git show HEAD -p --stat
dest_commit=$(git show HEAD -s --format=%H)

if [[ ${CI_COMMIT_REF_NAME} == "master" ]]; then
    echo "The pipeline was triggered from the master branch"
else
    if [[ $CI_COMMIT_TAG != "" && ${CI_COMMIT_TAG:0:1} == "v" ]]; then
        echo "The pipeline was triggered from a tag 'v*'"
    else
        # We are either on a non-master branch, or tagged with a tag that does
        # not start with v*. We skip the upload.
        echo "Not a master branch, not tagged with v*, skipping..."
        exit 0
    fi
fi


set +x
if [[ "${SSH_PRIVATE_KEY_TARBALL}" == "" ]]; then
    echo "Note: SSH_PRIVATE_KEY_TARBALL is empty, skipping..."
    exit 0
fi
# Generate the private/public key pair using:
#
#     ssh-keygen -f deploy_key -N ""
#
# then set the $SSH_PRIVATE_KEY_TARBALL environment variable in the GitLab-CI
# to the base64 encoded private key (uncheck the "Masked" check box):
#
#     cat deploy_key | base64 -w0
#
# and add the public key `deploy_key.pub` into the target git repository (with
# write permissions).

ssh-add <(echo "$SSH_PRIVATE_KEY_TARBALL" | base64 -d)
set -x


git push ${deploy_repo_push} master:master
echo "New commit pushed at:"
echo "https://github.com/lfortran/tarballs/commit/${dest_commit}"

echo "The tarball is available from:"
echo "https://lfortran.github.io/tarballs/${dest_dir}/lfortran-${lfortran_version}.tar.gz"
