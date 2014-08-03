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
#include <typeinfo>

#include <shark/Data/CVDatasetTools.h>

#include <shark/Algorithms/Trainers/CARTTrainer.h>
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h>
#include <shark/Algorithms/DirectSearch/ElitistCMA.h>
#include <shark/ObjectiveFunctions/ErrorFunction.h>
#include <shark/Algorithms/DirectSearch/GridSearch.h>

#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h>

#include <boost/archive/polymorphic_text_oarchive.hpp>
namespace mlta {

	Prediction DTreePredictor::predictionCV()
	{
		using namespace shark;
		using namespace std;

		CARTTrainer trainer;
		CARTClassifier<RealVector> model;
		ZeroOneLoss<unsigned int, RealVector> loss;
		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, m_nFolds);

		Prediction data;

		for (size_t fold = 0; fold < folds.size(); ++fold) {
			ClassificationDataset training = folds.training(fold);
			training.makeIndependent();
			ClassificationDataset validation = folds.validation(fold);
			validation.makeIndependent();
			trainer.train(model, training);
			Data<RealVector> prediction = model(validation.inputs());

			auto elements = validation.elements();
			for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
				// convert boost vector to std vector
				
				std::vector<double> stdInput;

//			// this (elegant) version doesn't work for some reason
//				for(const double& x : iter->input) {
//					stdInput.push_back(x);
//				}

				for (size_t i = 0; i < iter->input.size(); ++i) {
					stdInput.push_back(iter->input[i]);
				}

				std::vector<unsigned int> stdOutput;
				double label = iter->label;
				stdOutput.push_back(label);

				std::vector<unsigned int> stdPred;
				stdPred.push_back(arg_max(model(iter->input)));

				data.inputs.push_back(stdInput);
				data.outputs.push_back(stdOutput);
				data.preds.push_back(stdPred);
			}
		}
		return std::move(data);
	}

//	Prediction DTreePredictor::prediction2()
//	{
//		using namespace shark;
//		using namespace std;
//
//		CARTTrainer trainer;
//		CARTClassifier<RealVector> model;
//		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(m_data, m_nFolds);
//
//		SquaredLoss<> loss;
//		ErrorFunction<RealVector, unsigned int> errorFunction(m_data, &model, &loss);
//		PointSearch optimizer;
//		cout << "\tinit" << endl;
//		optimizer.init(errorFunction);
		Prediction pred;
//
//		for (size_t fold = 0; fold < folds.size(); ++fold) {
//			ClassificationDataset training = folds.training(fold);
//			ClassificationDataset validation = folds.validation(fold);
//
//
//			cout << __PRETTY_FUNCTION__ << endl;
//			cout << "\tstart learning" << endl;
//
//			// reduce the training set	
//			for (ClassificationDataset::batch_reference b : training.batches()) {
//				Batch<ClassificationDataset::element_type>::type batch = Batch<ClassificationDataset::element_type>::createBatch(*begin(b), b.size());
//				for (ClassificationDataset::element_reference e : b) {
//					cout << "\tlearn: " << e.input << endl;
//					optimizer.step(errorFunction);
//				}
//			}
//			
//			cout << __PRETTY_FUNCTION__ << endl;
//			cout << "\tstart evaluate" << endl;
//
////			Data<RealVector> prediction = model(validation.inputs());
//
//			cout << __PRETTY_FUNCTION__ << endl;
//			cout << "\tstart create statistics" << endl;
//
//			auto elements = validation.elements();
//			for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
//				cout <<  iter->input << " -> " <<  arg_max(model(iter->input)) << " : " <<  iter->label <<  " : " <<  endl;
//
//				// convert boost vector to std vector
//				std::vector<double> std_input;
//				for(auto x : iter->input) {
//					std_input.push_back(x);
//				}
//
//				pred.inputs.push_back(std_input);
//				pred.labels.push_back(iter->label);
//				pred.predictions.push_back(arg_max(model(iter->input)));
//			}
//		}
//		return std::move(pred);
//	}
}		/* -----  end of namespace mlta  ----- */
