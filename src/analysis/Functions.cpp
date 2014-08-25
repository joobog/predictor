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
#include <string>

#include "analysis/Functions.hpp"
#include "analysis/ModelInterpreter.hpp"
#include "boost/filesystem.hpp"


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


	shark::ClassificationDataset transformToClassificationDataset(
			const shark::LabeledData<shark::RealVector, shark::RealVector>& data, 
			ClassMap& classMap)
	{
		using namespace shark;
		std::vector<shark::RealVector> inputs;
		std::vector<unsigned int> labels;
		auto elements = data.elements();
		for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
			inputs.push_back(iter->input);
			labels.push_back(classMap.valueToClass(iter->label[0]));
			classMap.optimizeClass(iter->label[0]);
		}
		shark::ClassificationDataset result = createLabeledDataFromRange(inputs, labels); 
		return std::move(result);
	}

	/**
	 * @brief Export prediction to csv file
	 *
	 * @param data
	 * @param fn
	 * @param separator
	 * @param comment
	 */
	void exportCSV(const std::tuple<VerboseTrainingset, VerbosePrediction>& data, std::string dir, char separator, char comment) {
		using namespace std;

		cout << __PRETTY_FUNCTION__ << endl;
		cout << "\toutput dir: " << dir << endl;
		fstream output_file;
		fstream trainingset_file;

//		const shark::CARTClassifier<shark::RealVector>& model = get<0>(data);
		const VerbosePrediction& prediction = get<1>(data);
		const VerboseTrainingset& trainingset = get<0>(data);

		boost::filesystem::path outpath(dir.c_str());
		if (boost::filesystem::create_directories(outpath)) {
			cout << "directory created: " << dir.c_str() << endl;
		}
		else {
			cout << __PRETTY_FUNCTION__ << " couldn't create directory: " << dir << endl;
		}

		output_file.open(dir+"/output.csv", ios_base::out);
		trainingset_file.open(dir+"/trainingset.csv", ios_base::out);
//		ModelInterpreter::exportModel(dir, &model);


		for (size_t i = 0; i < trainingset.inputValues.size(); ++i) {
			stringstream line{};
			line.precision(10);
			for (size_t j = 0; j < trainingset.inputValues[i].size(); ++j) {
				line << setw(0) << trainingset.inputValues[i][j] << separator;
			}
			for (size_t j = 0; j < trainingset.outputValues[i].size(); ++j) {
				line << trainingset.outputValues[i][j] << separator;
			}

			for (size_t j = 0; j < trainingset.outputClasses[i].size(); ++j) {
				line << trainingset.outputClasses[i][j] << separator;
			}

			for (size_t j = 0; j < trainingset.outputBuckets[i].size()-1; ++j) {
				line << trainingset.outputBuckets[i][j].limits().first << separator;
				line << trainingset.outputBuckets[i][j].limits().second << separator;
			}

			line << trainingset.outputBuckets[i][trainingset.outputBuckets[i].size()-1].limits().first << separator;
			line << trainingset.outputBuckets[i][trainingset.outputBuckets[i].size()-1].limits().second;
			trainingset_file << line.str() << endl;
		}
		//		assert(data.size() != 0);
		//		assert(data.inputs.size() != 0);
		//		assert(data.inputs[0].size() != 0);
		//		assert(data.outputs.size() != 0);
		//		assert(data.outputs[0].size() != 0);
		//		assert(data.inputs.size() == data.outputs.size());
		//		assert(data.outputs.size() == data.outputs.size());
		//		assert(data.preds.size() == data.outputs.size());

		for (size_t i = 0; i < prediction.inputValues.size(); ++i) {
			stringstream line{};
			line.precision(10);
			for (size_t j = 0; j < prediction.inputValues[i].size(); ++j) {
				line << setw(0) << prediction.inputValues[i][j] << separator;
			}
			for (size_t j = 0; j < prediction.outputValues[i].size(); ++j) {
				line << prediction.outputValues[i][j] << separator;
			}

			for (size_t j = 0; j < prediction.outputClasses[i].size(); ++j) {
				line << prediction.outputClasses[i][j] << separator;
			}

			for (size_t j = 0; j < prediction.outputBuckets[i].size(); ++j) {
				line << prediction.outputBuckets[i][j].limits().first << separator;
				line << prediction.outputBuckets[i][j].limits().second << separator;
			}
			for (size_t j = 0; j < prediction.predValues[i].size(); ++j) {
				line << prediction.predValues[i][j] << separator;
			}

			for (size_t j = 0; j < prediction.predClasses[i].size(); ++j) {
				line << prediction.predClasses[i][j] << separator;
			}

			for (size_t j = 0; j < prediction.predBuckets[i].size()-1; ++j) {
				line << prediction.predBuckets[i][j].limits().first << separator;
				line << prediction.predBuckets[i][j].limits().second << separator;
			}

			line << prediction.predBuckets[i][prediction.predBuckets[i].size()-1].limits().first << separator;
			line << prediction.predBuckets[i][prediction.predBuckets[i].size()-1].limits().second;
			output_file << line.str() << endl;
		}

		output_file.close();
		trainingset_file.close();
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
	 * @brief 
	 *
	 * @param data
	 * @param fn
	 * @param lp
	 * @param numberOfOutputs
	 * @param separator
	 * @param comment
	 * @param maximumBatchSize
	 * @param 
	 */
	void importCSV(
			shark::LabeledData<shark::RealVector, shark::RealVector>& data,
			std::string fn,
			shark::LabelPosition lp,
			std::size_t numberOfOutputs,
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
		csvStringToData(data,contents,lp, numberOfOutputs, separator,comment,maximumBatchSize);
	}
		

	/**
	 * @brief Print prediction
	 *
	 * @param data
	 * @param separator
	 */
	void print(const VerbosePrediction& data, char separator) {
		using namespace std;

		for (size_t i = 0; i < data.inputValues.size(); ++i) {
			stringstream line{};
//			line.setf(ios_base::dec, ios_base::basefield);
//			line.setf(ios_base::fixed, ios_base::floatfield);
//			line.unsetf(ios_base::scientific);
			line.precision(10);
			for (size_t j = 0; j < data.inputValues[0].size(); ++j) {
				line  << setw(0) << data.inputValues[i][j] << separator;
			}
			for (size_t j = 0; j < data.outputValues[0].size(); ++j) {
				line << data.outputValues[i][j] << separator;
			}
			for (size_t j = 0; j < data.predValues[0].size() - 1; ++j) {
				line << data.predValues[i][j] << separator;
			}
			line << data.predValues[i][data.predValues[0].size() - 1];
			cout << line.str() << endl;
		}
	}
}		/* -----  end of namespace mlta  ----- */
