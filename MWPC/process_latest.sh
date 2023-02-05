#!/bin/bash
set -o nounset
set -o errexit

SPILL=-1
N_SPILLS=-1
N_PREV=

function print_the_help {
  echo "USAGE: ${0} [-s <spill_number>] [-n <n_spills>]"
  echo "  OPTIONS: "
  echo "            -N,--N-prev        Process Nth previous file "
  echo "            -s,--spill         Start at spill number"
  echo "            -n,--n_spills      Number of spills"
  exit 
}

function yes_or_no {
  while true; do
    read -p "$* [y/n]: " yn
    case $yn in
      [Yy]*) return 0 ;;
      [Nn]*) echo "No entered" ; return 1 ;;
    esac
  done
}

#if [[ $# -eq 0 ]] ; then
#  print_the_help
#  exit 
#fi


POSITIONAL=()
while [[ $# -gt 0 ]]
do
  key="$1"

  case $key in
    -h|--help)
      shift # past argument
      print_the_help
      ;;
    -s|--spill)
      SPILL="$2"
      shift # past argument
      shift # past value
      ;;
    -N|--N_prev)
      N_PREV="$2"
      shift # past argument
      shift # past value
      ;;
    -n|--n_spills)
      N_SPILLS="$2"
      shift # past argument
      shift # past value
      ;;
    *)    # unknown option
      #POSITIONAL+=("$1") # save it in an array for later
      echo "unknown option $1"
      print_the_help
      shift # past argument
      ;;
  esac
done
#set -- "${POSITIONAL[@]}" # restore positional parameters

back=${N_PREV}
[[ ! "${N_PREV}" ]] && back=1

latest_num=$(basename $(ls -rt $HOME/MWPC/data/*.log | tail -n ${back} | head -n1  ) | sed 's/exp_//' | sed 's/\.log//')
echo $latest_num
./WCAnalyzer/WCConverter $latest_num $SPILL $N_SPILLS | tee  /tmp/process_latest.log
#echo $res
cat /tmp/process_latest.log |  tail -n1

if [[ "${NANOWIRES}" != "" ]] ; then
  echo "$latest_num" >> $HOME/nanowires/wire_chamber_file_list.txt 
fi



