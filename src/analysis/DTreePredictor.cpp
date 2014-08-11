/*
 * =====================================================================================
 *
 *       Filename:  DTreePredictor.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/09/2014 10:09:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "DTreePredictor.hpp"
#include "Helpers.hpp"

#include <iostream>

#include <shark/Data/CVDatasetTools.h>
#include <shark/Models/Converter.h>

#include <shark/Algorithms/Trainers/CARTTrainer.h>
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h>
#include <shark/Algorithms/DirectSearch/ElitistCMA.h>
#include <shark/ObjectiveFunctions/ErrorFunction.h>
#include <shark/Algorithms/DirectSearch/GridSearch.h>

#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h>

#include <boost/archive/polymorphic_text_oarchive.hpp>
namespace mlta {



	/**
	 * @brief 
	 *
	 * @param data
	 *
	 * @return 
	 */
	shark::ArgMaxConverter<shark::CARTClassifier<shark::RealVector>> 
	createDTreeModel(shark::ClassificationDataset& data) {
		using namespace shark;
		using namespace std;

		CARTTrainer trainer;
		CARTClassifier<RealVector> model;

		trainer.train(model, data);

		shark::ArgMaxConverter<CARTClassifier<RealVector>> converter;
		converter.decisionFunction() = model;
	
		return std::move(converter);		
	}



	/**
	 * @brief 
	 *
	 * @return 
	 */
	Prediction 
	DTreePredictor::predictionCV(const size_t nFolds) {
		using namespace shark;

		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, nFolds);

		Prediction data;

		for (size_t fold = 0; fold < folds.size(); ++fold) {
			ClassificationDataset training = folds.training(fold);
			ClassificationDataset validation = folds.validation(fold);

			auto model = createDTreeModel(training);

			auto elements = validation.elements();
			for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
				append(data, iter->input, iter->label, model(iter->input));
			}
		}
		return std::move(data);
	}



	/**
	 * @brief 
	 *
	 * @return 
	 */
	std::vector<Prediction> 
	DTreePredictor::predictionInverseCV(const size_t nFolds) {
		using namespace shark;
		using namespace std;

		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, nFolds);

		std::vector<Prediction> dataVector;

		for (size_t fold = 0; fold < folds.size(); ++fold) {
			Prediction data;
			ClassificationDataset training = folds.validation(fold);
			ClassificationDataset validation = folds.training(fold);

			auto model = createDTreeModel(training);

			auto elements = validation.elements();
			for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
				append(data, iter->input, iter->label, model(iter->input));
			}
			dataVector.push_back(data);
		}
		return std::move(dataVector);
	}

	

	/**
	 * @brief 
	 *
	 * @return 
	 */
	Prediction 
	DTreePredictor::predictionOnSameData() {
		using namespace shark;
		using namespace std;

		Prediction data;
		auto model = createDTreeModel(*m_data);

		auto elements = m_data->elements();
		for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
			append(data, iter->input, iter->label, model(iter->input));
		}
		return std::move(data);
	}


	/**
	 * @brief 
	 *
	 * @param predicates
	 *
	 * @return 
	 */
	Prediction 
	DTreePredictor::predictionOfNewInput(std::vector<std::function<bool(double)>> predicates) {
		using namespace shark;
		using namespace std;

		CondFolds condFolds(*m_data, predicates);
		ClassificationDataset training = condFolds.training();
		ClassificationDataset validation = condFolds.validation();

		Prediction data;

		auto model = createDTreeModel(training);	

		auto elements = validation.elements();
		for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
			append(data, iter->input, iter->label, model(iter->input));
		}
		return std::move(data);
	}
}		/* -----  end of namespace mlta  ----- */
