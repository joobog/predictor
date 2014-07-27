/*
 * =====================================================================================
 *
 *       Filename:  Analyser.hpp
 *
 *    Description:  j
 *
 *        Version:  1.0
 *        Created:  07/10/2014 10:35:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef  Analyser_INC
#define  Analyser_INC

#include "Types.hpp"

namespace mlta {

	/*
	 * =====================================================================================
	 *        Class:  Analyser
	 *  Description:  
	 * =====================================================================================
	 */
	class Analyser
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			Analyser(const Prediction& pred) :
				m_pred(pred) {}

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */
			int maxError();
			unsigned int maxValue();
			double relError();
			double absError();
			double rootMeanSquareDeviation();

		protected:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */
			Prediction m_pred;

	}; /* -----  end of class Analyser  ----- */


}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef Analyser_INC  ----- */
