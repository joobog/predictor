/*
 * =====================================================================================
 *
 *       Filename:  ANNAnalyser.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/10/2014 07:34:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ANNPredictor.hpp"
#include "Helpers.hpp"

#include <iostream>
#include <vector>

#include <shark/Data/CVDatasetTools.h>


#include <shark/Models/FFNet.h>
#include <shark/Models/Converter.h>
#include <shark/ObjectiveFunctions/ErrorFunction.h>
#include <shark/ObjectiveFunctions/Loss/CrossEntropy.h>
#include <shark/Algorithms/GradientDescent/Rprop.h>
//#include <shark/Algorithms/StoppingCriteria/MaxIterations.h>
#include <shark/Algorithms/StoppingCriteria/TrainingError.h>

#include <shark/Algorithms/Trainers/OptimizationTrainer.h>
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h>
#include <shark/ObjectiveFunctions/CrossValidationError.h>

#include <boost/archive/polymorphic_text_oarchive.hpp>

namespace mlta {

	/**
	 * @brief Creates a trained proxy to a FFNet model
	 *
	 * @param data training data
	 *
	 * @return proxy to FFNet model
	 */
	shark::ArgMaxConverter<shark::FFNet<shark::LogisticNeuron, shark::LinearNeuron>> 
	createFFNetModel(shark::ClassificationDataset& data) {
		using namespace shark;
		using namespace std;

		FFNet<LogisticNeuron, LinearNeuron> model;
		vector<size_t> layers = {inputDimension(data), 10, numberOfClasses(data)};
		model.setStructure(layers, FFNetStructures::Full, true);	
		initRandomUniform(model,-0.1,0.1);

		CrossEntropy loss;

		ErrorFunction<RealVector, unsigned int> errorFunction(data, &model, &loss);
		IRpropPlus optimizer;
		optimizer.init(errorFunction);

//		TrainingError<> stoppingCriterion(10, 1.e-5);

		// Lower precision for faster learning 
		TrainingError<> stoppingCriterion(10, 1.e-3);
		OptimizationTrainer<FFNet<LogisticNeuron, LinearNeuron>, unsigned int> trainer(&loss, &optimizer, &stoppingCriterion);
		trainer.train(model, data);

		shark::ArgMaxConverter<FFNet<LogisticNeuron, LinearNeuron>> converter;
		converter.decisionFunction() = model;
	
		return std::move(converter);		
	}



	/**
	 * @brief Cross Validation
	 *
	 * @return 
	 */
	std::pair<TrainingSet, Prediction> 
	ANNPredictor::predictionCV(const size_t nFolds) {
		using namespace std;
		using namespace shark;

		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, nFolds);

		std::pair<TrainingSet, Prediction> data;
		for (size_t fold = 0; fold < folds.size(); ++fold) {

			ClassificationDataset training = folds.training(fold);
			ClassificationDataset validation = folds.validation(fold);

			auto model = createFFNetModel(training);

			auto elements  = validation.elements();
			for (auto iter = elements.begin(); iter != elements.end(); iter++) {
				append(data, iter->input, iter->label, model(iter->input));
			}
		}
	return std::move(data);
	}


	/**
	 * @brief Inverse Cross Validation
	 *
	 * @return 
	 */
	std::vector<std::pair<TrainingSet, Prediction>> 
	ANNPredictor::predictionInverseCV(const size_t nFolds) {
		using namespace std;
		using namespace shark;

		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, nFolds);
		std::vector<std::pair<TrainingSet, Prediction>> dataVector;

		for (size_t fold = 0; fold < folds.size(); ++fold) {
			ClassificationDataset training = folds.validation(fold);
			ClassificationDataset  validation = folds.training(fold);
			auto model = createFFNetModel(training);

			std::pair<TrainingSet, Prediction> data;
			auto elements  = validation.elements();
			for (auto iter = elements.begin(); iter != elements.end(); iter++) {
				append(data, iter->input, iter->label, model(iter->input));
			}
			dataVector.push_back(data);
		}
		return std::move(dataVector);
	}



	/**
	 * @brief Trainingset and validation are the same
	 *
	 * @return 
	 */
	std::pair<TrainingSet, Prediction> 
	ANNPredictor::predictionOnSameData() {
		using namespace std;
		using namespace shark;

		auto model = createFFNetModel(*m_data);

		std::pair<TrainingSet, Prediction> data;
		auto elements  = m_data->elements();
		for (auto iter = elements.begin(); iter != elements.end(); iter++) {
			append(data, iter->input, iter->label, model(iter->input));
		}
		return std::move(data);
	}



	/**
	 * @brief Predict new categories of data
	 *
	 * @param predicates
	 *
	 * @return 
	 */
	std::pair<TrainingSet, Prediction> 
	ANNPredictor::predictionOfNewInput(std::vector<std::function<bool(double)>> predicates) {
		using namespace std;
		using namespace shark;

		std::pair<TrainingSet, Prediction> data;
		CondFolds condFolds(*m_data, predicates);
		ClassificationDataset training = condFolds.training();
		ClassificationDataset validation = condFolds.validation();

		auto model = createFFNetModel(training);

		auto elements  = validation.elements();
		for (auto iter = elements.begin(); iter != elements.end(); iter++) {
			append(data, iter->input, iter->label, model(iter->input));
		}
		return std::move(data);
	}
}		/* -----  end of namespace mlta  ----- */
