#!/bin/bash
# Comment out problematic lines in the autogenerated Makefile-default.mk when
# building switchee_app on Linux using MPLABX.
# Details: MPLABX v4.10, C compiler v1.33
set -euo pipefail

PROBLEM_OBJ_FILES='lcd.o.d ui.o.d'

# Pass path to Makefile-default.mk on the command line
if [ $# -ne 1 ]
then
    echo "Usage: $0 <Makefile-default.mk>"
    exit 1
fi
MAKEFILE=$1

if [ ! -e $MAKEFILE ]
then
    echo "ERROR! Could not find $MAKEFILE"
    exit 1
fi

for file in $PROBLEM_OBJ_FILES
do
    echo "Commenting out FIXDEPS call on ${file}..."
    lines=`grep -n ".*FIXDEPS.*${file}" $MAKEFILE | cut -f1 -d ':'`
    for line in $lines
    do
        echo -e "  > Found on line $line"
        sed -i "${line}s/^/#&/" $MAKEFILE
    done
done
