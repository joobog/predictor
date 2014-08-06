/*
 * =====================================================================================
 *
 *       Filename:  CondFolds.cpp
 *
 *    Description:  j
 *
 *        Version:  1.0
 *        Created:  08/05/2014 12:30:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "CondFolds.hpp"

#include <shark/Data/DataView.h>

namespace mlta {

	bool isValidation(shark::ClassificationDataset::InputType input, std::vector<std::function<bool(double)>> predicates) {
		bool result = true;
		for ( size_t i = 0; i < predicates.size(); ++i) {
			if (predicates[i] != nullptr) {
				result = result && predicates[i](input[i]);
			}
		}
		return result;
	}

	CondFolds::CondFolds (shark::ClassificationDataset const& data, std::vector<std::function<bool(double)>> predicates) 
	: m_data(data), m_predicates(predicates)
	{
		using namespace shark;
		DataView<ClassificationDataset> view(m_data);
		std::vector<size_t> validationIndices;
		std::vector<size_t> trainingIndices;
		for (size_t i = 0; i < view.size(); ++i) {
			ClassificationDataset::InputType input = m_data.element(i).input;
			if (isValidation(input, predicates)) {
				validationIndices.push_back(i);
			}
			else {
				trainingIndices.push_back(i);
			}
		}
		m_validation = toDataset(subset(view, validationIndices));
		m_training= toDataset(subset(view, trainingIndices));
	}

	shark::ClassificationDataset CondFolds::training() {
		return m_training;
	}

	shark::ClassificationDataset CondFolds::validation() {
		return m_validation;
	}
}		/* -----  end of namespace mlta  ----- */
