#!/bin/bash

SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname ${SCRIPT}`
LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${SCRIPTPATH}/.."
cd ${SCRIPTPATH}

#find ${SCRIPTPATH}/.. -type f -name "dataset3-*csv" -exec ${SCRIPTPATH}/visualize.sh '{}' & \;
#FILES=$( find ${SCRIPTPATH}/../data/dataset3_201408061511 -type f -name "dataset3*csv" )
FILES=$( find ${SCRIPTPATH}/../../betke/plots/dataset_20140811* -type f -name "dataset*csv" )
for FILE in ${FILES[@]}
do
	${SCRIPTPATH}/visualize.sh "$FILE" &
done


#./visualize.sh ../data/McSvmWWTrainer/dataset3-McSvmWWTrainer-output.csv
#./visualize.sh ../data/McSvmOVATrainer/dataset3-McSvmOVATrainer-output.csv

