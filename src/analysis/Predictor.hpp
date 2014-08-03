
#ifndef  Predictor_INC
#define  Predictor_INC

#include <Types.hpp>

#include <string>
#include <memory>

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
			Predictor() : m_nFolds{5}{};

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */
			virtual void data(shark::ClassificationDataset* data) {m_data = data;}
			virtual Prediction predictionCV() = 0;
			virtual std::vector<Prediction> predictionInverseCV() = 0;
			virtual Prediction predictionOnSameData() = 0;
			virtual std::string name() {return m_name;}

		protected:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

			shark::ClassificationDataset* m_data;
			std::string m_name;
			unsigned int m_nFolds;
		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */
	}; /* -----  end of class Predictor  ----- */

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef Predictor_INC  ----- */
