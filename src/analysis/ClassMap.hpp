/*
 * =====================================================================================
 *
 *       Filename:  ClassMap.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/22/2014 03:55:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  ClassMap_INC
#define  ClassMap_INC

#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <fstream>


namespace mlta {

	class ClassMap;
	void exportCSV(ClassMap& classMap, std::string dir, const char separator=',');

	class Bucket {
		public:
			Bucket(const double first, const double second) {m_limits.first = first; m_limits.second = second;}
			double value() const {
				if (m_values.size() != 0){
					return m_value;
				}
				else {
					return m_limits.first + (m_limits.second - m_limits.first) / 2;
				}
			}

			std::pair<double, double> limits() const {return m_limits;}

			void limits(std::pair<double, double> limits) {m_limits = limits;}
			std::vector<double> values() {return m_values;}

			void optimizeBucket(const double value) {
				m_values.push_back(value);
				m_value = (m_value * (m_values.size() - 1)  + value) / m_values.size();
			}

		private:
			std::pair<double, double> m_limits;
			std::vector<double> m_values;	
			double m_value;
	};
	
	class ClassMap {
		public:
			ClassMap() {
				m_buckets.push_back(Bucket(0., 0.));		
				print();
			}

			ClassMap(std::vector<Bucket> buckets) {
				m_buckets = buckets;
				print();
			}

			ClassMap(const double max, const double relError, const double relThreshold);
			double classToValue(const unsigned int c) const {return m_buckets[c].value();}
			Bucket classToBucket(unsigned int c) const {return m_buckets[c];}
			Bucket valueToBucket(double v) const {return m_buckets[valueToClass(v)];}
			unsigned int valueToClass(const double value) const;
			void optimizeClass(const double value);
			void print();

			friend void exportCSV(ClassMap& classMap, std::string dir, const char separator);

		private:
			std::vector<Bucket> m_buckets;
	};

}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef ClassMap_INC  ----- */
