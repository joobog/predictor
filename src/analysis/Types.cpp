/*
 * =====================================================================================
 *
 *       Filename:  Types.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/11/2014 12:17:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <Types.hpp>

namespace mlta {

	void append(
			Prediction& data, 
			const shark::blas::vector<double> input,
			const unsigned int label,
			const unsigned int prediction)
	{
		std::vector<double> stdInput;
		for (size_t i = 0; i < input.size(); ++i) {
			stdInput.push_back(input[i]);
		}

		std::vector<unsigned int> stdOutput;
		stdOutput.push_back(label);

		std::vector<unsigned int> stdPred;
		stdPred.push_back(prediction);

		data.inputs.push_back(stdInput);
		data.outputs.push_back(stdOutput);
		data.preds.push_back(stdPred);
	}

}		/* -----  end of namespace mlta  ----- */
