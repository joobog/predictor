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

#include <Types.hpp>

namespace mlta {

	template <typename T>
	std::vector<T> toStdVector(const shark::blas::vector<T>& v) {
		std::vector<T> stdVector;
		for (size_t i = 0; i < v.size(); ++i) {
			stdVector.push_back(v[i]);
		}
		return std::move(stdVector);
	}

	std::vector<unsigned int> toClassVector(const shark::blas::vector<double>& v, const ClassMap& classMap) {
		std::vector<unsigned int> stdVector;
		for (size_t i = 0; i < v.size(); ++i) {
			stdVector.push_back(classMap.valueToClass(v[i]));
		}
		return std::move(stdVector);
	}

	std::vector<Bucket> toBucketVector(const shark::blas::vector<double>& v, const ClassMap& classMap) {
		std::vector<Bucket> stdVector;
		for (size_t i = 0; i < v.size(); ++i) {
			Bucket bucket = classMap.valueToBucket(v[i]);
			stdVector.push_back(classMap.valueToBucket(v[i]));
		}
		return std::move(stdVector);
	}

	/**
	 * @brief 
	 *
	 * @param data
	 * @param input
	 * @param label
	 * @param prediction
	 * @param classMap
	 */
	void append(
			VerbosePrediction& data, 
			const shark::blas::vector<double>& input,
			const shark::blas::vector<double>& label,
			const unsigned int prediction,
			const ClassMap& classMap)
	{
		// input vector
		data.inputValues.push_back(toStdVector<double>(input));

		// real output
		data.outputValues.push_back(toStdVector<double>(label));
		data.outputClasses.push_back(toClassVector(label, classMap));
		data.outputBuckets.push_back(toBucketVector(label, classMap));

		// prediction
		std::vector<double> tmpPredValueVector;
		tmpPredValueVector.push_back(classMap.classToValue(prediction));
		data.predValues.push_back(tmpPredValueVector);

		std::vector<unsigned int> tmpPredClassVector;
		tmpPredClassVector.push_back(prediction);
		data.predClasses.push_back(tmpPredClassVector);

		std::vector<Bucket> tmpPredBucketVector;
		tmpPredBucketVector.push_back(classMap.classToBucket(prediction));
		data.predBuckets.push_back(tmpPredBucketVector);
	}

	VerboseTrainingset makeVerboseTrainingset(
			const shark::LabeledData<shark::RealVector, shark::RealVector>& dataset,
			const ClassMap& classMap) {
		VerboseTrainingset data;
		// input vector

		auto elements = dataset.elements();
		
		for (auto iter = elements.begin(); iter != elements.end(); ++iter) {
			data.inputValues.push_back(toStdVector<double>(iter->input));

			// real output
			data.outputValues.push_back(toStdVector<double>(iter->label));
			data.outputClasses.push_back(toClassVector(iter->label, classMap));
			data.outputBuckets.push_back(toBucketVector(iter->label, classMap));
		}
		return std::move(data);
	}
}		/* -----  end of namespace mlta  ----- */
