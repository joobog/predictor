/*
 * =====================================================================================
 *
 *       Filename:  ModelInterpreter.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/23/2014 05:16:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  ModelInterpreter_INC
#define  ModelInterpreter_INC

#include <iostream>
#include <string>
#include <shark/Models/AbstractModel.h>
#include <shark/Models/Trees/CARTClassifier.h>
#include <shark/Models/Trees/RFClassifier.h>

namespace mlta {

	/*
	 * =====================================================================================
	 *        Class:  ModelInterpreter
	 *  Description:  
	 * =====================================================================================
	 */
	class ModelInterpreter
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			ModelInterpreter ();                             /* constructor      */
			ModelInterpreter ( const ModelInterpreter &other );   /* copy constructor */
			~ModelInterpreter ();                            /* destructor       */

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */

			static void exportModel(const std::string dir, const shark::CARTClassifier<shark::RealVector>* model);
			static void exportModel(const std::string dir, const shark::RFClassifier* model);


			ModelInterpreter& operator = ( const ModelInterpreter &other ); /* assignment operator */

		protected:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class ModelInterpreter  ----- */


}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef ModelInterpreter_INC  ----- */
