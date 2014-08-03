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

#include "Types.hpp"


#ifndef  Functions_INC
#define  Functions_INC

namespace mlta {
	void transformInput(shark::ClassificationDataset& data, const size_t n, std::shared_ptr<mu::Parser> func);
	void exportCSV(const std::vector<Prediction>& pred, std::string fn, char separator = ',', char comment = '#');
	void print(const Prediction& data, char separator = ':');
}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef Functions_INC  ----- */
