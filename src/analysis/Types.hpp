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

namespace mlta {
	struct Statistics {
		int maxError;
		double relError;
		double absError;
		double relTError;
		double absTError;
	};

	struct Prediction {
		std::vector<std::vector<double>> inputs;
		std::vector<std::string> inputLabels;
		std::vector<std::vector<unsigned int>> outputs;
		std::vector<std::string> outputLabels;
		std::vector<std::vector<unsigned int>> preds;
		std::vector<std::string> predLabels;
	};	

	void append(
			Prediction& data, 
			const shark::blas::vector<double> input,
			const unsigned int label,
			const unsigned int prediction);

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
