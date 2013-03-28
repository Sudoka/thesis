#!/bin/bash
#
# record the diff lines for all sections
# Dan Cashman dcashman@cs.ucsd.edu
#
working_dir=/home/dcashman/thesis/misc/rand_repro/diffs

cd $working_dir
for file in ${working_dir}/*
do
  ./extract_diff_num.pl $file results.txt
  #echo $file
done