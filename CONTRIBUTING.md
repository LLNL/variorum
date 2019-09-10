CONTRIBUTING
============

This file will describe how to provide feedback to the Variorum project.

The Variorum project updates the `dev` branch frequently. Use this
branch to get the latest changes and to create new feature branches. Tagged
releases will be pushed to the `master` branch.


Change Requests
---------------

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
looking back at them in the future. Please include a Signed-off-by: <name>
<email> in the commit message.

    $ git add <files_to_be_part_of_the_commit>
    $ git commit -m <descriptive_message_of_this_particular_commit>

Next, push it to your remote fork and create a PR:

    $ git push origin <descriptive_branch_name>

GitHub provides a tutorial on how to file a pull request. When you send the
request, make `dev` the destination branch.


Code Formatting
---------------

We ask that you use four spaces for indentation, remove all trailing
whitespace, use broken enclosing brackets, and indent the contents of classes,
switches, and namespace.

Code formatting can be automated using astyle with the following parameters:

    $ astyle --style=allman --indent=spaces=4 -y -S -C -N <src_file>
