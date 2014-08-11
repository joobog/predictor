#include <string>
#include <memory>
#include <stdio.h>

#include <muParser.h>

#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"

#include "analysis/DTreePredictor.hpp"
#include "analysis/ANNPredictor.hpp"
#include "analysis/SVMPredictor.hpp"
#include "analysis/Analyser.hpp"
#include "analysis/Functions.hpp"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */

namespace {
	const size_t ERROR_IN_COMMAND_LINE = 1;
	const size_t SUCCESS = 0;
	const size_t ERROR_UNHANDLED_EXCEPTION = 2;
	const size_t ERROR_UNKNOWN_ARGUMENT = 3;
	const size_t ERROR_INPUTFILE_REQUIRED = 4;
}

enum class PredictionType {
	CV,  // CrossValidation
	ICV, // InverseCrossValidation
	SD,  // SameData
	IP,  // Interpolation
};

std::string filenameGenerator(
		const std::string& inputFilename, 
		const std::vector<std::shared_ptr<mlta::Predictor>>& predictorList,
		const std::vector<std::string>& functions,
		PredictionType pType
		) 
{
	namespace bfs = boost::filesystem;

	std::string filename{};
	bfs::path p{inputFilename.c_str()};
	filename = p.parent_path().string() + "/" + p.stem().string();

	for (const auto& predictor : predictorList) {
		filename += "-" + predictor->name();
	}	

	switch (pType) {
		case PredictionType::CV:
			filename += "-cv";
			break;
		case PredictionType::ICV:
			filename += "-icv";
			break;
		case PredictionType::SD:
			filename += "-sd";
			break;
		case PredictionType::IP:
			filename += "-ip";
			break;
		default:
			filename +="-unknown";
			std::cout << "Warning: Unknown prediction type" << std::endl;
			break;	
	}

	if (!functions.empty()) {
		filename += "-functions";
	}

	filename += "-output.csv";
	return filename;
}

std::string appendFoldNum(std::string& filename, unsigned int foldNum) {
	namespace bfs = boost::filesystem;
	std::string newFilename{};
	bfs::path p{filename.c_str()};
	newFilename = p.parent_path().string() + "/" + p.stem().string() +  "-" + std::to_string(foldNum) + ".csv";
	return std::move(newFilename);
}

int main ( int argc, char *argv[] )
{
	using namespace std;
	using namespace shark;
	using namespace mlta;

	namespace bpo = boost::program_options;
	namespace bfs = boost::filesystem;

	std::vector<std::shared_ptr<Predictor>> predictorList;
	string inputFilename;
	string outputFilename;
	string statFilename;
	bool separateFiles;
	std::vector<std::string> rawFunctions;
	std::vector<std::shared_ptr<mu::Parser>> functions;
	std::vector<std::string> rawPredicates;
	std::vector<std::function<bool(double)>> predicates;
	PredictionType predictionType;
	size_t nFolds = 0;

	// nasty but a working solution
	// kernel must exist until the prediction is done
	std::shared_ptr<shark::AbstractKernelFunction<shark::RealVector>> kernel;
	// ***

	try {
		bpo::options_description desc("Allowed options");
		desc.add_options()
			("help"              , "produce help message")
			("alg"               , bpo::value<std::vector<std::string>>()->multitoken() , "learning algorithm (dtree ann svm)")
			("svm-trainer"       , bpo::value<std::vector<std::string>>()->multitoken() , "ova adm ww ats mmr cs atm llw")
			("svm-offset"        , bpo::value<bool>()->default_value(false)             , "svm trainer offset")
			("svm-unconstrained" , bpo::value<bool>()->default_value(false)             , "svm trainer unconstrained")
			("svm-regulation"    , bpo::value<double>()->default_value(1000.0)          , "svm trainer regulation parameter")
			("svm-gamma"         , bpo::value<double>()->default_value(0.5)             , "svm kernel gamma parameter")
			("csv-output-file"   , bpo::value<string>()                                 , "csv output file")
			("csv-input-file"    , bpo::value<string>()                                 , "csv input file")
			("prediction-type"   , bpo::value<string>()->default_value("cv")            , "CrossValidation (cv),, InverseCrossValidation(icv) , SameData(sd) , Interpolation(ip)")
			("number-of-folds" , bpo::value<size_t>()->default_value(5), "number of folds")
			("transform"         , bpo::value<std::vector<std::string>>()->multitoken() , "list of function (see documentation of fparser)")
			("predicates"        , bpo::value<std::vector<std::string>>()->multitoken() , "list of conditions (for interpolatino)");

// TODO
//			("statistics"        , bpo::value<string>()                         , "statistics output file")
//			("alg-output-config" , bpo::value<string>()                         , "algorithm output configuration file")
//			("alg-input-config"  , bpo::value<string>()                         , "algorithm input configuration file");

		bpo::variables_map vm;
		try {
			bpo::store(bpo::parse_command_line(argc, argv, desc), vm);

			if (vm.count("help")) {
				cout << "Parameters" << endl;
				cout << desc << endl;
				return SUCCESS;
			}
		
			if (vm.count("number-of-folds")) {
				nFolds=vm["number-of-folds"].as<size_t>();
			}

			/*
			 * List of learning algorithms
			 */	
			if (vm.count("alg")) {
				const std::vector<std::string> algoritmList = vm["alg"].as<std::vector<std::string>>();
				double gamma{vm["svm-gamma"].as<double>()};										// kernel bandwidth parameter
				kernel = std::make_shared<shark::GaussianRbfKernel<>>(gamma);

				for (auto& algorithm : algoritmList) {
					if (algorithm == "dtree") { 
						predictorList.push_back(make_shared<DTreePredictor>());
					}
					else if (algorithm == "ann") {
						predictorList.push_back(make_shared<ANNPredictor>());
					}
					else if (algorithm == "svm") {
						double c{vm["svm-regulation"].as<double>()};									// regulation parameter
						bool offset{vm["svm-offset"].as<bool>()};											// use bias/offset parameter
						bool unconstrained{vm["svm-unconstrained"].as<bool>()};

						std::shared_ptr<AbstractSvmTrainer<RealVector, unsigned int>> trainer;

						std::vector<std::string> svmTrainerList{vm["svm-trainer"].as<vector<string>>()}; 
						for (auto& svmTrainer : svmTrainerList) {
							if (svmTrainer == "ova") {
								trainer =	std::make_shared<McSvmOVATrainer<RealVector>>(kernel.get(), c, offset, unconstrained);
								predictorList.push_back(make_shared<SVMPredictor>(trainer));
							}
							else if (svmTrainer == "adm") {
								trainer =	std::make_shared<McSvmADMTrainer<RealVector>>(kernel.get(), c, unconstrained);
								predictorList.push_back(make_shared<SVMPredictor>(trainer));
							}
							else if (svmTrainer == "ww") {
								trainer =	std::make_shared<McSvmWWTrainer<RealVector>>(kernel.get(), c, offset, unconstrained);
								predictorList.push_back(make_shared<SVMPredictor>(trainer));
							}
							else if (svmTrainer == "ats") {
								trainer =	std::make_shared<McSvmATSTrainer<RealVector>>(kernel.get(), c, unconstrained);
								predictorList.push_back(make_shared<SVMPredictor>(trainer));
							}
							else if (svmTrainer == "mmr") {
								trainer =	std::make_shared<McSvmMMRTrainer<RealVector>>(kernel.get(), c, offset, unconstrained);
								predictorList.push_back(make_shared<SVMPredictor>(trainer));
							}
							else if (svmTrainer == "cs") {
								trainer = std::make_shared<McSvmCSTrainer<RealVector>>(kernel.get(), c, offset, unconstrained);
								predictorList.push_back(make_shared<SVMPredictor>(trainer));
							}
							else if (svmTrainer == "atm") {
								trainer = std::make_shared<McSvmATMTrainer<RealVector>>(kernel.get(), c, unconstrained);
								predictorList.push_back(make_shared<SVMPredictor>(trainer));
							}
							else if (svmTrainer == "llw") {
								trainer = std::make_shared<McSvmLLWTrainer<RealVector>>(kernel.get(), c, unconstrained);
								predictorList.push_back(make_shared<SVMPredictor>(trainer));
							}
						}
					}
					else {
						cerr << "Unknown algorithm " << algorithm << " in the --algorithm option" << endl;
						return ERROR_UNKNOWN_ARGUMENT;
					}
				}
			}


			if (vm.count("transform")) {
				rawFunctions = vm["transform"].as<std::vector<string>>();
				for (const std::string rawFunction : rawFunctions) {
					if (rawFunction == "-") {
						functions.push_back(nullptr);
					}
					else {
						std::shared_ptr<mu::Parser> function = std::make_shared<mu::Parser>();;
						double x = 0;
						function->DefineVar("x",  &x);
						function->SetExpr(rawFunction);
						functions.push_back(function);
					}
				}
				
			}


			if (vm.count("predicates")) {
				rawPredicates = vm["predicates"].as<std::vector<string>>();
				for (const std::string rawPredicate : rawPredicates) {
					if (rawPredicate == "-") {
						predicates.push_back(nullptr);
					}
					else {
						std::shared_ptr<mu::Parser> predicateParser = std::make_shared<mu::Parser>();;
						double x = 0;
						predicateParser->DefineVar("x",  &x);
						predicateParser->SetExpr(rawPredicate);
						auto predicate = [predicateParser](double value)->bool
						{
							mu::varmap_type vars = predicateParser->GetVar();
							if (vars.count("x")) {
								double* xVar = vars["x"]; 
								*xVar = value;
								return predicateParser->Eval();
							}
							else {
								return false;
							}
						};
						predicates.push_back(predicate);
					}
				}
				
			}

			/*
			 * Input and output filenames
			 */
			if (vm.count("csv-input-file")) {
				inputFilename = vm["csv-input-file"].as<string>();
				bfs::path p{inputFilename.c_str()};
				try {
					if (!bfs::exists(p)) {
						cout << "ERROR: " << inputFilename << " does not exists" << endl;
						return 1;
					}
					else if (bfs::is_directory(p)) {
						cout << "ERROR: " << inputFilename << " is a directory" << endl;
						return 1;
					}
					else if (!bfs::is_regular_file(p)) {
						cout << "ERROR: " << inputFilename << " is not a regular file" << endl;
						return 1;
					}
				}
				catch (bfs::filesystem_error e) {
					cerr << "ERROR: " <<  e.what() << endl;
					return 1;
				}
				cout << "Inputfile: " << inputFilename << endl;
			}
			else {
				cerr << "Inputfile required" << endl;
				cout << desc << endl;
				return ERROR_INPUTFILE_REQUIRED;
			}


			/* 
			 * Prediction Type
			 */
			if (vm.count("prediction-type")) {
				std::string rawPredictionType = vm["prediction-type"].as<std::string>();
				if (rawPredictionType == "cv") {
					predictionType = PredictionType::CV;
				}
				else if (rawPredictionType == "icv") {
					predictionType = PredictionType::ICV;
				}
				else if (rawPredictionType == "sd") {
					predictionType = PredictionType::SD;
				}
				else if (rawPredictionType == "ip") {
					predictionType = PredictionType::IP;
				}
				else {
					std::cout << "ERROR: unknown prediciton type: " << rawPredictionType << endl;
					exit(1);
				}

			}

			/*
			 * Filename generator
			 */
			if (vm.count("csv-output-file")) {
				outputFilename = vm["csv-output-file"].as<string>();
			}

//			if (vm.count("statistics")) {
//				statFilename = vm["statistics"].as<string>();
//				cout << "Statitstics: " << statFilename << endl;
//			}
			

			bpo::notify(vm);
		}
		catch(bpo::error& e) {
			cerr << "ERROR: " << e.what() << endl << endl;
//			cerr << desc << endl;
			return ERROR_IN_COMMAND_LINE;
		}
	}
	catch(exception& e) {
		cerr << "Unhandled exception reached the top of main: " << e.what() 
			<< ", application will now exit" << endl;
		return ERROR_UNHANDLED_EXCEPTION;		
	}


	// Make predictions and write them to files
	std::vector<Prediction> predictionList;
	shark::ClassificationDataset data;

	try {
		importCSV(data, inputFilename, shark::LAST_COLUMN, ',', '#', ClassificationDataset::DefaultBatchSize, 1);
		
	}
	catch (shark::Exception e) {
		std::cout << "Unable to open file " << inputFilename <<  ". Check paths!" << std::endl;
		std::cout << e.what()               << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (...) {
		std::cerr << "Unable to read data from file " << inputFilename << std::endl;
		exit(EXIT_FAILURE);
	}

	for ( size_t i = 0; i < functions.size(); ++i ) {
		if (functions[i] != nullptr) {
			transformInput(data, i, functions[i]);
		}
	}

	switch (predictionType) {
		case PredictionType::CV:
			for (auto& predictor : predictorList) {
				predictor->data(&data);
				Prediction prediction = predictor->predictionCV(nFolds);
				predictionList.push_back(prediction);
			}
			if (outputFilename.empty()) {
				outputFilename = filenameGenerator(inputFilename, predictorList, rawFunctions, PredictionType::CV);
			}
			exportCSV(predictionList, outputFilename, ',');
			std::cout << "Output is written to " << outputFilename << endl;
			break;

		case PredictionType::ICV:
			for (auto& predictor : predictorList) {
				predictor->data(&data);
				std::vector<Prediction> predictions = predictor->predictionInverseCV(nFolds);
				if (outputFilename.empty()) {
				outputFilename = filenameGenerator(inputFilename, predictorList, rawFunctions, PredictionType::ICV);
				}
				for (size_t i = 0; i < predictions.size(); ++i) {
					std::vector<Prediction> predictionList{};
					predictionList.push_back(predictions[i]);
					exportCSV(predictionList, appendFoldNum(outputFilename, i), ',');
					std::cout << "Output is written to " << outputFilename << endl;
				}
			}
			break;

		case PredictionType::SD:
			for (auto& predictor : predictorList) {
				predictor->data(&data);
				Prediction prediction = predictor->predictionOnSameData();
				predictionList.push_back(prediction);
			}
			if (outputFilename.empty()) {
				outputFilename = filenameGenerator(inputFilename, predictorList, rawFunctions, PredictionType::SD);
			}
			exportCSV(predictionList, outputFilename, ',');
			std::cout << "Output is written to " << outputFilename << endl;
			break;

		case PredictionType::IP:
			cout << "Start prediction of new Data" << endl;
			for (auto& predictor : predictorList) {
				predictor->data(&data);
				Prediction prediction = predictor->predictionOfNewInput(predicates);
				predictionList.push_back(prediction);
			}
			if (outputFilename.empty()) {
				outputFilename = filenameGenerator(inputFilename, predictorList, rawFunctions, PredictionType::IP);
			}
			exportCSV(predictionList, outputFilename, ',');
			std::cout << "Output is written to " << outputFilename << endl;
			break;

		default:
			break;	
	}
	

	return 0;
}				/* ----------  end of function main  ---------- */
