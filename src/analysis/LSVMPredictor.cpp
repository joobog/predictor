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

#include "LSVMPredictor.hpp"
#include <memory>



namespace mlta {

	/**
	 * @brief 
	 *
	 * @param data
	 *
	 * @return 
	 */
	shark::LinearClassifier<shark::RealVector>
	LSVMPredictor::createLinearClassifierModel(shark::ClassificationDataset& data) {
			using namespace shark;
//			LinearClassifier<RealVector> model;
			LinearClassifier<RealVector> model;
			std::cout << "start " << m_trainer->name() << std::endl;
			m_trainer->train(model, data);
			return model;
		}



	/**
	 * @brief 
	 *
	 * @return 
	 */
	std::pair<TrainingSet, Prediction> 
	LSVMPredictor::predictionCV(const size_t nFolds) {
		using namespace std;
		using namespace shark;
		cout << "create folds" << endl;
		
		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, nFolds);
		std::pair<TrainingSet, Prediction> data;

		cout << "start main loop" << endl;

		for (size_t fold = 0; fold < folds.size(); ++fold) {
			ClassificationDataset training = folds.training(fold);
			ClassificationDataset validation = folds.validation(fold);
			cout << "learn from training fold " << fold << endl;

			auto model = createLinearClassifierModel(training);

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
	std::vector<std::pair<TrainingSet, Prediction>> 
	LSVMPredictor::predictionInverseCV(const size_t nFolds) {
		using namespace std;
		using namespace shark;
	
		std::vector<std::pair<TrainingSet, Prediction>> dataVector;	
		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(*m_data, nFolds);
		std::pair<TrainingSet, Prediction> data;

		for (size_t fold = 0; fold < folds.size(); ++fold) {
			std::pair<TrainingSet, Prediction> data;
			ClassificationDataset training = folds.validation(fold);
			ClassificationDataset validation = folds.training(fold);
			LinearClassifier<RealVector> model;

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
	std::pair<TrainingSet, Prediction> 
	LSVMPredictor::predictionOnSameData() {
		using namespace std;
		using namespace shark;
	
		std::pair<TrainingSet, Prediction> data;
		std::vector<std::pair<TrainingSet, Prediction>> dataVector;	

		LinearClassifier<RealVector> model;
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
	std::pair<TrainingSet, Prediction> 
	LSVMPredictor::predictionOfNewInput(std::vector<std::function<bool(double)>> predicates) {
		using namespace std;
		using namespace shark;
	
		std::pair<TrainingSet, Prediction> data;
		CondFolds condFolds{*m_data, predicates};
		ClassificationDataset training = condFolds.training();
		ClassificationDataset validation = condFolds.validation();

		std::vector<std::pair<TrainingSet, Prediction>> dataVector;	

		LinearClassifier<RealVector> model;

		m_trainer->train(model, training);

		auto elements = validation.elements();
		for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
			append(data, iter->input, iter->label, model(iter->input));
		}
		return std::move(data);
	}
}		/* -----  end of namespace mlta  ----- */
