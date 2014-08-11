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
DRYRUN=false
SCRIPT_DEBUG=0

# File
if [ "$1" == "" ]; then
	#CSVINFILE=$( readlink -f ${SCRIPTPATH}/../data/dataset3.csv )
	#CSVINFILE=$( readlink -f ${SCRIPTPATH}/../data/irisdb.csv )
	CSVINFILE=$( readlink -f ${SCRIPTPATH}/../../betke/plots/dataset.csv )
else
	CSVINFILE=$( readlink -f $1 )
fi

# You can put your config here.
#	ALGORITHMS=( 'svm' 'dtree' 'ann' )
# SVMTRAINERS=( 'ova' 'adm' 'ww' 'ats' 'cs' 'atm' 'llw' )
# TYPES=( 'cv' 'icv' 'sd' 'ip' )

ALGORITHMS=( 'dtree' )
SVMTRAINERS=( 'ova' )
TYPES=( 'icv' )
FOLDNUMSET=( 2 4 8 16 32 64 )

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


debug() {
	[[ ${SCRIPT_DEBUG} == 1 ]] && "$@" || true 
}

logger() {
	echo "$@"
}

run() {
	local _command="$1"
	local _log="$2"
	if ${DRYRUN}; then eval echo "${_command}"; fi
	if ! ${DRYRUN}; then 
		echo "" > ${_log}
		eval echo "${_command}" >> ${_log}
		echo `date` >> ${_log}
		eval "${_command}" &>> ${_log}
		echo `date` >> ${_log}
	fi
}

### SCRIPT ###
#
# Generate output filename
# Optional arguments can be empty strings
#
function makeOutFilename() 
{
	local _workdir="$1"    # required
	local	_filename="$2"   # reguired
	local _alg="$3"        # required
	local _trainer="$4"    # optional
	local _functions="$5"  # optional
	local _predicates="$6" # optional
	local _ptype="$7"      # required
	local _foldnum="$8"    # optional

	__result="${_filename}_${_alg}"
	if [ "${_trainer}" != "" ]; then
		__result="${__result}_${_trainer}"
	fi
	if [ "${_functions}" != "" ]; then
		#_functions=$( echo ${_functions[*]} | tr -d ' ' )
		_functions="${_functions// /#}"
		__result="${__result}_trans_${_functions}"
	fi
	if [ "${_predicates}" != "" ]; then
		#_predicates=$( echo ${_predicates[*]} | tr -d ' ' )
		_predicates="${_predicates// /#}"
		__result="${__result}_pred_${_predicates}"
	fi
	__result="${__result}_${_ptype}"
	if [ "${_foldnum}" != "" ]; then 
		__result="${__result}_${_foldnum}"
	fi
	__result="${__result}/${__result}"
	__result="${_workdir}/${__result}"
	__result="${__result}_output.csv"
	__result="${__result//__/_}"
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


RUNPATH="${CSVINFILEPATH}/${CSVINFILENAME}_`date +%Y%m%d%H%M`"

if ! $DRYRUN
then
	if [ ! -d ${RUNPATH} ]; then
		mkdir -p ${RUNPATH}
		echo "Results will be saved in ${RUNPATH}"
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
			if [ "${ALGORITHM}" == "svm" ]; then
				for TRAINER in "${SVMTRAINERS[@]}"; do

					if [ "${TYPE}" == "cv" -o "${TYPE}" == "icv" ]; then
						for FOLDNUM in "${FOLDNUMSET[@]}"; do
							debug echo "debug $FOLDNUM"
							CSVOUTFILE=$( makeOutFilename "${RUNPATH}" "${CSVINFILENAME}" "${ALGORITHM}" "${TRAINER}" "${TMPFUNCSET[*]}" "${TYPE}" "${FOLDNUM}" )
							CSVOUTPATH=$( dirname "${CSVOUTFILE}" );
							LOGFILE=$( makeLogFilename "${CSVOUTFILE}" )
							mkdir -p "${CSVOUTPATH}"
							COMMAND='${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=${TRAINER} --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --number-of-folds="${FOLDNUM}" --csv-output-file=${CSVOUTFILE}'
							run "${COMMAND}" "${LOGFILE}" &
						done

					elif [ ${TYPE} == "ip" ]; then
						for PREDICATESET in "${PREDICATESETS[@]}"; do
							debug echo "debug predicates ${!PREDICATESET}"
							TMPPREDSET="${!PREDICATESET}"
							CSVOUTFILE=$( makeOutFilename "${RUNPATH}" "${CSVINFILENAME}" "${ALGORITHM}" "${TRAINER}" "${TMPFUNCSET[*]}" "${TMPPREDSET[*]}" "${TYPE}" )
							CSVOUTPATH=$( dirname "${CSVOUTFILE}" );
							LOGFILE=$( makeLogFilename "${CSVOUTFILE}" )
							mkdir -p "${CSVOUTPATH}"
							COMMAND='${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=${TRAINER} --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --predicates="${!PREDICATESET}" --csv-output-file=${CSVOUTFILE}'
							run "${COMMAND}" "${LOGFILE}" &
						done

					elif [ ${TYPE} == "sd" ]; then
						CSVOUTFILE=$( makeOutFilename "${RUNPATH}" "${CSVINFILENAME}" "${ALGORITHM}" "${TRAINER}" "${TMPFUNCSET[*]}" "${TYPE}" )
						CSVOUTPATH=$( dirname "${CSVOUTFILE}" );
						LOGFILE=$( makeLogFilename "${CSVOUTFILE}" )
						mkdir -p "${CSVOUTPATH}"
						COMMAND='${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=${TRAINER} --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --csv-output-file=${CSVOUTFILE}'
							run "${COMMAND}" "${LOGFILE}" &
					fi

				done
			fi

			## Artificial neural networks and decision trees
			if [ "${ALGORITHM}" == "dtree" ] || [ "${ALGORITHM}" == "ann" ];  then
				if [ "${TYPE}" == "cv" -o "${TYPE}" == "icv" ]; then
					for FOLDNUM in "${FOLDNUMSET[@]}"; do
						debug echo "debug $FOLDNUM"
						CSVOUTFILE=$( makeOutFilename "${RUNPATH}" "${CSVINFILENAME}" "${ALGORITHM}" "${TMPFUNCSET[*]}" "${TYPE}" "${FOLDNUM}" )
						CSVOUTPATH=$( dirname "${CSVOUTFILE}" );
						LOGFILE=$( makeLogFilename "${CSVOUTFILE}" )
						mkdir -p "${CSVOUTPATH}"
						COMMAND='${SCRIPTPATH}/../build/predictor --alg=${ALGORITHM}  --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --number-of-folds="${FOLDNUM}" --csv-output-file=${CSVOUTFILE}'
						run "${COMMAND}" "${LOGFILE}" &
					done

				elif [ ${TYPE} == "ip" ]; then
					for PREDICATESET in "${PREDICATESETS[@]}"; do
						debug echo "debug predicates ${!PREDICATESET}"
						TMPPREDSET="${!PREDICATESET}"
						CSVOUTFILE=$( makeOutFilename "${RUNPATH}" "${CSVINFILENAME}" "${ALGORITHM}" "${TRAINER}" "${TMPFUNCSET[*]}" "${TMPPREDSET[*]}" "${TYPE}" )
						CSVOUTPATH=$( dirname "${CSVOUTFILE}" );
						LOGFILE=$( makeLogFilename "${CSVOUTFILE}" )
						mkdir -p "${CSVOUTPATH}"
						COMMAND='${SCRIPTPATH}/../build/predictor --alg=${ALGORITHM}  --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --predicates="${!PREDICATESET}" --csv-output-file=${CSVOUTFILE}'
						run "${COMMAND}" "${LOGFILE}" &
					done

				elif [ ${TYPE} == "sd" ]; then
					CSVOUTFILE=$( makeOutFilename "${RUNPATH}" "${CSVINFILENAME}" "${ALGORITHM}" "${TRAINER}" "${TMPFUNCSET[*]}" "${TYPE}" )
					CSVOUTPATH=$( dirname "${CSVOUTFILE}" );
					LOGFILE=$( makeLogFilename "${CSVOUTFILE}" )
					mkdir -p "${CSVOUTPATH}"
					COMMAND='${SCRIPTPATH}/../build/predictor --alg=${ALGORITHM} --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --csv-output-file=${CSVOUTFILE}'
					run "${COMMAND}" "${LOGFILE}" &
				fi
			fi

		done
	done
done
