// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html [^]
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author $Author$ (last changed by)
*/

#ifndef __SCATTER_TESTER_H
#define __SCATTER_TESTER_H


#include "litTesterBase.h"
#include "litString.h"

#include "rttbBaseType.h"
#include "rttbBioModelScatterPlots.h"
#include "rttbBioModelCurve.h"

namespace rttb
{

	namespace testing
	{
		/*! class ScatterTester
			@brief Tester class for for model scatter plots. Compares a given scatter plot with a
			given model curve. The values should be similar for similar doses. Variations should
			only depend on the variance given for the scatter plot calculation. An additional
			deviation of 1e-4+givenVariance is ignored.
		*/
		class ScatterTester: public lit::TesterBase
		{

		private:
			models::CurveDataType _referenceCurve;
			models::ScatterPlotType _compareScatter;

			/*! Additional variance that is allowed in the comparison. This value usually corresponds to the value used in
				the generation of the scatter plot.
			*/
			models::BioModelParamType _variance;

			mutable float _maxDifference;
			mutable int   _numDifference;
			mutable float _meanDifference;

			/*! If true allows up to 5% of the scatter points to deviate without failing.
				If false, all points have to correspond.
			*/
			bool _allowExceptions;


		public:
			ScatterTester(models::CurveDataType aReferenceCurve, models::ScatterPlotType aCompareScatter,
			              models::BioModelParamType aVariance = 0);

			/*! Set the reference curve used in comparison.
			*/
			void setReferenceCurve(const models::CurveDataType aReferenceCurve);
			/*! Set the scatter data used in comparison.
			*/
			void setCompareScatter(const models::ScatterPlotType aCompareScatter);
			/*! Set the variance that is allowed for the scatter plot. Usually this matches the parameter used in the computation of
				the scattered values.
			*/
			void setVariance(const models::BioModelParamType aVariance);
			/*! If true allows up tp 5% of the scatter points to deviate without failing.
				If false, all points have to correspond.
			*/
			void setAllowExceptions(const bool allow);

			/*! Returns a string that specifies the test the tester currently performs.
			*/
			lit::StringType getTestDescription(void) const;
			lit::StringType getTestName(void) const
			{
				return "ScatterTester";
			};

		protected:
			/*! performes the test and checks the results.
				@result Indicates if the test was successfull (true) or if it failed (false)
			*/
			bool doCheck(void) const;

			/*! Function will be called be check() if test was succesfull.
			*/
			void handleSuccess(void) const;

			/*! Function will be called be check() if test was a failure.
			*/
			void handleFailure(void) const;
		};

	}
}

#endif