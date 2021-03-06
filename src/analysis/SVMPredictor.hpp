/*
 * =====================================================================================
 *
 *       Filename:  SVMPredictor.hpp
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



#ifndef  SVMPredictor_INC
#define  SVMPredictor_INC

#include <iostream>

#include "Predictor.hpp"
#include "Helpers.hpp"

#include <shark/Data/CVDatasetTools.h>
#include <shark/Models/Kernels/GaussianRbfKernel.h>
#include <shark/Models/Kernels/LinearKernel.h>
#include <shark/Models/Kernels/PolynomialKernel.h>
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
	 *        Class:  SVMPredictor
	 *  Description:  
	 * =====================================================================================
	 */
	class SVMPredictor : public Predictor
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			SVMPredictor(std::shared_ptr<shark::AbstractSvmTrainer<shark::RealVector, unsigned int>>& trainer) :
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
			shark::KernelClassifier<shark::RealVector> createKernelClassifierModel(shark::ClassificationDataset& data);

			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */
			std::shared_ptr<shark::AbstractSvmTrainer<shark::RealVector, unsigned int>> m_trainer;

	}; /* -----  end of class SVMPredictor  ----- */

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef SVMPredictor_INC  ----- */
