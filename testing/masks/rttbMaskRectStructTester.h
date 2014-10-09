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

#ifndef __MASK_RECT_TESTER_H
#define __MASK_RECT_TESTER_H

#include <boost/shared_ptr.hpp>

#include "litTesterBase.h"
#include "litString.h"

#include "rttbBaseType.h"
#include "../../code/core/rttbMaskAccessorInterface.h"

namespace rttb
{

	namespace testing
	{
		/*! class MaskRectStructTester
			@brief Tests if masked voxel are all inside the given boundaries
			The boundaries are defined in DummyStructure::CreateRectangularStructureCentered
			@see DummyStructure
		*/
		class MaskRectStructTester: public lit::TesterBase
		{

		private:
			typedef core::MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;
			typedef core::MaskAccessorInterface::MaskVoxelList MaskVoxelList;
			typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;

			MaskVoxelListPointer _referenceList;
			MaskAccessorPointer _maskAccessor;

			GridIndexType _zIndex;

			mutable bool _conversionFailed;
			mutable bool _differentSlice;
			mutable bool _outsideStructure;
			mutable bool _wrongRVF;

			mutable VoxelGridID _failedListIndex;
			mutable float _wrongVal;

		public:
			MaskRectStructTester(MaskAccessorPointer aMaskAccessor, GridIndexType z);

			/*! Set the mask accessor pointer for the dose comparison.
			*/
			void setAccessor(const MaskAccessorPointer aMaskAccessor);

			/*! Set index of slice containing the dummy structure.
			*/
			void setZ(const GridIndexType z);

			/*! Returns a string that specifies the test the tester currently performs.
			*/
			lit::StringType getTestDescription(void) const;
			lit::StringType getTestName(void) const
			{
				return "MaskRectStructTester";
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