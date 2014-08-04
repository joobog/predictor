/*
 * =====================================================================================
 *
 *       Filename:  SVMAnalyser.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/10/2014 07:37:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "SVMPredictor.hpp"
#include <memory>



namespace mlta {

	Prediction SVMPredictor::predictionCV() {
		using namespace std;
		using namespace shark;
		
		const AbstractKernelFunction<RealVector>* kernel = m_trainer->kernel();
		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, m_nFolds);
		Prediction data;

		for (size_t fold = 0; fold < folds.size(); ++fold) {
			ClassificationDataset training = folds.training(fold);
			ClassificationDataset validation = folds.validation(fold);
			KernelClassifier<RealVector> model;

			m_trainer->train(model, training);

			auto elements = validation.elements();
			for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
				std::vector<double> stdInput;
				for (size_t i = 0; i < iter->input.size(); ++i) {
					stdInput.push_back(iter->input[i]);
				}

				std::vector<unsigned int> stdOutput;
				stdOutput.push_back(iter->label);

				std::vector<unsigned int> stdPred;
				stdPred.push_back(model(iter->input));

				data.inputs.push_back(stdInput);
				data.outputs.push_back(stdOutput);
				data.preds.push_back(stdPred);
			}
		}
	return std::move(data);
	}

	std::vector<Prediction> SVMPredictor::predictionInverseCV() {
		using namespace std;
		using namespace shark;
	
		std::vector<Prediction> dataVector;	
		const AbstractKernelFunction<RealVector>* kernel = m_trainer->kernel();
		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, m_nFolds);
		Prediction data;

		for (size_t fold = 0; fold < folds.size(); ++fold) {
			Prediction data;
			ClassificationDataset training = folds.validation(fold);
			ClassificationDataset validation = folds.training(fold);
			KernelClassifier<RealVector> model;

			m_trainer->train(model, training);

			auto elements = validation.elements();
			for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
				std::vector<double> stdInput;
				for (size_t i = 0; i < iter->input.size(); ++i) {
					stdInput.push_back(iter->input[i]);
				}

				std::vector<unsigned int> stdOutput;
				stdOutput.push_back(iter->label);

				std::vector<unsigned int> stdPred;
				stdPred.push_back(model(iter->input));

				data.inputs.push_back(stdInput);
				data.outputs.push_back(stdOutput);
				data.preds.push_back(stdPred);
			}
			dataVector.push_back(data);
		}
		return std::move(dataVector);
	}

	Prediction SVMPredictor::predictionOnSameData() {
		using namespace std;
		using namespace shark;
	
		Prediction data;
		std::vector<Prediction> dataVector;	
		const AbstractKernelFunction<RealVector>* kernel = m_trainer->kernel();
		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, m_nFolds);

		KernelClassifier<RealVector> model;

		m_trainer->train(model, *m_data);

		auto elements = m_data->elements();
		for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
			std::vector<double> stdInput;
			for (size_t i = 0; i < iter->input.size(); ++i) {
				stdInput.push_back(iter->input[i]);
			}

			std::vector<unsigned int> stdOutput;
			stdOutput.push_back(iter->label);

			std::vector<unsigned int> stdPred;
			stdPred.push_back(model(iter->input));

			data.inputs.push_back(stdInput);
			data.outputs.push_back(stdOutput);
			data.preds.push_back(stdPred);
		}
		return std::move(data);
	}
}		/* -----  end of namespace mlta  ----- */


//		ZeroOneLoss<unsigned int> loss;
//		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(m_data, m_nFolds);
//		CrossValidationError<KernelClassifier<RealVector>, unsigned int> cvError(
//				folds, &trainer,&model, &trainer, &loss
//				);
//		std::cout << "SVM: Error: " << cvError(trainer.parameterVector()) << std::endl;
