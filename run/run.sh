#/bin/bash


SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname ${SCRIPT}`
LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${SCRIPTPATH}/.."

#DATA=$(readlink -f ../${SCRIPTPATH}/../data)
#DATABASE=$(basename $DATA)
#DATAPATH=`dirname ${DATA}`
#cd ${DATAPATH}

DATAPATH="${SCRIPTPATH}/../data"

echo "Current directory: `pwd`"
#${SCRIPTPATH}/../build/predictor --alg=svm dtree ann --svm-trainer=ova adm ww ats mmr cs atm llw --csv-input-file=${DATAPATH}/irisdb.csv
#../build/predictor --alg=dtree --csv-input-file="${DATAPATH}/irisdb.csv"

#../build/predictor --alg=svm --svm-trainer=ova adm --separate-files true --csv-input-file=${DATAPATH}/irisdb.csv
#../build/predictor --alg=ann --separate-files true --csv-input-file=${DATAPATH}/irisdb.csv
#../build/predictor --alg=svm  --svm-trainer=ww ats mmr cs atm llw --csv-input-file=/home/joobog/git/trace_analyse/data/dataset3.csv
../build/predictor --alg=ann --separate-files true --transform="log(x)" "-" "log(x)" --csv-input-file=${DATAPATH}/irisdb.csv
