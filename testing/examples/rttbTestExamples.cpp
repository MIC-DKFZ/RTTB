// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// (c) Copyright 2007, DKFZ, Heidelberg, Germany
// ALL RIGHTS RESERVED
//
// THIS FILE CONTAINS CONFIDENTIAL AND PROPRIETARY INFORMATION OF DKFZ.
// ANY DUPLICATION, MODIFICATION, DISTRIBUTION, OR
// DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY PROHIBITED
// WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF DKFZ.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author smang (last changed by)
// @author *none* (Reviewer)
// @author zhangl (Programmer)
//
// Subversion HeadURL: $HeadURL: http://sidt-hpc1/dkfz_repository/NotMeVisLab/SIDT/RTToolbox/branch/restructure/testing/core/rttbCoreTests.cpp $
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
    void registerTests()
    {
	  LIT_REGISTER_TEST(RTBioModelExampleTest);
	  LIT_REGISTER_TEST(DVHCalculatorExampleTest);
	  LIT_REGISTER_TEST(RTDVHTest);
	  LIT_REGISTER_TEST(RTDoseIndexTest);
	  LIT_REGISTER_TEST(RTDoseStatisticsTest);
	  LIT_REGISTER_TEST(RTBioModelScatterPlotExampleTest);

    }
  }
}

int main(int argc, char* argv[])
{
  int result = 0;

  rttb::testing::registerTests();

  try
  {
    result = lit::multiTestsMain(argc,argv);
  }
  catch(const std::exception& e)
  {
//    std::cerr << "RTToolbox test driver caught an exception:\n";
 //   std::cerr << e.what() << "\n";
    result = -1;
  }
  catch(...)
  {
    //std::cerr << "RTToolbox test driver caught an unknown exception!!!\n";
    result = -1;
  }

  return result;
}
