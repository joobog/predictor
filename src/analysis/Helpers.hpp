/*
 * =====================================================================================
 *
 *       Filename:  Helpers.hpp
 *
 *    Description:  A set of static universal often used functions.
 *
 *        Version:  1.0
 *        Created:  07/10/2014 07:09:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef  Helpers_INC
#define  Helpers_INC

#include <shark/Data/Dataset.h>
#include <shark/Data/CVDatasetTools.h>

namespace mlta {
	/*
	 * =====================================================================================
	 *        Class:  Helpers
	 *  Description:  
	 * =====================================================================================
	 */
	class Helpers
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */

			template <typename T>
				static std::string toString(T value) {
					std::stringstream ss{};
					ss << value;
					return ss.str();
				}

			static size_t arg_max(shark::RealVector v) {
				size_t index = 0;
				for (size_t i = 0; i < v.size(); ++i) {
					if (v[i] > v[index]) {
						index = i;
					}
				}
				return index;
			}

			static unsigned int max(const shark::CVFolds<shark::ClassificationDataset>&  folds) {
				using namespace shark;

				ClassificationDataset data{folds.dataset()};

				unsigned int max = 0;

				for (auto e : data.elements()) {
					if (e.label > max) {
						max = e.label;
					}
				}

				return max;
			}

			template <typename T>
				static void printData(const T& data) {
					using namespace std;
					using namespace shark;
					for (auto batch : data.batches()) {
						for (std::size_t i = 0; i < boost::size(batch); ++i) {
							cout << shark::get(batch, i) << endl;
						}
					}
				}
		protected:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  METHODS       ======================================= */

			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class Helpers  ----- */
}		/* -----  end of namespace mlta  ----- */
#endif   /* ----- #ifndef Helpers_INC  ----- */
