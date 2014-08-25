/*
 * =====================================================================================
 *
 *       Filename:  DTreeAnalyser.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/09/2014 10:07:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */



#ifndef  DTreeAnalyser_INC
#define  DTreeAnalyser_INC

#include <tuple>
#include <shark/Models/Trees/CARTClassifier.h>
#include <shark/Models/Converter.h>
#include "analysis/Predictor.hpp"

namespace mlta {

	/*
	 * =====================================================================================
	 *        Class:  DTreeAnalyser
	 *  Description:  
	 * =====================================================================================
	 */
	class DTreePredictor : public Predictor
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
				DTreePredictor() {
					m_name = "DTree";
				}
			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */

				std::vector<std::tuple<VerboseTrainingset, VerbosePrediction>> predictionCV(const size_t nFolds);
				std::vector<std::tuple<VerboseTrainingset, VerbosePrediction>> predictionInverseCV(const size_t nFolds);
				std::vector<std::tuple<VerboseTrainingset, VerbosePrediction>> predictionOnSameData();
				std::vector<std::pair<VerboseTrainingset, VerbosePrediction>> predictionOfNewInput(std::vector<std::function<bool(double)>> predicates) override;

		protected:
			/* ====================  METHODS       ======================================= */
				std::tuple<VerboseTrainingset, VerbosePrediction> run(const shark::RegressionDataset& originTraining, const shark::RegressionDataset& originValidation, const unsigned int fold);
				shark::ArgMaxConverter<shark::CARTClassifier<shark::RealVector>> createDTreeModel(shark::ClassificationDataset& data, const unsigned int fold);
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class DTreeAnalyser  ----- */

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef DTreeAnalyser_INC  ----- */
