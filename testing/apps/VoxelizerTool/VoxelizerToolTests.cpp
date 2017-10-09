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
// @version $Revision: 1127 $ (last changed revision)
// @date    $Date: 2015-10-01 13:33:33 +0200 (Do, 01 Okt 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "litMultiTestsMain.h"

namespace rttb
{
	namespace testing
	{
		const char* _callingAppPath = nullptr;
		void registerTests()
		{
			LIT_REGISTER_TEST(VoxelizerToolDifferentCommandsTest);
			LIT_REGISTER_TEST(VoxelizerToolVoxelValue);
			LIT_REGISTER_TEST(VoxelizerToolIncorrectCommandsTest);
			LIT_REGISTER_TEST(VoxelizerToolVoxelizerAllStructsTest);
		}
	}
}

int main(int argc, char* argv[])
{
	int result = 0;

	rttb::testing::registerTests();

	if (argc > 0)
	{
		rttb::testing::_callingAppPath = argv[0];
	}

	try
	{
		result = lit::multiTestsMain(argc, argv);
	}

	catch (...)
	{
		result = -1;
	}

	return result;
}
