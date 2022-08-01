#!/bin/bash
# Use for loops to display only odd natural numbers from 1 to 99

i=1
while [ $i -le 99 ]
do
  echo $i
  i=$((i+2))
done
