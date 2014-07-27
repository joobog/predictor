/*
 * =====================================================================================
 *
 *       Filename:  Predictor.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/09/2014 09:55:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <shark/Data/Csv.h>
#include <shark/Data/Dataset.h>

#include "analysis/Predictor.hpp"

namespace mlta {
	void Predictor::read(const std::string filename) {
		try {
			shark::importCSV(m_data, filename, shark::LAST_COLUMN, ';');
		}
		catch (shark::Exception e) {
			std::cout << "Unable to open file " << filename <<  ". Check paths!" << std::endl;
			std::cout << e.what()               << std::endl;
			exit(EXIT_FAILURE);
		}
		catch (...) {
			std::cerr << "Unable to read data from file " << filename << std::endl;
			exit(EXIT_FAILURE);
		}
	}
}		/* -----  end of namespace mlta  ----- */
