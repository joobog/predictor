/*
 * =====================================================================================
 *
 *       Filename:  Types.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/10/2014 09:37:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  Types_INC
#define  Types_INC

#include <shark/Data/Dataset.h>
//#include <boost/numeric/ublas/matrix.hpp>
//#include <boost/numeric/ublas/io.hpp>

#include <vector>

#include "analysis/ClassMap.hpp"

namespace mlta {
	struct Statistics {
		int maxError;
		double relError;
		double absError;
		double relTError;
		double absTError;
	};

	struct VerboseTrainingset {
		// input vector
		std::vector<std::vector<double>> inputValues;
		std::vector<std::string> inputValueLabels;

		// real output
		std::vector<std::vector<double>> outputValues;
		std::vector<std::string> outputValueLabels;
		std::vector<std::vector<unsigned int>> outputClasses;
		std::vector<std::string> outputClassLabels;
		std::vector<std::vector<Bucket>> outputBuckets;
		std::vector<std::string> outputBucketLabels;
	};

	struct VerbosePrediction : public VerboseTrainingset{
		// prediction
		std::vector<std::vector<double>> predValues;
		std::vector<std::string> predValueLabels;
		std::vector<std::vector<unsigned int>> predClasses;
		std::vector<std::string> predClassLabels;
		std::vector<std::vector<Bucket>> predBuckets;
		std::vector<std::string> predBucketLabels;
	};	

	void append(
			VerbosePrediction& data, 
			const shark::blas::vector<double>& input,
			const shark::blas::vector<double>& label,
			const unsigned int prediction,
			const ClassMap& classMap);

	VerboseTrainingset makeVerboseTrainingset(
			const shark::LabeledData<shark::RealVector, shark::RealVector>& dataset,
			const ClassMap& classMap);


//	template< typename T >
//	struct DataGroup {
//		std::vector<std::string> labels;
//		std::vector<std::vector<T>> data;
//	};
//
//	template< typename InputType, typename LabelType, typename OutputType >
//	struct Prediction {
//		CsvDataGroup<InputType> inputs;
//		CsvDataGroup<LabelTypeType> labels;
//		CsvDataGroup<OutputType> outputs;
//	};
}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef Types_INC  ----- */
