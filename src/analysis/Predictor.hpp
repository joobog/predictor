
#ifndef  Predictor_INC
#define  Predictor_INC

#include "analysis/Types.hpp"
#include "analysis/CondFolds.hpp"
#include "analysis/ClassMap.hpp"

#include <string>
#include <memory>
#include <tuple>

#include <shark/Data/Csv.h>
#include <shark/Data/Dataset.h>
#include <shark/Models/AbstractModel.h>
#include <shark/Models/Trees/CARTClassifier.h>

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
			Predictor(){};

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */
			virtual void data(shark::LabeledData<shark::RealVector, shark::RealVector>& data, ClassMap& classMap) {
				m_data = &data;
				m_classMap = &classMap;
			}

			virtual std::vector<std::tuple<VerboseTrainingset, VerbosePrediction>> predictionCV(const size_t nFolds) = 0;
			virtual std::vector<std::tuple<VerboseTrainingset, VerbosePrediction>> predictionInverseCV(const size_t nFolds) = 0;
			virtual std::vector<std::tuple<VerboseTrainingset, VerbosePrediction>> predictionOnSameData() = 0;
			virtual std::vector<std::pair<VerboseTrainingset, VerbosePrediction>> predictionOfNewInput(std::vector<std::function<bool(double)>> predicates) = 0;
			virtual void modelExportPath(std::string path){m_modelExportPath = path;}
			virtual std::string name() {return m_name;}

		protected:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

			shark::LabeledData<shark::RealVector, shark::RealVector>* m_data;
			ClassMap* m_classMap;
			std::string m_name;
			std::string m_modelExportPath;
		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */
	}; /* -----  end of class Predictor  ----- */

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef Predictor_INC  ----- */
