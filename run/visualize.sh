#!/bin/bash

SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname ${SCRIPT}`
LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${SCRIPTPATH}/.."
cd ${SCRIPTPATH}


DATA=$(readlink -f $1)
DATABASE=$(basename $DATA)
DATAPATH=`dirname ${DATA}`
cd ${DATAPATH}

Rscript ${SCRIPTPATH}/analyser.r ${DATAPATH}/${DATABASE} > ${DATAPATH}/output.log
