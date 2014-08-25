/*
 * =====================================================================================
 *
 *       Filename:  ANNPredictor.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/18/2014 05:37:00 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  ANNPredictor_INC
#define  ANNPredictor_INC

#include "analysis/Predictor.hpp"

namespace mlta {

	/*
	 * =====================================================================================
	 *        Class:  ANNPredictor
	 *  Description:  
	 * =====================================================================================
	 */
	class ANNPredictor : public Predictor
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			ANNPredictor() {
				m_name = "ANN";
			}
			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */
			
			std::pair<TrainingSet, Prediction> predictionCV(const size_t nFolds) override;
			std::vector<std::pair<TrainingSet, Prediction>> predictionInverseCV(const size_t nFolds) override;
			std::pair<TrainingSet, Prediction> predictionOnSameData() override;
			std::pair<TrainingSet, Prediction> predictionOfNewInput(std::vector<std::function<bool(double)>> predicates) override;

		protected:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class ANNPredictor  ----- */

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef ANNPredictor_INC  ----- */
