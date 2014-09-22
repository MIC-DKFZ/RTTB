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

#include <iomanip>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbDicomIODStructureSetGenerator.h"
#include "rttbDcmrtException.h"
#include "rttbInvalidParameterException.h"

namespace rttb{

  namespace testing{

	/*!@brief DicomIOTest - test structure set generator for dicom data
			1) test dicom file structure set generator
			2) test dicom IOD structure set generator
	*/
   
	int DicomStructureSetGeneratorTest(int argc, char* argv[] )
	{
	  typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;
	  //typedef boost::shared_ptr<DRTStructureSetIOD> DRTStrSetIODPtr;
	  typedef io::dicom::DicomIODStructureSetGenerator::DRTStrSetIODPtr DRTStrSetIODPtr;
	  
	  PREPARE_DEFAULT_TEST_REPORTING;
	  //ARGUMENTS: 1: structure file name
	  

	  std::string RTSTRUCT_FILENAME;
	  

	  if (argc>1)
	  {
		RTSTRUCT_FILENAME = argv[1];
	  }
	  

	  /* structure set */
	  //1) test dicom file structure set generator
	  CHECK_NO_THROW(io::dicom::DicomFileStructureSetGenerator(""));  
	  CHECK_NO_THROW(io::dicom::DicomFileStructureSetGenerator("Test.test"));
	  CHECK_THROW_EXPLICIT(io::dicom::DicomFileStructureSetGenerator("Test.test").generateStructureSet(), io::dicom::DcmrtException);
	  CHECK_NO_THROW(io::dicom::DicomFileStructureSetGenerator(RTSTRUCT_FILENAME.c_str()));

	  StructureSetPointer rtStructureSet=io::dicom::DicomFileStructureSetGenerator(RTSTRUCT_FILENAME.c_str()).generateStructureSet();


	  //2) test dicom IOD structure set generator
	  OFCondition status;
	  DcmFileFormat fileformat;	
	  boost::shared_ptr<DRTStructureSetIOD> drtStrSetIOD=boost::make_shared<DRTStructureSetIOD>();

	  CHECK_NO_THROW(io::dicom::DicomIODStructureSetGenerator generator1(drtStrSetIOD));
	  CHECK_THROW_EXPLICIT(io::dicom::DicomIODStructureSetGenerator(drtStrSetIOD).generateStructureSet(), core::InvalidParameterException);

	  status = fileformat.loadFile(RTSTRUCT_FILENAME.c_str());
	  status = drtStrSetIOD->read(*fileformat.getDataset());

	  CHECK_NO_THROW(io::dicom::DicomIODStructureSetGenerator generator2(drtStrSetIOD));
	  CHECK_NO_THROW(io::dicom::DicomIODStructureSetGenerator(drtStrSetIOD).generateStructureSet());

	  StructureSetPointer rtStructureSet2=io::dicom::DicomIODStructureSetGenerator(drtStrSetIOD).generateStructureSet();
	  CHECK_EQUAL(rtStructureSet2->getNumberOfStructures(),rtStructureSet->getNumberOfStructures());

	 

	  RETURN_AND_REPORT_TEST_SUCCESS;
	}

  }//testing
}//rttb

