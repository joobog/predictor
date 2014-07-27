/*
 * =====================================================================================
 *
 *       Filename:  Analyser.cpp
 *
 *    Description:  jjj
 *
 *        Version:  1.0
 *        Created:  07/10/2014 10:34:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Analyser.hpp"

#include <algorithm>

namespace mlta {

	int Analyser::maxError() {

		int maxError = 0;
		int error;
		int origValue;
		int predValue;

		for (size_t i = 0; i < m_pred.outputs[0].size(); ++i) {
			origValue = m_pred.outputs[0][i];
			predValue = m_pred.preds[0][i];

			if (predValue > origValue) {
				error = predValue - origValue;
			}
			else {
				error = origValue - predValue;
			}

			if (error > maxError) {
				maxError = error;
			}
		}
		return maxError;
	}

	unsigned int Analyser::maxValue() {
		return *std::max_element(m_pred.outputs[0].begin(), m_pred.outputs[0].end());
	}	

	/**
	 * @brief Root mean square error.
	 *
	 * \f[\sigma^2 = \frac{1}{n} \cdot \sum\limits_{i=1}^{n}(x_i - x_o)^2\f]
	 * \f[ x_i : \text{prediction} \f]
	 * \f[ x_o : \text{true value} \f]
	 *
	 * @return root mean square error
	 */
	double Analyser::rootMeanSquareDeviation() {
		double sumSquaredError = 0;
		for (size_t i = 0; i < m_pred.outputs[0].size(); ++i) {
			int error = m_pred.preds[0][i] - m_pred.outputs[0][i];
			int squaredError = error * error;
			sumSquaredError += squaredError;
		}
		return sumSquaredError / m_pred.outputs[0].size();
	}

	void	statistics () {
//		using namespace shark;
//		using namespace std;
//
//		CARTTrainer trainer;
//		CARTClassifier<RealVector> model;
//		ZeroOneLoss<unsigned int, RealVector> loss;
//		CVFolds<ClassificationDataset> folds = createCVSameSizeBalanced(m_data, m_nFolds);
//
//		double result = 0;
//		double result2 = 0;
//
//		/* Statistics */
//		double tolerance = 5;
//		unsigned int max_label = Helpers::max(folds);
//		int max_error = 0;
//		double rel_error = 0;
//		double abs_error = 0;
//		unsigned int hits_counter = 0;
//		double hit_rate = 0;
//
//		unsigned int curr_max_error = 0;
//		double curr_rel_error = 0;
//		double curr_abs_error = 0;
//		unsigned int curr_hits_counter = 0;
//		double curr_hit_rate;
//
//		unsigned int error_sum = 0;
//		
//		cout << __PRETTY_FUNCTION__ <<  " - max_label: " << max_label << endl;
//		for (size_t fold = 0; fold < folds.size(); ++fold) {
//			ClassificationDataset training = folds.training(fold);
//			ClassificationDataset validation = folds.validation(fold);
//			trainer.train(model, training);
//			Data<RealVector> prediction = model(validation.inputs());
//			
//			cout << "Validation: " << endl;
//			error_sum = 0;
//			curr_rel_error = 0;
//			curr_abs_error = 0;
//			curr_max_error = 0;
//			curr_hit_rate = 0;
//			curr_hits_counter = 0;
//
//			auto elements = validation.elements();
//			result2 = 0;
//			for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
//				int error = (arg_max(model(iter->input)) - iter->label);
//				if (error > max_error) {
//					max_error = error;
//				}
//				error_sum += std::abs(error);
//				cout << arg_max(model(iter->input)) << " : " << iter->input << " -> " <<  iter->label <<  " : " << error <<  endl;
//
//				if ((arg_max(model(iter->input))*(1.0 + tolerance) >= iter->label) &&
//					 (arg_max(model(iter->input))*(1.0 - tolerance) <= iter->label))
//				{
//					++curr_hits_counter;
////					cout << arg_max((*model)(iter->input)) << " : " <<  iter->label <<  " : " << error <<  endl;
//				}
//			}
//			curr_abs_error = (double) error_sum / (double) validation.numberOfElements();
//			curr_rel_error = (double) curr_abs_error / (double) max_label; 
//			curr_hit_rate = (double)curr_hits_counter / (double) validation.numberOfElements();
//
//			cout << "absolute error: " << curr_abs_error << endl;
//			cout << "relative error: " << curr_rel_error << endl;
//
//			abs_error += curr_abs_error;
//			rel_error += curr_rel_error;
//			hit_rate += curr_hit_rate;
//
//		}
//
//		abs_error /= folds.size();
//		rel_error /= folds.size();
//		hit_rate  /= folds.size();
		
//		return Statistics{};
	}


}		/* -----  end of namespace mlta  ----- */
