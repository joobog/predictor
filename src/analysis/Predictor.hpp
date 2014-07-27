
#ifndef  Predictor_INC
#define  Predictor_INC

#include <Types.hpp>

#include <string>

#include <shark/Data/Csv.h>
#include <shark/Data/Dataset.h>

namespace mlta {

	
	/*
	 * =====================================================================================
	 *        Class:  Predictor
	 *  Description:  
	 * =====================================================================================
	 */
	class Predictor
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */
			virtual void read(const std::string filename);

			virtual Prediction prediction() = 0;
			virtual std::string name() {return m_name;}

		protected:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

			shark::ClassificationDataset m_data;
			std::string m_name;
			unsigned int m_nFolds = 5;
		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */
	}; /* -----  end of class Predictor  ----- */

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef Predictor_INC  ----- */
