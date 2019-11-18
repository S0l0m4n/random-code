#!/bin/bash

# Clean up timestamped logs produced by the ExtraPuTTY application
#
# Example:
# If I get these log lines as input:
# ~~~
# [19-11-13 10:49:10.191] MSG: Message: 
# [19-11-13 10:49:10.207] 
# 
# [19-11-13 10:49:10.691] MSG: Message: 
# [19-11-13 10:49:10.707] 
# 
# [19-11-13 10:49:11.191] MSG: Message: 
# [19-11-13 10:49:11.191] 
# 
# [19-11-13 10:49:11.488] MDM: AT+CEREG?
# [19-11-13 10:49:11.504] 
# [19-11-13 10:49:11.504] 
# 
# [19-11-13 10:49:11.551] MDM: > [OK]
# [19-11-13 10:49:11.551] 
# 
# [19-11-13 10:49:11.551] MDM: > +CEREG: 0,2
# [19-11-13 10:49:11.551] 
# 
# [19-11-13 10:49:11.551] MDM: | <urc_nval> = 0
# [19-11-13 10:49:11.551] 
# 
# [19-11-13 10:49:11.551] MDM: | <stat> = 2
# [19-11-13 10:49:11.566] 
# 
# [19-11-13 10:49:11.692] MSG: Message: 
# [19-11-13 10:49:11.692] 
# 
# [19-11-13 10:49:12.191] MSG: Message: 
# [19-11-13 10:49:12.191] 
# ~~~
# Then I want this to be the output:
# ~~~
# [19-11-13 10:49:10.191] MSG: Message: 
# [19-11-13 10:49:10.691] MSG: Message: 
# [19-11-13 10:49:11.191] MSG: Message: 
# [19-11-13 10:49:11.488] MDM: AT+CEREG?
# [19-11-13 10:49:11.551] MDM: > [OK]
# [19-11-13 10:49:11.551] MDM: > +CEREG: 0,2
# [19-11-13 10:49:11.551] MDM: | <urc_nval> = 0
# [19-11-13 10:49:11.551] MDM: | <stat> = 2
# [19-11-13 10:49:11.692] MSG: Message: 
# [19-11-13 10:49:12.191] MSG: Message: 
# ~~~
#
# Usage: Accept the input file as argument and write in place.

INPUT=$1

if [ $# -ne 1 ]
then
    echo "Usage: $0 <log-file>"
    exit 1
elif [ ! -e "$INPUT" ]
then
    echo "Log file $INPUT does not exist"
    exit 1
fi

TMP_OUTPUT=${INPUT}.tmp

if [ -e $TMP_OUTPUT ]
then
    echo "The file $TMP_OUTPUT would be overwritten! Quitting..."
    exit 1
fi

cp $INPUT $TMP_OUTPUT

# Remove empty lines
grep -vE '^$' $INPUT > $TMP_OUTPUT

# Remove empty timestamped lines and write in place
grep -v '\[..-..-.. ..:..:......\] $' $TMP_OUTPUT > $INPUT

# Remove temporary output file
rm $TMP_OUTPUT

# Convert to UNIX format
dos2unix -q $INPUT
