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

	/**
	 * @brief 
	 *
	 * @param data
	 *
	 * @return 
	 */
	shark::KernelClassifier<shark::RealVector>
	SVMPredictor::createKernelClassifierModel(shark::ClassificationDataset& data) {
			using namespace shark;
			KernelClassifier<RealVector> model;
			m_trainer->train(model, data);
			return model;
		}



	/**
	 * @brief 
	 *
	 * @return 
	 */
	Prediction 
	SVMPredictor::predictionCV(const size_t nFolds) {
		using namespace std;
		using namespace shark;
		
		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, nFolds);
		Prediction data;

		for (size_t fold = 0; fold < folds.size(); ++fold) {
			ClassificationDataset training = folds.training(fold);
			ClassificationDataset validation = folds.validation(fold);

			auto model = createKernelClassifierModel(training);

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
	SVMPredictor::predictionInverseCV(const size_t nFolds) {
		using namespace std;
		using namespace shark;
	
		std::vector<Prediction> dataVector;	
		const AbstractKernelFunction<RealVector>* kernel = m_trainer->kernel();
		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, nFolds);
		Prediction data;

		for (size_t fold = 0; fold < folds.size(); ++fold) {
			Prediction data;
			ClassificationDataset training = folds.validation(fold);
			ClassificationDataset validation = folds.training(fold);
			KernelClassifier<RealVector> model;

			m_trainer->train(model, training);

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
	SVMPredictor::predictionOnSameData() {
		using namespace std;
		using namespace shark;
	
		Prediction data;
		std::vector<Prediction> dataVector;	
		const AbstractKernelFunction<RealVector>* kernel = m_trainer->kernel();

		KernelClassifier<RealVector> model;
		m_trainer->train(model, *m_data);

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
	SVMPredictor::predictionOfNewInput(std::vector<std::function<bool(double)>> predicates) {
		using namespace std;
		using namespace shark;
	
		Prediction data;
		CondFolds condFolds{*m_data, predicates};
		ClassificationDataset training = condFolds.training();
		ClassificationDataset validation = condFolds.validation();

		std::vector<Prediction> dataVector;	
		const AbstractKernelFunction<RealVector>* kernel = m_trainer->kernel();

		KernelClassifier<RealVector> model;

		m_trainer->train(model, training);

		auto elements = validation.elements();
		for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
			append(data, iter->input, iter->label, model(iter->input));
		}
		return std::move(data);
	}
}		/* -----  end of namespace mlta  ----- */
