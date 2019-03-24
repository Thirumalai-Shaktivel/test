#!/bin/bash

set -e
set -x

mkdir ~/.ssh
chmod 700 ~/.ssh
ssh-keyscan github.com >> ~/.ssh/known_hosts

eval "$(ssh-agent -s)"

set +x
if [[ "${SSH_PRIVATE_KEY_MIRROR}" == "" ]]; then
    echo "Error: SSH_PRIVATE_KEY_MIRROR is empty."
    exit 1
fi
# Generate the private/public key pair using:
#
#     ssh-keygen -f deploy_key -N ""
#
# then set the $SSH_PRIVATE_KEY_DOCS environment variable in the GitLab-CI to
# the base64 encoded private key:
#
#     cat deploy_key | base64 -w0
#
# and add the public key `deploy_key.pub` into the target git repository (with
# write permissions).

ssh-add <(echo "$SSH_PRIVATE_KEY_MIRROR" | base64 -d)
set -x

git push git@github.com:lfortran/lfortran.git +master:master --tags
