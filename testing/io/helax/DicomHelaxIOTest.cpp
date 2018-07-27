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

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>

#include "litCheckMacros.h"

#include "rttbDoseIteratorInterface.h"
#include "rttbDicomHelaxFileDoseAccessorGenerator.h"
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
			typedef core::StructureSet::Pointer StructureSetPointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//ARGUMENTS: 1: directory name

			std::string RT_DIRNAME;


			if (argc > 1)
			{
				RT_DIRNAME = argv[1];
			}



			OFCondition status;
			DcmFileFormat fileformat;

			/* read dicom-rt dose */
			io::helax::DicomHelaxFileDoseAccessorGenerator doseAccessorGenerator1(RT_DIRNAME.c_str());
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

			CHECK_EQUAL(doseAccessor1->getValueAt(start), doseAccessor1-> getValueAt(start3D));

			inbetween = int(std::floor(doseAccessor1->getGridSize() / 2.0));
			doseAccessor1->getGeometricInfo().convert(inbetween, inbetween3D);

			CHECK_EQUAL(doseAccessor1->getValueAt(inbetween), doseAccessor1-> getValueAt(inbetween3D));

			end = doseAccessor1->getGridSize() - 1;
			doseAccessor1->getGeometricInfo().convert(end, end3D);

			CHECK_EQUAL(doseAccessor1->getValueAt(end), doseAccessor1-> getValueAt(end3D));


			/* structure set */
			StructureSetPointer rtStructureSet = io::dicom::DicomFileStructureSetGenerator(
			        RT_DIRNAME.c_str()).generateStructureSet();
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

