/*
 * =====================================================================================
 *
 *       Filename:  ModelInterpreter.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/23/2014 05:20:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "analysis/ModelInterpreter.hpp"
#include <shark/Models/Converter.h>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/archive/polymorphic_text_oarchive.hpp>
#include <boost/archive/polymorphic_text_iarchive.hpp>
#include "boost/filesystem.hpp"

namespace mlta {
	void ModelInterpreter::exportModel(const std::string dir, const shark::CARTClassifier<shark::RealVector>* model) {
		using namespace std;
		using namespace shark;
		const string rawModelFn = dir + "/model_raw.txt";
		const string interModelFn = dir + "/model_interpretation.txt";

		boost::filesystem::path outpath(dir.c_str());
		if (boost::filesystem::create_directories(outpath)) {
			cout << "directory created: " << dir.c_str() << endl;
		}
		else {
			cout << __PRETTY_FUNCTION__ << " couldn't create directory: " << dir << endl;
		}
		{
			ofstream ofs{rawModelFn};
			boost::archive::polymorphic_text_oarchive out(ofs, ofstream::out);
			model->save(out, 1);
			ofs.close();
		}

		ifstream ifs(rawModelFn);
		ofstream ofs(interModelFn);
		boost::archive::polymorphic_text_iarchive in(ifs);

		CARTClassifier<RealVector>::SplitMatrixType matrix;
		in >> matrix;

		shark::ArgMaxConverter<CARTClassifier<RealVector>> converter;
		converter.decisionFunction() = *model;

		ofs << "CARTClassifier" << endl;
		for (CARTClassifier<RealVector>::SplitMatrixType::iterator itr = matrix.begin(); itr != matrix.end(); itr++){
			ofs << "ID:"           << itr->nodeId
				<< " attribute: "  << (itr->attributeIndex == 12297829382473034411llu ? 100 : itr->attributeIndex)
				<< " value: "      << itr->attributeValue
				<< " left node: "  << itr->leftNodeId
				<< " right node: " << itr->rightNodeId
				<< " label: "      << blas::arg_max(itr->label)
				<< " orig_label: " << itr->label
				<< endl;
		}
		ofs.close();
	}
	void ModelInterpreter::exportModel(const std::string dir, const shark::RFClassifier* model) {
		using namespace std;
		using namespace shark;
		const string rawModelFn = dir + "/model_raw.txt";
		const string interModelFn = dir + "/model_interpretation.txt";

		boost::filesystem::path outpath(dir.c_str());
		if (boost::filesystem::create_directories(outpath)) {
			cout << "directory created: " << dir.c_str() << endl;
		}
		else {
			cout << __PRETTY_FUNCTION__ << " couldn't create directory: " << dir << endl;
		}
		{
			ofstream ofs{rawModelFn};
			boost::archive::polymorphic_text_oarchive out(ofs, ofstream::out);
			model->save(out, 1);
			ofs.close();
		}

//		ifstream ifs(rawModelFn);
//		ofstream ofs(interModelFn);
//		boost::archive::polymorphic_text_iarchive in(ifs);
//
//		RFClassifier::SplitMatrixType matrix;
//		in >> matrix;
//
//		shark::ArgMaxConverter<RFClassifier> converter;
//		converter.decisionFunction() = *model;
//
//		ofs << "RFClassifier" << endl;
//		for (RFClassifier::SplitMatrixType::iterator itr = matrix.begin(); itr != matrix.end(); itr++){
//			ofs << "ID:"           << itr->nodeId
//				<< " attribute: "  << (itr->attributeIndex == 12297829382473034411llu ? 100 : itr->attributeIndex)
//				<< " value: "      << itr->attributeValue
//				<< " left node: "  << itr->leftNodeId
//				<< " right node: " << itr->rightNodeId
//				<< " label: "      << blas::arg_max(itr->label)
//				<< " orig_label: " << itr->label
//				<< endl;
//		}
//		ofs.close();
	}
}		/* -----  end of namespace mlta  ----- */
