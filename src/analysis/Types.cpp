/*
 * =====================================================================================
 *
 *       Filename:  Types.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/11/2014 12:17:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <sstream>
#include <fstream>
#include <cassert>
#include <iomanip>

#include <Types.hpp>

namespace mlta {
	void exportCSV(const Prediction& data, std::string fn, char separator, char comment) {
		using namespace std;

		cout << __PRETTY_FUNCTION__ << endl;
		cout << "\tfilename: " << fn << endl;
		fstream output_file;

		output_file.open(fn, ios_base::out);

		assert(data.inputs.size() != 0);
		assert(data.inputs[0].size() != 0);
		assert(data.outputs.size() != 0);
		assert(data.outputs[0].size() != 0);
		assert(data.inputs.size() == data.outputs.size());
		assert(data.outputs.size() == data.outputs.size());
		assert(data.preds.size() == data.outputs.size());
		
		for (size_t i = 0; i < data.inputs.size(); ++i) {
			stringstream line{};
//			line.setf(ios_base::dec, ios_base::basefield);
//			line.setf(ios_base::fixed, ios_base::floatfield);
//			line.unsetf(ios_base::scientific);
			line.precision(10);
			for (size_t j = 0; j < data.inputs[0].size(); ++j) {
				line  << setw(0) << data.inputs[i][j] << separator;
			}
			for (size_t j = 0; j < data.outputs[0].size(); ++j) {
				line << data.outputs[i][j] << separator;
			}
			for (size_t j = 0; j < data.preds[0].size() - 1; ++j) {
				line << data.preds[i][j] << separator;
			}
			line << data.preds[i][data.preds[0].size() - 1];
			output_file << line.str() << endl;
		}

		output_file.close();
	}

	void exportCSV(const std::vector<Prediction>& data, std::string fn, char separator, char comment) {
		using namespace std;

		cout << __PRETTY_FUNCTION__ << endl;
		cout << "\tfilename: " << fn << endl;
		fstream output_file;

		output_file.open(fn, ios_base::out);

//		assert(data.size() != 0);
//		assert(data.inputs.size() != 0);
//		assert(data.inputs[0].size() != 0);
//		assert(data.outputs.size() != 0);
//		assert(data.outputs[0].size() != 0);
//		assert(data.inputs.size() == data.outputs.size());
//		assert(data.outputs.size() == data.outputs.size());
//		assert(data.preds.size() == data.outputs.size());
		
		for (size_t i = 0; i < data[0].inputs.size(); ++i) {
			stringstream line{};
			line.precision(10);
			for (size_t j = 0; j < data[0].inputs[0].size(); ++j) {
				line  << setw(0) << data[0].inputs[i][j] << separator;
			}
			for (size_t j = 0; j < data[0].outputs[0].size(); ++j) {
				line << data[0].outputs[i][j] << separator;
			}
			for (size_t k = 0; k < data.size(); ++k) {
				for (size_t j = 0; j < data[k].preds[0].size(); ++j) {
					line << data[k].preds[i][j] << separator;
				}
			}
			output_file << line.str() << endl;
		}

		output_file.close();
	}

	void print(const Prediction& data, char separator) {
		using namespace std;

		for (size_t i = 0; i < data.inputs.size(); ++i) {
			stringstream line{};
//			line.setf(ios_base::dec, ios_base::basefield);
//			line.setf(ios_base::fixed, ios_base::floatfield);
//			line.unsetf(ios_base::scientific);
			line.precision(10);
			for (size_t j = 0; j < data.inputs[0].size(); ++j) {
				line  << setw(0) << data.inputs[i][j] << separator;
			}
			for (size_t j = 0; j < data.outputs[0].size(); ++j) {
				line << data.outputs[i][j] << separator;
			}
			for (size_t j = 0; j < data.preds[0].size() - 1; ++j) {
				line << data.preds[i][j] << separator;
			}
			line << data.preds[i][data.preds[0].size() - 1];
			cout << line.str() << endl;
		}
	}


}		/* -----  end of namespace mlta  ----- */
