/*
 * =====================================================================================
 *
 *       Filename:  ClassMap.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/22/2014 04:08:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "analysis/ClassMap.hpp"
namespace mlta {
	/**
	 * @brief 
	 *
	 * @param max
	 * @param relError
	 * @param relThreshold
	 */
	ClassMap::ClassMap(const double max, const double relError, const double relThreshold) {
		using namespace std;
		const double absThreshold = max * relThreshold;
		double currPos = max;
		while ( currPos > absThreshold) {
			double nextPos = currPos / (1 + relError);
			m_buckets.push_back(Bucket(nextPos, currPos));
			currPos = nextPos;
		}

		const double smallSlotSize = m_buckets[m_buckets.size()-1].limits().second - m_buckets[m_buckets.size()-1].limits().first;
		while (smallSlotSize <= currPos) {
			double nextPos = currPos - smallSlotSize;
			m_buckets.push_back(Bucket(nextPos, currPos));
			currPos = nextPos;
		}

		if (currPos != 0) {
			m_buckets.push_back(Bucket(0, currPos));
		}
		reverse(m_buckets.begin(), m_buckets.end());

		print();
	}

	/**
	 * @brief 
	 *
	 * @param value
	 *
	 * @return 
	 */
	unsigned int ClassMap::valueToClass(const double value) const
	{
		for (int i = 0; i < m_buckets.size(); ++i) {
			if (value >= m_buckets[i].limits().first && value < m_buckets[i].limits().second) {
				return i;
			}
		}
		return 0;
	}

	void ClassMap::optimizeClass(const double value)
	{
		const unsigned int c = valueToClass(value);
		m_buckets[c].optimizeBucket(value);
	}


	/**
	 * @brief 
	 */
	void ClassMap::print() {
		using namespace std;
		cout << "Buckets" << endl;
		size_t counter = 0;
		for (auto& c : m_buckets) {
			cout << counter++ << "\t " << c.limits().first << ": \t" << c.limits().second << endl;
		}
	}

	void exportCSV(ClassMap& classMap, std::string dir, const char separator) {
		using namespace std;
		std::string filename = dir + "/buckets.csv";
		std::ofstream ofs(filename);
		ofs << "bucket_num, first_limit, second_limit, value, num_of_values, values" << endl;
		for (size_t i = 0; i < classMap.m_buckets.size(); ++i) {
			const std::vector<double> values = classMap.m_buckets[i].values();
			ofs << i << separator
					<< classMap.m_buckets[i].limits().first << separator
					<< classMap.m_buckets[i].limits().second << separator
					<< classMap.m_buckets[i].value() << separator
					<< values.size();
			for (size_t j = 0; j < values.size(); ++j) {
				ofs << separator << values[j];
			}
			ofs << endl;
		}
		ofs.close();
	}
}		/* -----  end of namespace mlta  ----- */
