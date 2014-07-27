/*
 * =====================================================================================
 *
 *       Filename:  Helpers.cpp
 *
 *    Description:  j
 *
 *        Version:  1.0
 *        Created:  07/10/2014 09:09:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "Helpers.hpp"

namespace mlta {
	template <>
		std::string Helpers::toString<double>(double value) {
			std::stringstream ss{};
			ss.precision(4);
			ss << std::fixed << value;
			return ss.str();
		}
}		/* -----  end of namespace mlta  ----- */
