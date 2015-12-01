// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision $ (last changed revision)
// @date $Date $ (last change date)
// @author $Author $ (last changed by)
*/
#include <sstream>

#include "rttbScatterTester.h"

namespace rttb
{

	namespace testing
	{

		ScatterTester::ScatterTester(models::CurveDataType aReferenceCurve,
		                             models::ScatterPlotType aCompareScatter, models::BioModelParamType aVariance)
		{
			_referenceCurve = aReferenceCurve;
			_compareScatter = aCompareScatter;
			_variance = aVariance;
			_numDifference = 0;
			_allowExceptions = false;
		}

		void ScatterTester::setReferenceCurve(const models::CurveDataType aReferenceCurve)
		{
			_referenceCurve = aReferenceCurve;
		}
		void ScatterTester::setCompareScatter(const models::ScatterPlotType aCompareScatter)
		{
			_compareScatter = aCompareScatter;
		}
		void ScatterTester::setVariance(const models::BioModelParamType aVariance)
		{
			_variance = aVariance;
		}
		void ScatterTester::setAllowExceptions(const bool allow)
		{
			_allowExceptions = allow;
		};


		lit::StringType ScatterTester::getTestDescription(void) const
		{
			return "Compare a model curve and the corresponding scatter plot. The values should be comparable.";
		};

		bool ScatterTester::doCheck(void) const
		{
			_pResults->onTestStart(getCurrentTestLabel());

			const double scatterError = errorConstant * 10 + _variance;

			_numDifference = 0;
			_maxDifference = 0;
			_meanDifference = 0;

			double lower, upper, difference, oldVal;
			models::CurveDataType::const_iterator iterC = _referenceCurve.begin();
			oldVal = iterC->second;
			models::ScatterPlotType::const_iterator iter;

			for (iter = _compareScatter.begin(); iter != _compareScatter.end(); ++iter)
			{
				double currentKey = iter->first;

				//only set old value if the reference iterator iterC moves
				if (currentKey > iterC->first)
				{
					oldVal = iterC->second;

					while (currentKey > iterC->first)
					{
						++iterC;
					}
				}

				if (iterC != _referenceCurve.end())
				{
					//determine if curve is ascending or descending
					lower = oldVal;
					upper = iterC->second;

					if (upper < lower)
					{
						lower = iterC->second;
						upper = oldVal;
					}

					//check boundaries because no interpolation is done
					if (upper < (iter->second).first)
					{
						difference = abs(upper - (iter->second).first);

						if (difference < scatterError)
						{
							continue;
						}
						else if (difference > _maxDifference)
						{
							_maxDifference = difference;
						}

						_meanDifference += difference;
						++_numDifference;
					}

					if (lower > (iter->second).first)
					{
						difference = abs(upper - (iter->second).first);

						if (difference < scatterError)
						{
							continue;
						}
						else if (difference > _maxDifference)
						{
							_maxDifference = difference;
						}

						_meanDifference += difference;
						++_numDifference;
					}
				}
			}//end for(iter = _compareScatter.begin(); iter != _compareScatter.end(); ++iter)

			if (_numDifference > 0)
			{
				_meanDifference /= _numDifference;

				if (!_allowExceptions)
				{
					return false;
				}
				else
				{
					// allow up to 5% exceptions
					if (_numDifference / _compareScatter.size() <= 0.05)
					{
						return true;
					}

					return false;
				}
			}

			return true;
		}

		void ScatterTester::handleSuccess(void) const
		{
			std::ostringstream stream;

			if (!_allowExceptions)
			{
				stream << "Curve and scatter plot are similar" << std::endl;
			}
			else
			{
				stream << "Curve and scatter plot do not correspond" << std::endl;
				stream << std::endl << "Error voxel count:   " << _numDifference << std::endl;
				stream << std::endl << "Maximum difference:  " << _maxDifference << std::endl;
				stream << std::endl << "Mean difference:  " << _meanDifference << ", variance was " << _variance <<
				       std::endl;
			}

			_pResults->onTestSuccess(getCurrentTestLabel(), stream.str());
		}

		void ScatterTester::handleFailure(void) const
		{
			std::ostringstream stream;
			stream << "Curve and scatter plot do not correspond" << std::endl;
			stream << std::endl << "Error voxel count:   " << _numDifference << std::endl;
			stream << std::endl << "Maximum difference:  " << _maxDifference << std::endl;
			stream << std::endl << "Mean difference:  " << _meanDifference << ", variance was " << _variance <<
			       std::endl;

			_pResults->onTestFailure(getCurrentTestLabel(), stream.str());
		}

	}
}