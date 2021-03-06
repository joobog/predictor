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
#include <utility>

#include <shark/Data/CVDatasetTools.h>
#include <shark/Data/DataView.h>

#include <shark/Algorithms/Trainers/CARTTrainer.h>
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h>
#include <shark/Algorithms/DirectSearch/ElitistCMA.h>
#include <shark/ObjectiveFunctions/ErrorFunction.h>
#include <shark/Algorithms/DirectSearch/GridSearch.h>

#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h>

#include <boost/archive/polymorphic_text_oarchive.hpp>

#include "analysis/SameSizeFolds.hpp" 
#include "analysis/Functions.hpp"
#include "analysis/ModelInterpreter.hpp"

namespace mlta {



	/**
	 * @brief 
	 *
	 * @param data
	 *
	 * @return 
	 */
	shark::ArgMaxConverter<shark::CARTClassifier<shark::RealVector>> 
	DTreePredictor::createDTreeModel(shark::ClassificationDataset& data, const unsigned int fold) {
		using namespace shark;
		using namespace std;

		CARTTrainer trainer;
		CARTClassifier<RealVector> model;
		trainer.train(model, data);
	
		// Nasty workaround	
		if (!m_modelExportPath.empty()) {
			std::string outputSubDir = m_modelExportPath + "/fold-" + to_string(fold);
			ModelInterpreter::exportModel(outputSubDir, &model);
		}
		shark::ArgMaxConverter<CARTClassifier<RealVector>> converter;
		converter.decisionFunction() = model;
	
		return std::move(converter);		
	}



	/**
	 * @brief 
	 *
	 * @return 
	 */
	std::tuple<VerboseTrainingset, VerbosePrediction>
	DTreePredictor::run(const shark::RegressionDataset& originTraining, const shark::RegressionDataset& originValidation, const unsigned int fold) {
		using namespace std;
		using namespace shark;

//		std::vector<VerboseTrainingset, VerbosePrediction>> data;
		std::tuple<VerboseTrainingset, VerbosePrediction> run_results;

		ClassificationDataset training = transformToClassificationDataset(originTraining, *m_classMap);
		ClassificationDataset validation = transformToClassificationDataset(originValidation, *m_classMap);

		VerboseTrainingset vtrainingset = makeVerboseTrainingset(originTraining, *m_classMap);

		auto modelProxy = createDTreeModel(training, fold);

		VerbosePrediction vprediction;
		auto elements = originValidation.elements();
		for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
			append(vprediction, iter->input, iter->label, modelProxy(iter->input), *m_classMap);
		}

		get<0>(run_results) = vtrainingset;
		get<1>(run_results) = vprediction;
		return std::move(run_results);
	}


	/**
	 * @brief 
	 *
	 * @return 
	 */
	std::vector<std::tuple<VerboseTrainingset, VerbosePrediction>>
	DTreePredictor::predictionCV(const size_t nFolds) {
		using namespace shark;
		SameSizeFolds folds{*m_data, nFolds};
		std::vector<std::tuple<VerboseTrainingset, VerbosePrediction>> data;
		for (size_t fold = 0; fold < folds.size(); ++fold) {
			data.push_back(run(folds.training(fold), folds.validation(fold), fold));
		}
		return std::move(data);
	}



	/**
	 * @brief 
	 *
	 * @return 
	 */
	std::vector<std::tuple<VerboseTrainingset, VerbosePrediction>>
	DTreePredictor::predictionInverseCV(const size_t nFolds) {
		using namespace shark;
		SameSizeFolds folds{*m_data, nFolds};
		std::vector<std::tuple<VerboseTrainingset, VerbosePrediction>> data;
		for (size_t fold = 0; fold < folds.size(); ++fold) {
			data.push_back(run(folds.validation(fold), folds.training(fold), fold));
		}
		return std::move(data);
	}

	

	/**
	 * @brief 
	 *
	 * @return 
	 */
	std::vector<std::tuple<VerboseTrainingset, VerbosePrediction>>
	DTreePredictor::predictionOnSameData() {
		using namespace shark;
		std::vector<std::tuple<VerboseTrainingset, VerbosePrediction>> data;
		data.push_back(run(*m_data, *m_data, 1));
		return std::move(data);
	}



	/**
	 * @brief 
	 *
	 * @param predicates
	 *
	 * @return 
	 */
	std::vector<std::pair<VerboseTrainingset, VerbosePrediction>> 
	DTreePredictor::predictionOfNewInput(std::vector<std::function<bool(double)>> predicates) {
		using namespace shark;
		using namespace std;

//		CondFolds condFolds(*m_data, predicates);
//		ClassificationDataset training = condFolds.training();
//		ClassificationDataset validation = condFolds.validation();
//
		std::vector<std::pair<VerboseTrainingset, VerbosePrediction>> data;

//		auto model = createDTreeModel(training);	
//
//		auto elements = validation.elements();
//		for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
//			append(data, iter->input, iter->label, model(iter->input));
//		}
		return std::move(data);
	}
}		/* -----  end of namespace mlta  ----- */
