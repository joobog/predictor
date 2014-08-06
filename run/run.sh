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

# File
if [ "$1" == "" ]; then
	CSVINFILE=$( readlink -f ${SCRIPTPATH}/../data/irisdb.csv )
else
	CSVINFILE=$( readlink -f $1 )
fi

# You can put your config here.
#	ALGORITHMS=( 'svm' 'dtree' 'ann' )
# SVMTRAINERS=( 'ova' 'adm' 'ww' 'ats' 'cs' 'atm' 'llw' )
# TYPES=( 'cv' 'icv' 'sd' 'ip' )

ALGORITHMS=( 'dtree' 'svm' 'ann' )
SVMTRAINERS=( 'ova' )
TYPES=( 'cv' 'icv' 'sd' 'ip' )

# BASH doesn't support multidimensional array directly.
# Define an array and put it in the FUNCTIONSETS array.
# Syntax: 'arrayname[@]'
FSET0=( '-' )
FSET1=( 'log(x)' )
FSET2=( 'log(x)' 'log(x)' )
FUNCTIONSETS=( 'FSET0[@]' 'FSET1[@]' 'FSET2[@]' )

PSET0=( 'x==2048000' )
PSET1=( '-' '-' '-' 'x==4' )
PSET2=( '-' )
PREDICATESETS=( 'PSET0[@]' 'PSET1[@]' 'PSET2[@]' )
### END CONFIG ###


### SCRIPT ###
#
# Generate output filename
#
function makeOutFilename() 
{
	local _workdir="$1"
	local	_filename="$2" 
	local _alg="$3"
	local _trainer="$4"	# optional
	local _functions="$5" # optional
	local _predicates="$6" # optional
	local _ptype="$7"

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
	__result="${__result}/${__result}"
	__result="${_workdir}/${__result}_output.csv"
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

for ALGORITHM in "${ALGORITHMS[@]}"; do
	for FUNCTIONSET in "${FUNCTIONSETS[@]}"; do
		TMPFUNCSET="${!FUNCTIONSET}" # just a copy
		for TYPE in "${TYPES[@]}"; do
			for PREDICATESET in "${PREDICATESETS[@]}"; do
				TMPPREDSET="${!PREDICATESET}"  # just a copy
				# Support vector machines
				if [ "${ALGORITHM}" == "svm" ]; then
					for TRAINER in "${SVMTRAINERS[@]}"; do
						CSVOUTFILE=$( makeOutFilename "${RUNPATH}" "${CSVINFILENAME}" "${ALGORITHM}" "${TRAINER}" "${TMPFUNCSET[*]}" "${TMPPREDSET[*]}" "${TYPE}" )
						CSVOUTPATH=$( dirname "${CSVOUTFILE}" );
						LOGFILE=$( makeLogFilename "${CSVOUTFILE}" )
						if $DRYRUN; then
							echo "${CSVOUTPATH}"
							echo "${CSVOUTFILE}"	
							echo "${LOGFILE}"
						else
							mkdir -p "${CSVOUTPATH}"
							COMMAND='${SCRIPTPATH}/../build/predictor --alg=svm --svm-trainer=${TRAINER} --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --predicates="${!PREDICATESET}" --csv-output-file=${CSVOUTFILE}'
							eval echo "$COMMAND" >> $LOGFILE
							eval "$COMMAND" &>> $LOGFILE &
						fi
					done
				fi

				# Artificial neural networks
				if [ "${ALGORITHM}" == "ann" ]; then
					CSVOUTFILE=$( makeOutFilename "${RUNPATH}" "${CSVINFILENAME}" "${ALGORITHM}" "" "${TMPFUNCSET[*]}" "${TMPPREDSET[*]}" "${TYPE}" )
					CSVOUTPATH=$( dirname "${CSVOUTFILE}" );
					LOGFILE=$( makeLogFilename "${CSVOUTFILE}" )
					if $DRYRUN; then
						echo "${CSVOUTPATH}"
						echo "${CSVOUTFILE}"	
						echo "${LOGFILE}"
					else
						mkdir -p "${CSVOUTPATH}"
						COMMAND='${SCRIPTPATH}/../build/predictor --alg=ann --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --predicates="${!PREDICATESET}" --csv-output-file=${CSVOUTFILE}'
						eval echo "$COMMAND" >> $LOGFILE
						eval "$COMMAND" &>> $LOGFILE &
					fi
				fi

				# Decision trees
				if [ "${ALGORITHM}" == "dtree" ]; then
					CSVOUTFILE=$( makeOutFilename "${RUNPATH}" "${CSVINFILENAME}" "${ALGORITHM}" "" "${TMPFUNCSET[*]}" "${TMPPREDSET[*]}" "${TYPE}" )
					CSVOUTPATH=$( dirname "${CSVOUTFILE}" );
					LOGFILE=$( makeLogFilename "${CSVOUTFILE}" )
					if $DRYRUN; then
						echo "${CSVOUTPATH}"
						echo "${CSVOUTFILE}"	
						echo "${LOGFILE}"
					else
						mkdir -p "${CSVOUTPATH}"
						COMMAND='${SCRIPTPATH}/../build/predictor --alg=dtree --prediction-type=${TYPE} --csv-input-file=${CSVINFILE} --transform="${!FUNCTIONSET}" --predicates="${!PREDICATESET}" --csv-output-file=${CSVOUTFILE}'
						eval echo "$COMMAND" >> $LOGFILE
						eval "$COMMAND" &>> $LOGFILE &
					fi
				fi
			done
		done
	done
done

