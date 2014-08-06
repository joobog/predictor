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
			
			Prediction predictionCV() override;
			std::vector<Prediction> predictionInverseCV() override;
			Prediction predictionOnSameData() override;
			Prediction predictionOfNewInput(std::vector<std::function<bool(double)>> predicates) override;

		protected:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class ANNPredictor  ----- */

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef ANNPredictor_INC  ----- */
