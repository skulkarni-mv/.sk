#!/bin/bash


if [ -n "$1" ] && [ -e "$1" ]
then

  echo "shubham=0; {"  >> $1 && echo "" && echo "" && git diff $1 && echo "" && echo "" && make bzImage -j8 ; echo "" ; git reset --hard ; echo ""

else
  echo""; echo "ERROR: Please provide CORRECT 'source code / .c' file name OR file which EXISTS."; echo ""
fi


