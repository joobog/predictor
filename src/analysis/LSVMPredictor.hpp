/*
 * =====================================================================================
 *
 *       Filename:  LLSVMPredictor.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/20/2014 03:36:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */



#ifndef  LLSVMPredictor_INC
#define  LLSVMPredictor_INC

#include <iostream>

#include "Predictor.hpp"
#include "Helpers.hpp"

#include <shark/Data/CVDatasetTools.h>
#include <shark/Models/Kernels/GaussianRbfKernel.h>
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h>
#include <shark/Algorithms/Trainers/McSvmOVATrainer.h>
#include <shark/Algorithms/Trainers/McSvmADMTrainer.h>
#include <shark/Algorithms/Trainers/McSvmWWTrainer.h>
#include <shark/Algorithms/Trainers/McSvmATSTrainer.h> 
#include <shark/Algorithms/Trainers/McSvmMMRTrainer.h>
#include <shark/Algorithms/Trainers/McSvmCSTrainer.h>
#include <shark/Algorithms/Trainers/McSvmATMTrainer.h>
#include <shark/Algorithms/Trainers/McSvmLLWTrainer.h>

namespace mlta {
	
	/*
	 * =====================================================================================
	 *        Class:  LSVMPredictor
	 *  Description:  
	 * =====================================================================================
	 */
	class LSVMPredictor : public Predictor
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			LSVMPredictor(std::shared_ptr<shark::AbstractLinearSvmTrainer<shark::RealVector>>& trainer) :
				m_trainer{trainer}
			{
				m_name = m_trainer->name();
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
			shark::LinearClassifier<shark::RealVector> createLinearClassifierModel(shark::ClassificationDataset& data);

			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */
			std::shared_ptr<shark::AbstractLinearSvmTrainer<shark::RealVector>> m_trainer;

	}; /* -----  end of class LSVMPredictor  ----- */

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef LSVMPredictor_INC  ----- */
