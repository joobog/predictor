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
#include <shark/ObjectiveFunctions/ErrorFunction.h>
#include <shark/ObjectiveFunctions/Loss/CrossEntropy.h>
#include <shark/Algorithms/GradientDescent/Rprop.h>
#include <shark/Algorithms/StoppingCriteria/MaxIterations.h>
#include <shark/Algorithms/StoppingCriteria/TrainingError.h>

#include <shark/Algorithms/Trainers/OptimizationTrainer.h>
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h>
#include <shark/ObjectiveFunctions/CrossValidationError.h>
#include <shark/ObjectiveFunctions/Loss/NegativeClassificationLogLikelihood.h>


#include <boost/archive/polymorphic_text_oarchive.hpp>

namespace mlta {
	Prediction ANNPredictor::predictionCV() 
	{
		using namespace std;
		using namespace shark;

		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, m_nFolds);
		Prediction data;

		double result = 0;
		for (size_t fold = 0; fold < folds.size(); ++fold) {
			FFNet<LogisticNeuron, LogisticNeuron> network;
			vector<size_t> layers = {inputDimension(*m_data), 10, numberOfClasses(*m_data)};
			network.setStructure(layers, FFNetStructures::Full, true);	
			initRandomUniform(network,-0.1,0.1);

			LabeledData<RealVector, unsigned int> training = folds.training(fold);
			LabeledData<RealVector, unsigned int> validation = folds.validation(fold);

			CrossEntropy loss;
//			NegativeClassificationLogLikelihood loss;

			ErrorFunction<RealVector, unsigned int> errorFunction(training, &network, &loss);
			IRpropPlus optimizer;
			optimizer.init(errorFunction);
			
			for (unsigned int step = 0; step != 20; ++step) {
				optimizer.step(errorFunction);
			}	
			
			network.setParameterVector(optimizer.solution().point);

			auto elements  = validation.elements();
			for (auto iter = elements.begin(); iter != elements.end(); iter++) {
				std::vector<double> stdInput;
				for (size_t i = 0; i < iter->input.size(); ++i) {
					stdInput.push_back(iter->input[i]);
				}

				std::vector<unsigned int> stdOutput;
				stdOutput.push_back(iter->label);

				std::vector<unsigned int> stdPred;
				stdPred.push_back(shark::blas::arg_max(network(iter->input)));

				data.inputs.push_back(stdInput);
				data.outputs.push_back(stdOutput);
				data.preds.push_back(stdPred);
			}
		}
	return std::move(data);
	}
	std::vector<Prediction> ANNPredictor::predictionInverseCV() {
		using namespace std;
		using namespace shark;

		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, m_nFolds);
		std::vector<Prediction> dataVector;

		double result = 0;
		for (size_t fold = 0; fold < folds.size(); ++fold) {
			Prediction data;
			FFNet<LogisticNeuron, LogisticNeuron> network;
			vector<size_t> layers = {inputDimension(*m_data), 10, numberOfClasses(*m_data)};
			network.setStructure(layers, FFNetStructures::Full, true);	
			initRandomUniform(network,-0.1,0.1);

			LabeledData<RealVector, unsigned int> training = folds.validation(fold);
			LabeledData<RealVector, unsigned int> validation = folds.training(fold);

			CrossEntropy loss;
			//			NegativeClassificationLogLikelihood loss;

			ErrorFunction<RealVector, unsigned int> errorFunction(training, &network, &loss);
			IRpropPlus optimizer;
			optimizer.init(errorFunction);

			for (unsigned int step = 0; step != 20; ++step) {
				optimizer.step(errorFunction);
			}	

			network.setParameterVector(optimizer.solution().point);

			auto elements  = validation.elements();
			for (auto iter = elements.begin(); iter != elements.end(); iter++) {

				std::vector<double> stdInput;
				for (size_t i = 0; i < iter->input.size(); ++i) {
					stdInput.push_back(iter->input[i]);
				}

				std::vector<unsigned int> stdOutput;
				stdOutput.push_back(iter->label);

				std::vector<unsigned int> stdPred;
				stdPred.push_back(shark::blas::arg_max(network(iter->input)));

				data.inputs.push_back(stdInput);
				data.outputs.push_back(stdOutput);
				data.preds.push_back(stdPred);
			}
			dataVector.push_back(data);
		}
		return std::move(dataVector);
	}

	Prediction ANNPredictor::predictionOnSameData() {
		using namespace std;
		using namespace shark;

		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, m_nFolds);
		Prediction data;

		double result = 0;
		for (size_t fold = 0; fold < folds.size(); ++fold) {
			FFNet<LogisticNeuron, LogisticNeuron> network;
			vector<size_t> layers = {inputDimension(*m_data), 10, numberOfClasses(*m_data)};
			network.setStructure(layers, FFNetStructures::Full, true);	
			initRandomUniform(network,-0.1,0.1);


			CrossEntropy loss;
//			NegativeClassificationLogLikelihood loss;

			ErrorFunction<RealVector, unsigned int> errorFunction(*m_data, &network, &loss);
			IRpropPlus optimizer;
			optimizer.init(errorFunction);
			
			for (unsigned int step = 0; step != 20; ++step) {
				optimizer.step(errorFunction);
			}	
			
			network.setParameterVector(optimizer.solution().point);

			auto elements  = m_data->elements();
			for (auto iter = elements.begin(); iter != elements.end(); iter++) {
				std::vector<double> stdInput;
				for (size_t i = 0; i < iter->input.size(); ++i) {
					stdInput.push_back(iter->input[i]);
				}

				std::vector<unsigned int> stdOutput;
				stdOutput.push_back(iter->label);

				std::vector<unsigned int> stdPred;
				stdPred.push_back(shark::blas::arg_max(network(iter->input)));

				data.inputs.push_back(stdInput);
				data.outputs.push_back(stdOutput);
				data.preds.push_back(stdPred);
			}
		}
	return std::move(data);
	}
//	Prediction ANNPredictor::prediction() 
//	{
//		using namespace std;
//		using namespace shark;
//
//		shark::FFNet<LogisticNeuron, LinearNeuron> network;
//		vector<size_t> layers = {inputDimension(m_data), 20, 20, numberOfClasses(m_data)};
//		network.setStructure(layers, FFNetStructures::Full, true);	
//		initRandomUniform(network,-0.1,0.1);
//
////		NegativeClassificationLogLikelihood	loss;
//		CrossEntropy loss;
//			
//		ErrorFunction<RealVector, unsigned int> errorFunction(&network, &loss);
//		IRpropPlus optimizer;
//		MaxIterations<> stoppingCriterion{50};
//		OptimizationTrainer<FFNet<LogisticNeuron, LinearNeuron>, unsigned int> trainer(&errorFunction, &optimizer, &stoppingCriterion);
//
//		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(m_data, m_nFolds);
//
//		double result = 0;
//		for (size_t fold = 0; fold < folds.size(); ++fold) {
//			ClassificationDataset training = folds.training(fold);
//			ClassificationDataset validation = folds.validation(fold);
//			trainer.train(network, training);
////			Data<RealVector> prediction = (*model)(validation.inputs());
//
//			auto elements  = validation.elements();
//			for (auto iter = elements.begin(); iter != elements.end(); iter++) {
//				cout << iter->input << " \t->" << iter->label << " : " << arg_max(network(iter->input)) << endl;
//			}
//		}
//	Prediction p;
//	return p;
//	}
}		/* -----  end of namespace mlta  ----- */
