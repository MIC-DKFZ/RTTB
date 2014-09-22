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
#include "rttbGeometricInfo.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbDicomHelaxFileDoseAccessorGenerator.h"
#include "rttbDicomHelaxIODVecDoseAccessorGenerator.h"
#include "rttbDicomHelaxDoseAccessor.h"
#include "rttbDicomFileStructureSetGenerator.h"


namespace rttb
{

	namespace testing
	{

		/*!@brief DicomHelaxIOTest - test the IO for dicom helax data
				1) test dicom helax dose import if geometric info was set correctly
				2) test dicom helax dose import accessing dose data and converting
				3) test dicom helax structure import

			WARNING: The values for comparison need to be adjusted if the input files are changed!
		*/

		int DicomHelaxIOTest(int argc, char* argv[])
		{
			typedef core::DoseIteratorInterface::DoseAccessorPointer DoseAccessorPointer;
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS: 1: structure file name
			//           2: dose1 directory name


			std::string RTSTRUCT_FILENAME;
			std::string RTDOSE_DIRNAME;


			if (argc > 1)
			{
				RTSTRUCT_FILENAME = argv[1];
			}

			if (argc > 2)
			{
				RTDOSE_DIRNAME = argv[2];
			}



			OFCondition status;
			DcmFileFormat fileformat;

			/* read dicom-rt dose */
			io::helax::DicomHelaxFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_DIRNAME.c_str());
			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			//1) test dicom dose import if geometric info was set correctly
			core::GeometricInfo geoInfo = doseAccessor1->getGeometricInfo();
			CHECK_EQUAL(64, geoInfo.getNumRows());
			CHECK_EQUAL(54, geoInfo.getNumColumns());
			CHECK_EQUAL(52, geoInfo.getNumSlices());
			CHECK_EQUAL(OrientationMatrix(), geoInfo.getOrientationMatrix());

			const VoxelGridID start = 0;
			const VoxelGridIndex3D start3D(0);

			VoxelGridID end, inbetween;
			VoxelGridIndex3D end3D, inbetween3D;

			//2) test dicom dose import accessing dose data and converting

			CHECK_EQUAL(doseAccessor1->getDoseAt(start), doseAccessor1-> getDoseAt(start3D));

			inbetween = int(std::floor(doseAccessor1->getGridSize() / 2.0));
			doseAccessor1->getGeometricInfo().convert(inbetween, inbetween3D);

			CHECK_EQUAL(doseAccessor1->getDoseAt(inbetween), doseAccessor1-> getDoseAt(inbetween3D));

			end = doseAccessor1->getGridSize() - 1;
			doseAccessor1->getGeometricInfo().convert(end, end3D);

			CHECK_EQUAL(doseAccessor1->getDoseAt(end), doseAccessor1-> getDoseAt(end3D));


			/* structure set */
			StructureSetPointer rtStructureSet = io::dicom::DicomFileStructureSetGenerator(
			        RTSTRUCT_FILENAME.c_str()).generateStructureSet();
			//3) test structure import
			CHECK_EQUAL(8, rtStructureSet->getNumberOfStructures());

			CHECK_EQUAL("Patient outline", (rtStructureSet->getStructure(0))->getLabel());
			CHECK_EQUAL("boost", (rtStructureSet->getStructure(1))->getLabel());
			CHECK_EQUAL("Chiasma", (rtStructureSet->getStructure(2))->getLabel());
			CHECK_EQUAL("Sehnerv li.", (rtStructureSet->getStructure(3))->getLabel());
			CHECK_EQUAL("Sehnerv re.", (rtStructureSet->getStructure(4))->getLabel());
			CHECK_EQUAL("Auge li.", (rtStructureSet->getStructure(5))->getLabel());
			CHECK_EQUAL("Auge re.", (rtStructureSet->getStructure(6))->getLabel());
			CHECK_EQUAL("Hirnstamm", (rtStructureSet->getStructure(7))->getLabel());

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

