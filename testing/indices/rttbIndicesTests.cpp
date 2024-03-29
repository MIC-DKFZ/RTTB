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
// and all it expects is that you have a function called RegisterTests

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "litMultiTestsMain.h"

namespace rttb
{
	namespace testing
	{

		void registerTests()
		{
			LIT_REGISTER_TEST(ConformationNumberTest);
			LIT_REGISTER_TEST(ConformalIndexTest);
			LIT_REGISTER_TEST(ConformityIndexTest);
			LIT_REGISTER_TEST(CoverageIndexTest);
			LIT_REGISTER_TEST(HomogeneityIndexTest);
			LIT_REGISTER_TEST(GammaIndexTest);
		}
	}
}

int main(int argc, char* argv[])
{
	int result = 0;

	rttb::testing::registerTests();

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
