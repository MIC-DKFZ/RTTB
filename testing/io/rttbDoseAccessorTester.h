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

#ifndef __DOSE_ACCESSOR_TESTER_H
#define __DOSE_ACCESSOR_TESTER_H

#include <boost/shared_ptr.hpp>

#include "litTesterBase.h"
#include "litString.h"

#include "rttbBaseType.h"
#include "../../code/core/rttbDoseAccessorInterface.h"

namespace rttb
{

	namespace testing
	{
		/*! class DoseAccessorTester
			@brief Tester class for io classes. Compares two given DoseAccessors for similarity.
			These DoseAccessors are not similar if their geometry or grid size are not similar,
			if the conversion of a given ID is invalid for one of the acessors, or if the dose
			at a given ID is not the same for both accessors.
		*/
		class DoseAccessorTester: public lit::TesterBase
		{
		public:
			typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;

		private:
			DoseAccessorPointer _referenceDose;
			DoseAccessorPointer _compareDose;

			mutable float _maxDifference;
			mutable float _numDifference;

			mutable bool _geometryIsSimilar;
			mutable bool _sameGridSize;

			mutable bool _conversionFailed;
			mutable VoxelGridID _failedID;

		public:
			DoseAccessorTester(DoseAccessorPointer aReferenceDose, DoseAccessorPointer aCompareDose);

			/*! Set the dose accessor pointer for the dose comparison.
			*/
			void setReferenceDose(const DoseAccessorPointer aReferenceDose);
			void setCompareDose(const DoseAccessorPointer aCompareDose);

			/*! Returns a string that specifies the test the tester currently performs.
			*/
			lit::StringType getTestDescription(void) const;
			lit::StringType getTestName(void) const
			{
				return "DoseAccessorTester";
			};

		protected:
			/*! performs the test and checks the results.
				@result Indicates if the test was successful (true) or if it failed (false)
			*/
			bool doCheck(void) const;

			/*! Function will be called by check() if test was succesfull.
			*/
			void handleSuccess(void) const;

			/*! Function will be called by check() if test was a failure.
			*/
			void handleFailure(void) const;

		};

	}
}

#endif