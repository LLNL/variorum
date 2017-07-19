CONTRIBUTING
============

This file will describe how to provide feedback to the Libvariorum project.


Code Formatting
---------------

We ask that you use four spaces for indentation, remove all trailing
whitespace, use broken enclosing brackets, and indent the contents of classes,
switches, and namespace.

Code formatting can be automated using astyle with the following parameters:

    $ astyle --style=allman --indent=spaces=4 -y -S -C -N <src_file>


Change Requests
---------------

The easiest way to contribute a change request is to make all of your changes on
a new branch in a fork of libmsr. Make sure your `master` is up-to-date and
create a new branch off of it:

    $ git checkout master 
    $ git pull
    $ git branch <descriptive_branch_name>
    $ git checkout <descriptive_branch_name>

Edit a few files and commit them. It is important to add a descriptive commit
message so that others can easily find (and understand) the changes when
looking back at them in the future.

    $ git add <files_to_be_part_of_the_commit>
    $ git commit -m <descriptive_message_of_this_particular_commit>

Next, push it to your remote fork and create a PR:

    $ git push origin <descriptive_branch_name>

GitHub provides a tutorial on how to file a pull request. When you send the
request, make `master` the destination branch.


Bug Requests
------------

Please open a new issue on GitHub.
