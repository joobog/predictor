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

	void exportCSV(const Prediction& pred, std::string fn, char separator = ',', char comment = '#');
	void exportCSV(const std::vector<Prediction>& pred, std::string fn, char separator = ',', char comment = '#');

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef Types_INC  ----- */
