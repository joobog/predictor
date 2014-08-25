/*
 * =====================================================================================
 *
 *       Filename:  SameSizeFolds.cpp
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

#include "SameSizeFolds.hpp"

#include <shark/Data/DataView.h>

#include <algorithm>
#include <random>
#include <iostream>

namespace mlta {

	/**
	 * @brief 
	 *
	 * @param shark::LabeledData
	 * @param nFolds
	 */
	SameSizeFolds::SameSizeFolds (shark::LabeledData<shark::RealVector, shark::RealVector> const& data, const size_t nFolds)
	: m_data(data), m_foldIndices(nFolds)
	{
		using namespace shark;

		// create and shuffle indices
		//
		DataView<LabeledData<RealVector, RealVector>> view(m_data);
		std::vector<size_t> allIndices(view.size());
		for (int i = 0; i < allIndices.size(); ++i) {
			allIndices[i] = i;
		}
		unsigned int seed = 1000;
		std::shuffle(allIndices.begin(), allIndices.end(), std::default_random_engine(seed));
		size_t foldSize = allIndices.size() / nFolds;

		for (int i = 0; i < nFolds; ++i) {
			std::vector<size_t> indices;
			for (int j = 0; j < foldSize; ++j) {
				indices.push_back(allIndices[i * foldSize + j]);
			}
			m_foldIndices[i] = indices;
		}		
		std::cout << __PRETTY_FUNCTION__ << "\tsize of foldIndices" << m_foldIndices[0].size() << std::endl;
	}

	/**
	 * @brief 
	 *
	 * @param fold
	 *
	 * @return 
	 */
	shark::LabeledData<shark::RealVector, shark::RealVector> SameSizeFolds::training(const size_t fold) {
		using namespace shark;
		DataView<LabeledData<RealVector, RealVector>> view(m_data);
		std::vector<size_t> trainingIndices;
		trainingIndices.reserve( m_foldIndices[0].size() * (m_foldIndices.size() - 1)); // preallocate memory
		for (size_t i = 0; i < m_foldIndices.size(); ++i) {
			if (i != fold) {
				trainingIndices.insert( trainingIndices.end(), m_foldIndices[i].begin(), m_foldIndices[i].end() );
			}
		}
		LabeledData<RealVector, RealVector> trainingset = toDataset(subset(view, trainingIndices));
		return std::move(trainingset);
	}

	/**
	 * @brief 
	 *
	 * @param fold
	 *
	 * @return 
	 */
	shark::LabeledData<shark::RealVector, shark::RealVector>  SameSizeFolds::validation(const size_t fold) {
		using namespace shark;
		DataView<LabeledData<RealVector, RealVector>> view(m_data);
		LabeledData<RealVector, RealVector> validationset = toDataset(subset(view, m_foldIndices[fold]));
		return std::move(validationset);
	}
}		/* -----  end of namespace mlta  ----- */
