/*
 * =====================================================================================
 *
 *       Filename:  RFAnalyser.hpp
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



#ifndef  RFAnalyser_INC
#define  RFAnalyser_INC

#include <tuple>
#include <shark/Models/Trees/RFClassifier.h>
#include <shark/Models/Converter.h>
#include "analysis/Predictor.hpp"

namespace mlta {

	/*
	 * =====================================================================================
	 *        Class:  RFAnalyser
	 *  Description:  
	 * =====================================================================================
	 */
	class RFPredictor : public Predictor
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
				RFPredictor() {
					m_name = "RF";
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
				shark::ArgMaxConverter<shark::RFClassifier> createRFModel(shark::ClassificationDataset& data, const unsigned int fold);
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class RFAnalyser  ----- */

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef RFAnalyser_INC  ----- */
