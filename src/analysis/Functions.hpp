/*
 * =====================================================================================
 *
 *       Filename:  Functions.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/03/2014 01:00:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <memory>
#include <shark/Data/Dataset.h>
#include <muParser.h>


#ifndef  Functions_INC
#define  Functions_INC

namespace mlta {
	void transformInput(shark::ClassificationDataset& data, const size_t n, std::shared_ptr<mu::Parser> func);
}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef Functions_INC  ----- */
