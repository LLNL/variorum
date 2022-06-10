CONTRIBUTING
============

This file will describe how to provide feedback to the Variorum project.

The Variorum project updates the `dev` branch frequently. Use this
branch to get the latest changes and to create new feature branches. Tagged
releases will be pushed to the `main` branch.


Pull Requests
-------------

The easiest way to contribute a change request is to make all of your changes on
a new branch in a fork of Variorum. Make sure your `dev` is up-to-date and
create a new branch off of it. We ask that you name the branches starting with
your <username>-descriptive_branch_name>, so we can track who owns the branch
and the feature.

    $ git checkout dev
    $ git pull
    $ git checkout -b <descriptive_branch_name>

Edit the files and commit them. It is important to add a descriptive commit
message so that others can easily find (and understand) the changes when
looking back at them in the future.

    $ git add <files_to_be_part_of_the_commit>
    $ git commit -m <descriptive_message_of_this_particular_commit>

Next, push it to your remote fork and create a pull request (PR):

    $ git push origin <descriptive_branch_name>

GitHub provides a tutorial on how to file a PR. When you send the request, make
`dev` the destination branch.

A PR template for variorum can be found in `.github/pull_request.md`. 

GitHub Actions will run on all PRs, and will (1) build and compile the source
code, and (2) enforce style format.


Code Formatting
---------------

Code formatting is checked as part of GitHub Actions. We use the Allman style
in [astyle](http://astyle.sourceforge.net/). You can run
`./scripts/check-code-format.sh` to automatically format your code to our
required format.

A good vim recipe for code formatting is:

    vi:tabstop=4 shiftwidth=4 expandtab
