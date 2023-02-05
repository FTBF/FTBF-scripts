#!/bin/bash

back=$1
[[ ! "${back}" ]] && back=1

latest_num=$(basename $(ls -rt $HOME/MWPC/data/*.log | tail -n ${back} | head -n1  ) | sed 's/exp_//' | sed 's/\.log//')
echo $latest_num
res=$(./WCAnalyzer/WCConverter $latest_num)
#echo $res
echo  $res |  tail -n1

if [[ "${NANOWIRES}" != "" ]] ; then
  echo "$latest_num" >> $HOME/nanowires/wire_chamber_file_list.txt 
fi



