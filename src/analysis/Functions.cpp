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
#include <fstream>
#include <iostream>

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



	/**
	 * @brief Export prediction to csv file
	 *
	 * @param data
	 * @param fn
	 * @param separator
	 * @param comment
	 */
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
				for (size_t j = 0; j < data[k].preds[0].size()-1; ++j) {
					line << data[k].preds[i][j] << separator;
				}
				line << data[k].preds[i][data[k].preds[0].size()-1];
			}
			output_file << line.str() << endl;
		}

		output_file.close();
	}


	/**
	 * @brief CSV import function, based on shark conventions
	 *
	 * @param data output
	 * @param fn filename
	 * @param lp position of the label column
	 * @param separator 
	 * @param comment
	 * @param maximumBatchSize
	 * @param titleLines number of lines reserver for the title
	 */
	void importCSV(
			shark::LabeledData<shark::RealVector, unsigned int>& data,
			std::string fn,
			shark::LabelPosition lp,
			char separator,
			char comment,
			std::size_t maximumBatchSize,
			std::size_t titleLines
			){
		using namespace shark;
		std::ifstream stream(fn.c_str());
		stream.unsetf(std::ios::skipws);

		for(std::size_t i=0; i < titleLines; ++i) // ignoring the first lines
			stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::istream_iterator<char> streamBegin(stream);
		std::string contents(//read contents of file in string
				streamBegin,
				std::istream_iterator<char>()
				);
		//call the actual parser
		csvStringToData(data,contents,lp,separator,comment,maximumBatchSize);
	}



	/**
	 * @brief Print prediction
	 *
	 * @param data
	 * @param separator
	 */
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
