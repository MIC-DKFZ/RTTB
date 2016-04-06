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
// @version $Revision: 1221 $ (last changed revision)
// @date    $Date: 2015-12-01 13:43:31 +0100 (Di, 01 Dez 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called registerTests()
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "litMultiTestsMain.h"

namespace rttb
{
	namespace testing
	{

		const char* _callingAppPath = NULL;

		void registerTests()
		{
			LIT_REGISTER_TEST(DoseToolBasicUsageTest);
			LIT_REGISTER_TEST(DoseToolInvalidParametersTest);
			LIT_REGISTER_TEST(DoseToolVirtuosDoseTest);
			LIT_REGISTER_TEST(DoseToolDicomDoseTest);
			LIT_REGISTER_TEST(DoseToolITKDoseTest);
			LIT_REGISTER_TEST(DoseToolRegexTest);
			LIT_REGISTER_TEST(DoseToolDVHTest);
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
