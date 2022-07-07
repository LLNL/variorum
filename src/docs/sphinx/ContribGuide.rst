..
   # Copyright 2019-2022 Lawrence Livermore National Security, LLC and other
   # Variorum Project Developers. See the top-level LICENSE file for details.
   #
   # SPDX-License-Identifier: MIT

####################
 Contributing Guide
####################

This page is for those who would like to contribute a new feature or bugfix to
Variorum. The guide will show a few examples of contributing workflows and
discuss the granularity of pull requests (PRs). It will also discuss the tests
your PR must pass in order to be accepted into Variorum.

The changes proposed in a PR should correspond to one feature, bugfix, etc. PRs
can contain changes relevant to different ideas, however reviewing these PRs
becomes tedious and error prone. If possible, try to follow the
**one-PR-per-bugfix/feature** rule.

**********
 Branches
**********

Variorum is in active development. The ``dev`` branch has the latest
contributions to Variorum. All pull requests should start from the ``dev``
branch and target ``dev``.

There is a branch for each release, each originating from ``dev``, and follows
the naming convention ``releases/v<major>.<minor>.x``.

************************
 Continuous Integration
************************

Variorum uses `Github Actions <https://docs.github.com/en/actions>`_ for
Continuous Integration testing. For each pull request, a series of tests will be
run to make sure the code change does not accidentally introduce any bugs into
Variorum. Your PR must pass all of these tests before being accepted. While you
can certainly wait for the results of these tests after submitting a PR, we
recommend that you run them locally to speed up the review process.

We currently test against several gcc versions and different build options on
Linux and perform 2 types of tests:

Unit Tests
==========

Unit tests ensure that core Variorum features like building for supported
platforms are working as expected.

The status of the unit tests can be checked on the `Github Actions tab
<https://github.com/LLNL/variorum/actions>`_.

These tests may take several minutes to complete depending on how quickly the
runners are launched.

Style Tests
===========

Variorum uses two formatters for style checking. `Flake8
<http://flake8.pycqa.org/en/latest/>`_ to test for `PEP 8
<https://www.python.org/dev/peps/pep-0008/>`_ conformance. PEP 8 is a series of
style guides for Python that provide suggestions for everything from variable
naming to indentation. Flake8 is a code linter; it provides warnings of syntax
errors, possible bugs, stylistic errors, etc.

To check for compliance, you will want to run the following two commands at the
root of Variorum:

.. code:: console

   $ flake8
   $ black --check --diff --exclude "/(src/thirdparty_builtin/googletest|build|src/docs)/" .

If your code is compliant, ``flake8`` will output nothing:

.. code:: console

   $ flake8

and ``black`` will show:

.. code:: console

   $ black --check --diff --exclude "/(src/thirdparty_builtin/googletest|build|src/docs)/" .

   All done! ✨ 🍰 ✨
   4 files would be left unchanged.

However, if your code is not compliant with PEP 8, flake8 and black will
complain:

.. code:: console

   $ flake8
   ./src/utilities/verify_msr_kernel.py:35:1: E302 expected 2 blank lines, found 1
   ./src/utilities/verify_opal.py:12:1: F401 'numpy' imported but unused

.. code:: console

   $ black --check --diff --exclude "/(src/thirdparty_builtin/googletest|build|src/docs)/" .
   --- src/utilities/verify_opal.py     2022-07-07 05:09:42.145667 +0000
   +++ src/utilities/verify_opal.py     2022-07-07 05:09:46.232596 +0000
   @@ -10,10 +10,11 @@
    import os
    import sys
    import pandas

    import getopt
   +

    def check_opal_files_presence(verbose):
        if verbose:
            print("-- Check if OPAL files exist")

   would reformat src/utilities/verify_opal.py
   Oh no! 💥 💔 💥
   1 file would be reformatted, 3 files would be left unchanged.

As you address these errors with the addition or removal of lines, the line
numbers will change, so you will want to re-run flake8 and black again to update
them.

Alternatively, fixing the errors in reverse order will eliminate the need for
multiple runs of flake8 and black just to re-compute line numbers.

Additionally, Variorum uses `Artistic Style <http://astyle.sourceforge.net/>`_
for formatting C/C++ files.

.. note::

   We have a helper script in Variorum for calling ``astyle`` locally and
   checking for style compliance of your C/C++ files. To call this script to
   format C/C++ files, use ``scripts/check-code-format.sh``.

***********************
 Contributing Workflow
***********************

(Thanks to Spack for providing a great overview of the different contributing
workflows described in this section.)

Variorum is under active development, so new features and bugfixes are
constantly being merged into the ``dev`` branch. The recommended way to
contribute a pull request is to fork the Variorum repository in your own space
(if you already have a fork, make sure is it up-to-date), and then create a new
branch off of ``dev``.

.. code:: console

   $ git checkout dev
   $ git fetch upstream && git merge --ff-only upstream/dev
   $ git branch <descriptive_branch_name>
   $ git checkout <descriptive_branch_name>

Here, we assume that the upstream remote points at
``https://github.com/llnl/variorum.git``.

We prefer that commits pertaining to different pieces of Variorum (new hardware
port, specific hardware feature, docs, etc.) prefix the component name in the
commit message (for example ``<component>: descriptive message``.

Now, you can make your changes while keeping the ``dev`` branch unmodified. Edit
a few files and commit them by running:

.. code:: console

   $ git add <files_to_be_part_of_the_commit>
   $ git commit --message <descriptive_message_of_this_particular_commit>

Next, push it to your remote fork (that is, ``origin`` points at
``https://github.com/<your_user_name>/variorum.git``) and create a PR:

.. code:: console

   $ git push origin <descriptive_branch_name>

GitHub provides a `tutorial
<https://help.github.com/articles/about-pull-requests/>`_ on how to file a pull
request. When you send the request, make ``dev`` the destination branch.

If you have multiple PRs that build on top of one another, one option is to keep
a branch that includes all of your other feature branches:

.. code:: console

   $ git checkout dev
   $ git branch <your_branch_with_all_features>
   $ git checkout <your_branch_with_all_features>
   $ git rebase <descriptive_branch_name>

This can be done with each new PR you submit. Just make sure to keep this local
branch up-to-date with upstream ``dev`` too.

Rebasing
========

Other developers are making contributions to Variorum, possibly to the same
files that your PR has modified. If their PR is merged before yours, it can
create a merge conflict. This means that your PR can no longer be automatically
merged without a chance of breaking your changes. In this case, you will be
asked to rebase your branch on top of the latest upstream ``dev``.

First, make sure your ``dev`` branch is up-to-date:

.. code:: console

   $ git checkout dev
   $ git fetch upstream
   $ git merge --ff-only upstream/dev

Now, we need to switch to the branch you submitted for your PR and rebase it on
top of ``dev``:

.. code:: console

   $ git checkout <descriptive_branch_name>
   $ git rebase dev

Git will likely ask you to resolve conflicts. Edit the file that it says can't
be merged automatically and resolve the conflict. Then, run:

.. code:: console

   $ git add <file_with_a_conflict>
   $ git rebase --continue

You may have to repeat this process multiple times until all conflicts are
resolved. Once this is done, simply force push your rebased branch to your
remote fork:

.. code:: console

   $ git push --force origin <descriptive_branch_name>
