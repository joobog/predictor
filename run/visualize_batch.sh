#!/bin/bash

SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname ${SCRIPT}`
LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${SCRIPTPATH}/.."
cd ${SCRIPTPATH}

./visualize.sh ../data/DTree/dtreeprediction.csv
./visualize.sh ../data/McSvmWWTrainer/dataset3-McSvmWWTrainer-output.csv
./visualize.sh ../data/McSvmOVATrainer/dataset3-McSvmOVATrainer-output.csv

