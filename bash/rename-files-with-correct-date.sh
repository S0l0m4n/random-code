#!/bin/bash
# For each file or directory in the current location, and all children
# locations, rename each occurrence of a date within the name in the form of
# 21xxxx to 22xxxx

set -euo pipefail

# substitute 21 with 22 at the start of the directory name
function sub21
{
  echo $1 | awk 'sub("21", "22")'
}

# rename directories at this level
function rename_dirs
{
  dirs=`find . -maxdepth 1 -type d`

  for dir in ${dirs}
  do
    newdir=`sub21 $dir`

    if [ "$newdir" ]
    then
      # rename dir to the new name
      mv $dir $newdir

      # print transformation
      echo "${dir} -> ${newdir}"

      # rename files in this directory
      cd $newdir
      rename_files
      cd ..
    fi
  done
}

# rename files at this level
function rename_files
{
  files=`find . -maxdepth 1 -type f`

  for file in ${files}
  do
    newfile=`sub21 $file`

    if [ "$newfile" ]
    then
      # rename file to the new name
      mv $file $newfile

      # print transformation
      echo "${file} -> ${newfile}"
    fi
  done
}

# main script: change into the input directory and begin renaming...
echo "Change into $1..."
cd $1
rename_dirs
