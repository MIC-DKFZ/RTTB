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

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called registerTests()
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "litMultiTestsMain.h"

#include "RTToolboxConfigure.h"

namespace rttb
{
	namespace testing
	{

		const char* _callingAppPath = nullptr;

		void registerTests()
		{
            LIT_REGISTER_TEST(DoseAccInvalidParametersTest);
            LIT_REGISTER_TEST(DoseAccSimpleTest);
			LIT_REGISTER_TEST(DoseAccNeutralWeightTest);
		}

	} //namespace testing
} //namespace map


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
