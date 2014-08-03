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
	std::vector<std::shared_ptr<mu::Parser>> functions;

	// nasty but a working solution
	// kernel must exist until the prediction is done
	std::shared_ptr<shark::AbstractKernelFunction<shark::RealVector>> kernel;
	// ***

	try {
		bpo::options_description desc("Allowed options");
		desc.add_options()
			("help"              , "produce help message")
			("alg"               , bpo::value<std::vector<std::string>>()->multitoken() , "learning algorithm (dtree ann svm)")
			("svm-trainer"       , bpo::value<std::vector<std::string>>()->multitoken()    , "ova adm ww ats mmr cs atm llw")
			("svm-offset"        , bpo::value<bool>()->default_value(false)     , "svm trainer offset")
			("svm-unconstrained" , bpo::value<bool>()->default_value(false)     , "svm trainer unconstrained")
			("svm-regulation"    , bpo::value<double>()->default_value(1000.0)  , "svm trainer regulation parameter")
			("svm-gamma"         , bpo::value<double>()->default_value(0.5)     , "svm kernel gamma parameter")
			("csv-output-file"   , bpo::value<string>()                         , "csv output file")
			("csv-input-file"    , bpo::value<string>()                         , "csv input file")
			("transform", bpo::value<std::vector<std::string>>()->multitoken(), "list of function (see documentation of fparser)")
			("separate-files"		, bpo::value<bool>()->default_value(true), 	"save results in separate files");
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
			
			// create a list of learning algorithms
			
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
				const std::vector<std::string> rawFunctions = vm["transform"].as<std::vector<string>>();
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

			// Input and output filenames
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

			if (vm.count("csv-output-file")) {
				outputFilename = vm["csv-output-file"].as<string>();

			}
			else {
				bfs::path p{inputFilename.c_str()};
				outputFilename = p.parent_path().string() + "/" + p.stem().string() + "-" + "mix" + "-output.csv";
			}

//			if (vm.count("statistics")) {
//				statFilename = vm["statistics"].as<string>();
//				cout << "Statitstics: " << statFilename << endl;
//			}
			

			separateFiles = vm["separate-files"].as<bool>();

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
		shark::importCSV(data, inputFilename, shark::LAST_COLUMN, ';');
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

	for (auto& predictor : predictorList) {
		predictor->data(&data);
		Prediction prediction = predictor->prediction();

//		print(prediction);
		if (!separateFiles) {
			predictionList.push_back(prediction);
		}
		else {
			cout << "Use a separate file for each prediction" << endl;
			bfs::path p{inputFilename.c_str()};
			outputFilename = p.parent_path().string() + "/" + p.stem().string() + "-" + predictor->name() + "-output.csv";
			cout << "Outputfile: " << outputFilename << endl;
			exportCSV(prediction, outputFilename, ';');

//			Analyser analyser(prediction);
		}
	}

	if (!separateFiles) {
		cout << "Mixed outputfile: " << outputFilename << endl;
		exportCSV(predictionList, outputFilename, ';');
	}
	
	return 0;
}				/* ----------  end of function main  ---------- */
