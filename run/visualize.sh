#!/bin/bash

SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname ${SCRIPT}`
LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${SCRIPTPATH}/.."
cd ${SCRIPTPATH}


DATA=$(readlink -f $1)
DATABASE=$(basename $DATA)
DATAPATH=`dirname ${DATA}`
cd ${DATAPATH}

mkdir "${DATABASE}dir"
cd "${DATAPATH}/${DATABASE}dir"

LINE=$( head -1 ${DATAPATH}/${DATABASE} )
if [ "$LINE" ==	'Data.in.Byte,Gap.in.Byte,Datasieving.1.on,Buffer.in.MByte,Performance.in.MByte/s,Prediction.in.MByte/s' ]
then
	echo "Header already there"
else
	echo "Add Header"
	sed -i '1s/^/Data.in.Byte,Gap.in.Byte,Datasieving.1.on,Buffer.in.MByte,Performance.in.MByte\/s,Prediction.in.MByte\/s\n/'  ${DATAPATH}/${DATABASE}
fi

Rscript ${SCRIPTPATH}/analyser.r ${DATAPATH}/${DATABASE} > "${DATAPATH}/${DATABASE}dir/output.log"
