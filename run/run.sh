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
#../build/predictor --alg=ann --csv-input-file=${DATAPATH}/irisdb.csv
#../build/predictor --alg=svm  --svm-trainer=ww ats mmr cs atm llw --csv-input-file=/home/joobog/git/trace_analyse/data/dataset3.csv
#../build/predictor --alg=ann --transform="log(x)" "-" "log(x)" --csv-input-file=${DATAPATH}/irisdb.csv

${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=ova --prediction-type=cv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=adm --prediction-type=cv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=ww --prediction-type=cv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=ats --prediction-type=cv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=mmr --prediction-type=cv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=cs --prediction-type=cv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=atm --prediction-type=cv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=llw --prediction-type=cv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=dtree --prediction-type=cv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=ann --prediction-type=cv --csv-input-file=${DATAPATH}/irisdb.csv

${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=ova --prediction-type=icv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=adm --prediction-type=icv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=ww --prediction-type=icv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=ats --prediction-type=icv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=mmr --prediction-type=icv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=cs --prediction-type=icv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=atm --prediction-type=icv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=llw --prediction-type=icv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=dtree --prediction-type=icv --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=ann --prediction-type=icv --csv-input-file=${DATAPATH}/irisdb.csv


${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=ova --prediction-type=sd --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=adm --prediction-type=sd --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=ww --prediction-type=sd --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=ats --prediction-type=sd --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=mmr --prediction-type=sd --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=cs --prediction-type=sd --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=atm --prediction-type=sd --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=llw --prediction-type=sd --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=dtree --prediction-type=sd --csv-input-file=${DATAPATH}/irisdb.csv
${SCRIPTPATH}/../build/predictor --alg=ann --prediction-type=sd --csv-input-file=${DATAPATH}/irisdb.csv
