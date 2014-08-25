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



#ifndef  Functions_INC
#define  Functions_INC

#include <memory>
#include <utility>
#include <tuple>

#include <shark/Data/Dataset.h>
#include <shark/Data/Csv.h>
#include <shark/Models/Trees/CARTClassifier.h>
#include <muParser.h>
#include "analysis/ClassMap.hpp"

#include "Types.hpp"

namespace mlta {
	void transformInput(shark::ClassificationDataset& data, const size_t n, std::shared_ptr<mu::Parser> func);
	void exportCSV(const std::tuple<VerboseTrainingset, VerbosePrediction>& pred, std::string fn, char separator = ',', char comment = '#');
	void print(const VerbosePrediction& data, char separator = ':');
	shark::ClassificationDataset transformToClassificationDataset(
			const shark::LabeledData<shark::RealVector, shark::RealVector>& data, 
			ClassMap& classMap);

	/// \brief Import a labeled Dataset from a csv file
	/////
	///// \param  data       Container storing the loaded data
	///// \param  fn         The file to be read from
	///// \param  lp         Position of the label in the record, either first or last column
	///// \param  separator  Optional separator between entries, typically a comma, spaces ar automatically ignored
	///// \param  comment    Trailing character indicating comment line. By dfault it is '#'
	///// \param  maximumBatchSize   Size of batches in the dataset
	void importCSV(
			shark::LabeledData<shark::RealVector, unsigned int>& data,
			std::string fn,
			shark::LabelPosition lp,
			char separator = ',',
			char comment = '#',
			std::size_t maximumBatchSize = shark::LabeledData<shark::RealVector, unsigned int>::DefaultBatchSize,
			std::size_t titleLines = 0
			);

	void importCSV(
			shark::LabeledData<shark::RealVector, shark::RealVector>& data,
			std::string fn,
			shark::LabelPosition lp,
			std::size_t numberOfOutputs,
			char separator = ',',
			char comment = '#',
			std::size_t maximumBatchSize = shark::LabeledData<shark::RealVector, shark::RealVector>::DefaultBatchSize,
			std::size_t titleLines = 0
			);
}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef Functions_INC  ----- */
