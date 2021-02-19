#!/bin/bash
# Run hexdump on a binary file and print it as per the defined hex format
# e.g.
# LAU: 3d 71 02 08 31 d5 02 08
# LAU: 31 d5 02 08 31 d5 02 08
# LAU: 31 d5 02 08 31 d5 02 08
# LAU: 5d 6d 02 08 31 d5 02 08
# LAU: 31 d5 02 08

help_msg="Usage: ${0} <input> <output>"

hex_format='"LAU: " 8/1 "%02x " "\n""LAU: " 8/1 "%02x " "\n""LAU: " 8/1 "%02x " "\n""LAU: " 8/1 "%02x " "\n""LAU: " 4/1 "%02x " " \n"'

function help {
  printf "%s\n" "$help_msg"
  exit 0
}

if [ $# == 2 ]
then
  INPUT=$1
  OUTPUT=$2
else
  help
fi

# first include the 8 bytes of metadata
hexdump -n8 -ve '"LAU: " 8/1 "%02x " "\n"' $INPUT > $OUTPUT
hexdump -s8 -ve "$hex_format" $INPUT >> $OUTPUT
