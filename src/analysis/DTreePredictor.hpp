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

			Prediction predictionCV() override;
			std::vector<Prediction> predictionInverseCV() override {
				std::vector<Prediction> pList{};
				return std::move(pList);
			}
			Prediction predictionOnSameData() override {
				Prediction p{};
				return std::move(p);
			}

		protected:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class DTreeAnalyser  ----- */

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef DTreeAnalyser_INC  ----- */
