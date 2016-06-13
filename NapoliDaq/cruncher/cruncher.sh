#!/bin/sh
FileDir=$1
/bin/ls -1 $FileDir/Run*.0?? > lista_raw.txt
nfile=`cat lista_raw.txt | wc | awk '{print $1}'`
echo "NFILE " $nfile
for ffile in `cat lista_raw.txt | awk '{print $1}'` ; do
     echo file=$ffile
     root -b -q -l DecodeMB_v4.C\(\"$ffile\"\) > log_tmp.txt
done
