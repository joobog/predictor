#/bin/bash

### DESCRIPTION ###
# This scripts runs a set of predictions. Each prediction is executed in 
# in its own thread. This can be an advantage, when running on a large scaled
# server, but it can easily overload your workstation.

# Each run creates a new folder "basename_timestamp" in the same 
# directory as the csv-input-file. Inside this folder, each prediction 
# creates its own folder "basename_parameterlist". You can see the list of 
# files if you set DRYRUN variable to true.

# See documentation of predictor for supported parameter.
### END DESCRIPTION ###

### BASICS ###
SCRIPT=$( readlink -f "$0" )
SCRIPTPATH=$( dirname "${SCRIPT}" )
## BASICS ###

### CONFIG ###
# If true, shows a list of files.
export DRYRUN=false
SCRIPT_DEBUG=0

# File
if [ "$1" == "" ]; then
	#CSVINFILE=$( readlink -f ${SCRIPTPATH}/../data/dataset3.csv )
	#CSVINFILE=$( readlink -f ${SCRIPTPATH}/../data/irisdb.csv )
	#CSVINFILE=$( readlink -f ${SCRIPTPATH}/../../betke/plots/prediction_error/averageDataGap.csv )
	#CSVINFILE=$( readlink -f ${SCRIPTPATH}/../../betke/plots/prediction_error/datasetDataGap.csv )
	#CSVINFILE=$( readlink -f ${SCRIPTPATH}/../../betke/plots/prediction_error/averageFillGap.csv )
	#CSVINFILE=$( readlink -f ${SCRIPTPATH}/../../betke/plots/prediction_error/datasetFillGap.csv )
	#CSVINFILE=$( readlink -f ${SCRIPTPATH}/../../betke/exp/cleaned/data-gap/input.csv )
	CSVINFILE=$( readlink -f ${SCRIPTPATH}/../../betke/exp/cleaned/fill-gap/input.csv )
else
	CSVINFILE=$( readlink -f $1 )
fi

# You can put your config here.
#	ALGORITHMS=( 'lsvm' 'svm' 'dtree' 'ann' )
# SVMTRAINERS=( 'ova' 'adm' 'ww' 'ats' 'cs' 'atm' 'llw' )
# TYPES=( 'cv' 'icv' 'sd' 'ip' )

ALGORITHMS=( 'dtree' 'rf' )
SVMTRAINERS=( 'ova' 'ww' )
TYPES=( 'icv' )
FOLDNUMSET=( 2 4 8 16 32 64 )

#ALGORITHMS=( 'dtree' 'rf' )
#SVMTRAINERS=( 'ova' 'ww' )
#TYPES=( 'cv' )
#FOLDNUMSET=( 2 3 4 5 6 7 8 )

#ALGORITHMS=( 'dtree' )
#SVMTRAINERS=( 'ova' 'ww' )
#TYPES=( 'sd' )
#FOLDNUMSET=( 2 )


# BASH doesn't support multidimensional array directly.
# Define an array and put it in the FUNCTIONSETS array.
# Syntax: 'arrayname[@]'
FSET0=( '-' )
FSET1=( 'log(x)' )
FSET2=( 'log(x)' 'log(x)' )
FUNCTIONSETS=( 'FSET0[@]' )

PSET0=( '-' )
PSET1=( 'x==2048000' )
PSET2=( '-' '-' '-' 'x==4' )
PREDICATESETS=( 'PSET0[@]' )
### END CONFIG ###

export JOBS=""

debug() {
	[[ ${SCRIPT_DEBUG} == 1 ]] && "$@" || true 
}

logger() {
	echo "$@"
}


collect_jobs() {
	local _command="$1"
	local _log="$2"
	if ${DRYRUN}; then 
		eval echo "${_command}"; 
		eval echo "${_log}"; 
	fi
	if ! ${DRYRUN}; then 
		JOBS="$JOBS $(eval echo ${_command})\n"
		JOBS="$JOBS $(eval echo ${_log})\n"
	fi
}

### SCRIPT ###
#
# Generate output filename
# Optional arguments can be empty strings
#
# /workdir/ptype/alg/trainer/transform/predicate/fold#
# /home/joobog/git/cv/svm/ww/trans-log(x)/pred-x>100/fold-8
function makeOutDirname() 
{
	local _workdir="$1"    # required
	local _ptype="$2"      # required
	local _alg="$3"        # required

	local _trainer="$4"    # optional
	local _functions="$5"  # optional
	local _predicates="$6" # optional
	local _foldnum="$7"    # optional
	local _timestamp="$8"  # optional

	### (1) ### TYPE
	__result="${_ptype}"

	### (1.2) ### TIMESTAMP
	if [ "${_timestamp}H" != "" ]; then
		__result="${__result}_${_timestamp}"
	fi

	### (2) ###	ALGORITHM
	__result="${__result}/${_alg}"

	### (3) ### OPTIONAL TRAINER
	if [ "${_trainer}" != "" ]; then
		__result="${__result}/${_trainer}"
	fi

	### (4) ### OPTIONAL TRANSFORMATION
	if [ "${_functions}" != "" -a "${_functions}" != "-" ]; then
		_functions="${_functions// /#}"
		__result="${__result}/trans_${_functions}"
	fi

	### (5) ### OPTIONAL PREDICATES
	if [ "${_predicates}" != "" -a "${_predicates}" != "-" ]; then
		_predicates="${_predicates// /#}"
		__result="${__result}/pred-${_predicates}"
	fi

	### (6) ### OPTIONAL FOLD NUMBER
	if [ "${_foldnum}" != "" ]; then 
		__result="${__result}/folds-${_foldnum}"
	fi

	### (7) ### ABSOLUTE PATH
	__result="${_workdir}/${__result}"

	echo "${__result}"
}


#
# Generate log filename
#
function makeLogFilename()
{
	local _csvfile="$1"
	local _logdir=$( dirname "${_csvfile}" )

	__result="${_logdir}/log"
	echo "${__result}" 
}


SCRIPT=$( readlink -f "$0" )
SCRIPTPATH=$( dirname "${SCRIPT}" )

CSVINFILEBASE=$( basename ${CSVINFILE} )
CSVINFILEPATH=$( dirname ${CSVINFILE} )
CSVINFILEEXT="${CSVINFILEBASE##*.}"
CSVINFILENAME="${CSVINFILEBASE%.*}"


RUNPATH="${CSVINFILEPATH}"
TIMESTAMP="$( date +%Y%m%d%H%M )"

if $DRYRUN
then
	echo "dryrun no folder created"
else
	if [ ! -d ${RUNPATH} ]; then
		mkdir -p ${RUNPATH}
		echo "Results will be saved in ${RUNPATH}"
	else
		echo "path exists ${RUNPATH}"
	fi
fi


for FUNCTIONSET in "${FUNCTIONSETS[@]}"; do
	TMPFUNCSET="${!FUNCTIONSET}" # just a copy
	debug echo "debug functions $TMPFUNCSET"
	for TYPE in "${TYPES[@]}"; do
		debug echo "debug type $TYPE"
		for ALGORITHM in "${ALGORITHMS[@]}"; do
			debug echo "debug algorithm $ALGORITHM"
			# Support vector machines
			if [ "${ALGORITHM}" == "svm" -o "${ALGORITHM}" == "lsvm" ]; then
				for TRAINER in "${SVMTRAINERS[@]}"; do

					if [ "${TYPE}" == "cv" -o "${TYPE}" == "icv" ]; then
						for FOLDNUM in "${FOLDNUMSET[@]}"; do
							debug echo "debug $FOLDNUM"
							OUTDIR=$( makeOutDirname "${RUNPATH}" "${TYPE}" "${ALGORITHM}" "${TRAINER}" "${TMPFUNCSET[*]}" "" "${FOLDNUM}" "${TIMESTAMP}" )
							CSVOUTPATH=$( dirname "${OUTDIR}" );
							LOGFILE=$( makeLogFilename "${OUTDIR}" )
							mkdir -p "${CSVOUTPATH}"
							COMMAND='${SCRIPTPATH}/../build/predictor --alg=$ALGORITHM --svm-trainer=${TRAINER} --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --number-of-folds="${FOLDNUM}" --output-dir=${OUTDIR}'
							collect_jobs "${COMMAND}" "${LOGFILE}"
						done

					elif [ ${TYPE} == "ip" ]; then
						for PREDICATESET in "${PREDICATESETS[@]}"; do
							debug echo "debug predicates ${!PREDICATESET}"
							TMPPREDSET="${!PREDICATESET}"
							OUTDIR=$( makeOutDirname "${RUNPATH}" "${TYPE}" "${ALGORITHM}" "${TRAINER}" "${TMPFUNCSET[*]}" "${TMPPREDSET[*]}" "${FOLDNUM}" "${TIMESTAMP}" )
							CSVOUTPATH=$( dirname "${OUTDIR}" );
							LOGFILE=$( makeLogFilename "${OUTDIR}" )
							mkdir -p "${CSVOUTPATH}"
							COMMAND='${SCRIPTPATH}/../build/predictor --alg=$ALGORITHM --svm-trainer=${TRAINER} --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --predicates="${!PREDICATESET}" --output-dir=${OUTDIR}'
							collect_jobs "${COMMAND}" "${LOGFILE}"
						done

					elif [ ${TYPE} == "sd" ]; then
						OUTDIR=$( makeOutDirname "${RUNPATH}" "${TYPE}" "${ALGORITHM}" "${TRAINER}" "${TMPFUNCSET[*]}" "" "" "${TIMESTAMP}" )
						CSVOUTPATH=$( dirname "${OUTDIR}" );
						LOGFILE=$( makeLogFilename "${OUTDIR}" )
						mkdir -p "${CSVOUTPATH}"
						COMMAND='${SCRIPTPATH}/../build/predictor --alg=$ALGORITHM --svm-trainer=${TRAINER} --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --output-dir=${OUTDIR}'
							collect_jobs "${COMMAND}" "${LOGFILE}"
					fi

				done
			fi

			## Artificial neural networks and decision trees
			if [ "${ALGORITHM}" == "dtree" -o  "${ALGORITHM}" == "rf" -o  "${ALGORITHM}" == "ann" ];  then
				if [ "${TYPE}" == "cv" -o "${TYPE}" == "icv" ]; then
					for FOLDNUM in "${FOLDNUMSET[@]}"; do
						debug echo "debug $FOLDNUM"
						OUTDIR=$( makeOutDirname "${RUNPATH}" "${TYPE}" "${ALGORITHM}" "" "${TMPFUNCSET[*]}" "" "${FOLDNUM}" "${TIMESTAMP}" )
						CSVOUTPATH=$( dirname "${OUTDIR}" );
						LOGFILE=$( makeLogFilename "${OUTDIR}" )
						mkdir -p "${CSVOUTPATH}"
						COMMAND='${SCRIPTPATH}/../build/predictor --alg=${ALGORITHM}  --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --number-of-folds="${FOLDNUM}" --output-dir=${OUTDIR}'
						collect_jobs "${COMMAND}" "${LOGFILE}"
					done

				elif [ ${TYPE} == "ip" ]; then
					for PREDICATESET in "${PREDICATESETS[@]}"; do
						debug echo "debug predicates ${!PREDICATESET}"
						TMPPREDSET="${!PREDICATESET}"
						OUTDIR=$( makeOutDirname "${RUNPATH}" "${TYPE}" "${ALGORITHM}" "" "${TMPFUNCSET[*]}" "${TMPPREDSET[*]}" "${FOLDNUM}" "${TIMESTAMP}" )
						CSVOUTPATH=$( dirname "${OUTDIR}" );
						LOGFILE=$( makeLogFilename "${OUTDIR}" )
						mkdir -p "${CSVOUTPATH}"
						COMMAND='${SCRIPTPATH}/../build/predictor --alg=${ALGORITHM}  --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --predicates="${!PREDICATESET}" --output-dir=${OUTDIR}'
						collect_jobs "${COMMAND}" "${LOGFILE}"
					done

				elif [ ${TYPE} == "sd" ]; then
					OUTDIR=$( makeOutDirname "${RUNPATH}" "${TYPE}" "${ALGORITHM}" "" "${TMPFUNCSET[*]}" "" "" "${TIMESTAMP}" )
					CSVOUTPATH=$( dirname "${OUTDIR}" );
					LOGFILE=$( makeLogFilename "${OUTDIR}" )
					mkdir -p "${CSVOUTPATH}"
					COMMAND='${SCRIPTPATH}/../build/predictor --alg=${ALGORITHM} --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --output-dir=${OUTDIR}'
					collect_jobs "${COMMAND}" "${LOGFILE}"
				fi
			fi

		done
	done
done


run() {
	local _command="$1"
	local _log="$2"

	if ${DRYRUN}; then 
		eval echo "first param: ${_command}"; 
		eval echo "second param: ${_log}";
	fi
	if ! ${DRYRUN}; then 
		eval echo "run: ${_command}"; 
		eval echo "log: ${_log}";
		echo "" > ${_log}
		eval echo "${_command}" &>> ${_log}
		echo "started $( date )" &>> ${_log}
		echo "blah" &>> ${_log}
		eval "${_command}" &>> ${_log}
		echo "finished $( date )" &>> ${_log}
	fi
}

func() {
	echo $1
	echo $2
}

export -f run
export -f func

export SHELL="/bin/bash"
JOBS="$( echo ${JOBS} | sed -s 's/..$//g' )"
echo -e ${JOBS} | parallel -n 2 -j 16 run '{}'
