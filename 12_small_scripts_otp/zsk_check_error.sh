#!/bin/bash


if [ $1 != '' ]
then

  echo "shubham=0; {"  >> $1 && echo "" && echo "" && git diff $1 && echo "" && echo "" && make bzImage -j8 > kbuild.log ; echo "" ; git reset --hard ; echo ""

fi


