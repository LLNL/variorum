#!/bin/bash

RES=$(astyle --errors-to-stdout \
             --preserve-date \
             --style=allman \
             --indent=spaces=4 \
             --convert-tabs \
             --attach-extern-c \
             --indent-col1-comments \
             --min-conditional-indent=0 \
             --max-instatement-indent=40 \
             --pad-oper \
             --pad-header \
             --align-pointer=name \
             --align-reference=name \
             --break-closing-brackets \
             --keep-one-line-blocks \
             --keep-one-line-statements \
             --max-code-length=80 \
             --break-after-logical \
             --dry-run \
             --indent-switches \
             src/variorum/*.[ch] \
             src/variorum/*/*.[ch] \
             src/examples/*.c \
             src/tests/*.cpp \
             src/powmon/*.[ch] | grep Formatted)

if [ -z "$RES" ]; then
    exit 0
else
    exit 1
fi

#
