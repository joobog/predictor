/*
 * =====================================================================================
 *
 *       Filename:  SameSizeFolds.hpp
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


#ifndef  SameSizeFolds_INC
#define  SameSizeFolds_INC

#include <shark/Data/Dataset.h>

#include <functional>
#include <vector>

namespace mlta {

	/*
	 * =====================================================================================
	 *        Class:  SameSizeFolds
	 *  Description:  
	 * =====================================================================================
	 */
	class SameSizeFolds
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			SameSizeFolds (shark::LabeledData<shark::RealVector, shark::RealVector> const& data, const size_t nFolds);

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */

			shark::LabeledData<shark::RealVector, shark::RealVector> training(const size_t fold);
			shark::LabeledData<shark::RealVector, shark::RealVector> validation(const size_t fold);
			size_t size() {return m_foldIndices.size();}


		protected:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */
			shark::LabeledData<shark::RealVector, shark::RealVector> m_data;
			std::vector<std::vector<size_t>> m_foldIndices;
	}; /* -----  end of class SameSizeFolds  ----- */
}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef SameSizeFolds_INC  ----- */

