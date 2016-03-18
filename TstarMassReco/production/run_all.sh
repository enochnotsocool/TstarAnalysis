#!/bin/bash
#*******************************************************************************
 #
 #  Filename    : run_all.sh
 #  Description : Running the production of all the sample
 #  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 #
#*******************************************************************************

SOURCE_DIR=/wk_cms/yichen/TstarAnalysis/filtered_MiniAODs/
OUTPUT_DIR=$PWD/MASSREO
LOG_DIR=$PWD/LOG
MAX_INSTANCE=$(cat /proc/cpuinfo | grep processor | wc --lines)
MAX_INSTANCE=$((MAX_INSTANCE/2))

for channel in MuonSignal ElectronSignal ; do
   mkdir -p $OUTPUT_DIR/$channel
   mkdir -p $LOG_DIR/$channel
   for sample_dir in $(ls -d $SOURCE_DIR/$channel/* ) ; do
      sample=${sample_dir##*/};

      inputsfile_list=$(ls $sample_dir/*.root)
      if [[ $inputsfile_list == "" ]]; then
         continue;
      fi

      sample_option=""
      for file in $inputsfile_list; do
         sample_option="file://${file},"${sample_option}
      done
      sample_option=${sample_option%,}

      output_option=${OUTPUT_DIR}/${channel}/${sample}.root
      output_log=${LOG_DIR}/${channel}/${sample}.txt

      echo "Sending to background ${channel} ${sample}"
      instance_count=$( ps aux | grep ${USER} | grep "cmsRun" | wc -l )
      while [[ $instance_count -gt  $MAX_INSTANCE ]]; do
         echo "Already too many instances! ${instance_count}"
         sleep 10
         instance_count=$( ps aux | grep ${USER} | grep "cmsRun" | wc -l )
      done

      cmsRun $PWD/cmsRun_cfg.py \
             sample=$sample_option \
             output=$output_option \
             maxEvents=-1 &> $output_log &
   done
done

instance_count=$( ps aux | grep ${USER} | grep "cmsRun" | wc -l )
while [[ $instance_count != "0" ]]; do
   printf "\rStill ${instance_count} running..."
   instance_count=$( ps aux | grep ${USER} | grep "cmsRun" | wc -l )
   sleep 10
done
