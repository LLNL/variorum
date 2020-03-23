#!/bin/bash

# re-fromat all *.cpp, *.h, and *.c files in src/ directory
#   exclude files in thirdparty_builtin/ (gtest source files)
#   exclude files in fortran/ (EP source files)
FILES=$(find src -type f  \( -name *.[ch] -o -name *.cpp \) \
        | grep -v thirdparty_builtin \
        | grep -v fortran)

TMP=$(astyle --errors-to-stdout \
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
             --indent-switches \
             ${FILES})

RES=$(echo "$TMP" | grep Formatted)

echo -e "$RES"

if [ -z "$RES" ]; then
    exit 0
else
    exit 1
fi

#
