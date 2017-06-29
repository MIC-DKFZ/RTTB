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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbMaskVoxel.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace testing
	{

		/*! @brief MaskVoxelTest - test the API of MaskVoxel
			1) test constructors (values as expected?)
			2) test set/getRelevantVolumeFraction
			4) test operators "=="
		*/

		int MaskVoxelTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			//1) test constructors (values as expected?)
			//MaskVoxel(const VoxelGridID& aVoxelGridID);
			VoxelGridID anID = 5;
			const FractionType defaultFraction = 1;
			CHECK_NO_THROW(core::MaskVoxel MaskVoxel(anID));
			CHECK_THROW_EXPLICIT(core::MaskVoxel MaskVoxel(-anID), core::InvalidParameterException);
			core::MaskVoxel aMaskVoxel2(anID);
			CHECK_EQUAL(anID, aMaskVoxel2.getVoxelGridID());
			CHECK_EQUAL(defaultFraction, aMaskVoxel2.getRelevantVolumeFraction());

			//MaskVoxel(const VoxelGridID& aVoxelGridID, FractionType aVolumeFraction)
			anID = 15;
			FractionType aFraction = 0.73;
			CHECK_NO_THROW(core::MaskVoxel MaskVoxel(anID, aFraction));
			CHECK_THROW_EXPLICIT(core::MaskVoxel MaskVoxel(-anID, aFraction), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(core::MaskVoxel MaskVoxel(anID, -aFraction), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(core::MaskVoxel MaskVoxel(-anID, -aFraction), core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(core::MaskVoxel MaskVoxel(anID, aFraction + 2),
			                     core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(core::MaskVoxel MaskVoxel(-anID, aFraction + 2),
			                     core::InvalidParameterException);
			core::MaskVoxel aMaskVoxel3(anID, aFraction);
			CHECK_EQUAL(anID, aMaskVoxel3.getVoxelGridID());
			CHECK_EQUAL(aFraction, aMaskVoxel3.getRelevantVolumeFraction());

			//2) test set/getRelevantVolumeFraction
			aFraction = 0.42;
			anID = aMaskVoxel3.getVoxelGridID();
			CHECK_NO_THROW(aMaskVoxel3.setRelevantVolumeFraction(aFraction));
			CHECK_THROW_EXPLICIT(aMaskVoxel3.setRelevantVolumeFraction(-aFraction),
			                     core::InvalidParameterException);
			CHECK_THROW_EXPLICIT(aMaskVoxel3.setRelevantVolumeFraction(aFraction + 2),
			                     core::InvalidParameterException);
			aMaskVoxel3.setRelevantVolumeFraction(aFraction);
			CHECK_EQUAL(anID, aMaskVoxel3.getVoxelGridID());
			CHECK_EQUAL(aFraction, aMaskVoxel3.getRelevantVolumeFraction());

			//4) test operators "=="
			CHECK(!(aMaskVoxel2 == aMaskVoxel3)); //not equal
			core::MaskVoxel aMaskVoxel4(aMaskVoxel3.getVoxelGridID());
			CHECK(!(aMaskVoxel4 == aMaskVoxel3)); //equal ID, but unequal volume fraction -> not equal
			aMaskVoxel4.setRelevantVolumeFraction(aMaskVoxel3.getRelevantVolumeFraction());
			CHECK_EQUAL(aMaskVoxel4, aMaskVoxel3); //equal
			aMaskVoxel2.setRelevantVolumeFraction(aMaskVoxel3.getRelevantVolumeFraction());
			CHECK(!(aMaskVoxel2 == aMaskVoxel3)); //no equal ID -> not equal

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//end namespace testing
}//end namespace rttb

