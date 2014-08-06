/*
 * =====================================================================================
 *
 *       Filename:  CondFolds.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/05/2014 12:24:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  CondFolds_INC
#define  CondFolds_INC

#include <shark/Data/Dataset.h>
#include <muParser.h>

#include <functional>
#include <vector>

namespace mlta {

	/*
	 * =====================================================================================
	 *        Class:  CondFolds
	 *  Description:  
	 * =====================================================================================
	 */
	class CondFolds
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			CondFolds (shark::ClassificationDataset const& data, std::vector<std::function<bool(double)>> predicates);

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */

			shark::ClassificationDataset training();
			shark::ClassificationDataset validation();


		protected:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */
			std::vector<std::function<bool(double)>> m_predicates;
			shark::ClassificationDataset m_data;
			shark::ClassificationDataset m_training;
			shark::ClassificationDataset m_validation;
	}; /* -----  end of class CondFolds  ----- */
}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef CondFolds_INC  ----- */

