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
// @author zhangl (last changed by)
// @author *none* (Reviewer)
// @author zhangl (Programmer)
//
// Subversion HeadURL: $HeadURL: http://sidt-hpc1/dkfz_repository/NotMeVisLab/SIDT/RTToolbox/trunk/testing/core/DVHCalculatorTest.cpp $
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbVirtuosDoseAccessor.h"
#include "rttbVirtuosFileStructureSetGenerator.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"
#include "rttbIndexOutOfBoundsException.h"


namespace rttb{

  namespace testing{

	/*!@brief VirtuosStructureSetGeneratorTest - test structure set generator for virtuos data
			1) test constructor 
			2) test generateStructureSet
	*/
   
	int VirtuosStructureSetGeneratorTest(int argc, char* argv[] )
	{
	  typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;
	  
	  PREPARE_DEFAULT_TEST_REPORTING;
	  //ARGUMENTS: 1: structure file name	   ".../testing/data/Virtuos/NHH030000.vdx" 
	  //           2: ctx  file name (virtuos) ".../testing/data/Virtuos/NHH030000.ctx.gz"
	  

	  std::string RTSTRUCT_FILENAME;
	  std::string CT_FILENAME;
	 
	  if (argc>1)
	  {
		RTSTRUCT_FILENAME = argv[1];
	  }
	  if (argc>2)
	  {
		CT_FILENAME = argv[2];
	  }
	 
	 

	  /* structure set */
	  //1) check constructor
	  CHECK_THROW_EXPLICIT(io::virtuos::VirtuosFileStructureSetGenerator("aStructureFileName.vdx",CT_FILENAME.c_str()), core::InvalidParameterException);
	  CHECK_THROW_EXPLICIT(io::virtuos::VirtuosFileStructureSetGenerator(RTSTRUCT_FILENAME.c_str(),"aCTFileName.ctx"), core::InvalidParameterException);
	  CHECK_NO_THROW(io::virtuos::VirtuosFileStructureSetGenerator(RTSTRUCT_FILENAME.c_str(),CT_FILENAME.c_str()));

	  //2) check generateStructureSet
	  CHECK_THROW_EXPLICIT(io::virtuos::VirtuosFileStructureSetGenerator("aStructureFileName.vdx",CT_FILENAME.c_str()).generateStructureSet(), core::InvalidParameterException);
	  CHECK_THROW_EXPLICIT(io::virtuos::VirtuosFileStructureSetGenerator(RTSTRUCT_FILENAME.c_str(),"aCTFileName.ctx").generateStructureSet(), core::InvalidParameterException);
	  std::string testFileName = RTSTRUCT_FILENAME;
	  int gzPosition = testFileName.find(".vdx");
	  if(gzPosition != std::string::npos){
		  testFileName.erase(gzPosition, testFileName.length());
	  }
	  CHECK_THROW_EXPLICIT(io::virtuos::VirtuosFileStructureSetGenerator(testFileName,CT_FILENAME.c_str()).generateStructureSet(), core::InvalidParameterException);
	  testFileName = CT_FILENAME;
	  gzPosition = testFileName.find(".ctx");
	  if(gzPosition != std::string::npos){
		  testFileName.erase(gzPosition, testFileName.length());
	  }
	  CHECK_THROW_EXPLICIT(io::virtuos::VirtuosFileStructureSetGenerator(RTSTRUCT_FILENAME.c_str(),testFileName).generateStructureSet(), core::InvalidParameterException);
	  CHECK_NO_THROW(io::virtuos::VirtuosFileStructureSetGenerator(RTSTRUCT_FILENAME.c_str(),CT_FILENAME.c_str()).generateStructureSet());
	  

	  RETURN_AND_REPORT_TEST_SUCCESS;
	}

  }//testing
}//rttb

