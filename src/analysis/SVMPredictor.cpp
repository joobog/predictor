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



namespace mlta {

	Prediction SVMPredictor::prediction() {
		using namespace std;
		using namespace shark;

//		GaussianRbfKernel<RealVector> kernel(0.5); //unconstrained?
//		KernelClassifier<RealVector> model;
//		bool offset        = true;
//		bool unconstrained = true;
//		TrainerT trainer(&kernel, 1000.0, offset, unconstrained);
		
		const AbstractKernelFunction<RealVector>* kernel = m_trainer->kernel();

		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(m_data, m_nFolds);

		Prediction data;

		for (size_t fold = 0; fold < folds.size(); ++fold) {
			ClassificationDataset training = folds.training(fold);
			ClassificationDataset validation = folds.validation(fold);
			KernelClassifier<RealVector> model;
			m_trainer->train(model, training);

			auto elements = validation.elements();
			for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
//				cout <<  iter->input << " \t-> " <<  model(iter->input) << " : " <<  iter->label  <<  endl;

//				convert boost vector to std vector
				std::vector<double> stdInput;
				for(auto x : iter->input) {
					stdInput.push_back(x);
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
}		/* -----  end of namespace mlta  ----- */


//		ZeroOneLoss<unsigned int> loss;
//		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(m_data, m_nFolds);
//		CrossValidationError<KernelClassifier<RealVector>, unsigned int> cvError(
//				folds, &trainer,&model, &trainer, &loss
//				);
//		std::cout << "SVM: Error: " << cvError(trainer.parameterVector()) << std::endl;
