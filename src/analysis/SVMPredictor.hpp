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
			std::shared_ptr<shark::AbstractSvmTrainer<shark::RealVector, unsigned int>> m_trainer;

	}; /* -----  end of class SVMPredictor  ----- */

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef SVMPredictor_INC  ----- */
