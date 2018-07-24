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

#ifndef __MASK_VL_TESTER_H
#define __MASK_VL_TESTER_H

#include <boost/shared_ptr.hpp>

#include "litTesterBase.h"
#include "litString.h"

#include "rttbBaseType.h"
#include "../../code/core/rttbMaskAccessorInterface.h"

namespace rttb
{

	namespace testing
	{
		/*! class MaskVoxelListTester
			@brief Tester class for lists. Compares two given lists for similarity.
			Both lists need to have the same length and have equal values in each element to be considered similar.
		*/
		class MaskVoxelListTester: public lit::TesterBase
		{

		private:
			typedef core::MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;
			typedef core::MaskAccessorInterface::MaskVoxelList MaskVoxelList;

			MaskVoxelListPointer _referenceList;
			MaskVoxelListPointer _compareList;

			mutable double _maxDifference;
			mutable double _numDifference;

			mutable bool _sameSize;
			mutable bool _masVoxelsDiffer;

			mutable VoxelGridID _failedListIndex;

		public:
			MaskVoxelListTester(MaskVoxelListPointer aReferenceList, MaskVoxelListPointer aCompareList);

			/*! Set the dose accessor pointer for the dose comparison.
			*/
			void setReferenceList(const MaskVoxelListPointer aReferenceList);
			void setCompareList(const MaskVoxelListPointer aCompareList);

			/*! Returns a string that specifies the test the tester currently performs.
			*/
			lit::StringType getTestDescription(void) const;
			lit::StringType getTestName(void) const
			{
				return "MaskVoxelListTester";
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