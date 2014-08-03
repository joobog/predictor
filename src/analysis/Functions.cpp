/*
 * =====================================================================================
 *
 *       Filename:  Functions.cpp
 *
 *    Description: 
 *
 *        Version:  1.0
 *        Created:  08/03/2014 12:55:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <cassert>
#include "analysis/Functions.hpp"


namespace mlta {
	/**
	 * @brief Apply a function to the values in a column.
	 *
	 * @param data non-empty dataset
	 * @param n column number
	 * @param func function applied to each value in the column n
	 */
	void transformInput(shark::ClassificationDataset& data, const size_t n, std::shared_ptr<mu::Parser> func) {
		assert(data.numberOfElements() != 0);
		assert(shark::inputDimension(data) != 0);
		assert(n + 1 <= shark::inputDimension(data));

		mu::varmap_type x = func->GetVar();

		auto elements = data.elements();
		for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
			if (x.count("x")) {
				double* var = x["x"];
				*var = (iter->input)[n];
				(iter->input)[n] = func->Eval();
			}
		}
	}
}		/* -----  end of namespace mlta  ----- */
