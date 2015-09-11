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
/*!
// @file
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author $Author$ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbVirtuosPlanFileDoseAccessorGenerator.h"
#include "rttbVirtuosDoseFileDoseAccessorGenerator.h"
#include "rttbVirtuosDoseAccessor.h"
#include "rttbVirtuosFileStructureSetGenerator.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"
#include "rttbIndexOutOfBoundsException.h"
#include "../rttbDoseAccessorTester.h"


namespace rttb
{

	namespace testing
	{

		/*! @brief TripDoseIOTest - test the IO for virtuos data
				2) test virtuos dose import if geometric info was set correctly
				3) test trip dose import accessing dose data and converting

				Since the same class is used for the import of VIRTUOS structures, the constructors are
				tested more thoroughly in VirtuosDoseIOTest.

				WARNING: The values for comparison need to be adjusted if the input files are changed!

				@see VirtuosDoseIOTest

		*/

		int TripDoseIOTest(int argc, char* argv[])
		{
			typedef core::DoseIteratorInterface::DoseAccessorPointer DoseAccessorPointer;

			PREPARE_DEFAULT_TEST_REPORTING;
			//           1: plan  file name (virtuos)		.../testing/data/Virtuos/NHH030101.pln
			//           2: dose1 file name (virtuos)		.../testing/data/Virtuos/NHH030101.dos.gz
			//           3: dose2 file name (trip):		.../testing/data/Virtuos/trip/NHH030101.dos
			// WARNING: Test will fail if dose2 does not contain the same dose as dose1!

			std::string RTPLAN_FILENAME;
			std::string RTDOSE_FILENAME;
			std::string RTDOSE2_FILENAME;

			if (argc > 1)
			{
				RTPLAN_FILENAME = argv[1];
			}

			if (argc > 2)
			{
				RTDOSE_FILENAME = argv[2];
			}

			if (argc > 3)
			{
				RTDOSE2_FILENAME = argv[3];
			}


			/* import VIRTUOS dose */
			io::virtuos::VirtuosPlanFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str(),
			        RTPLAN_FILENAME.c_str());
			boost::shared_ptr<io::virtuos::VirtuosDoseAccessor> doseAccessor1 =
			    boost::static_pointer_cast<io::virtuos::VirtuosDoseAccessor>
			    (doseAccessorGenerator1.generateDoseAccessor());


			//CHECK_EQUAL(76.0,spDoseAccessor1->getPrescribedDose());
			//CHECK_EQUAL(885.0,spDoseAccessor1->getNormalizationDose());

			//2) test dose import if geometric info was set correctly
			core::GeometricInfo geoInfo = doseAccessor1->getGeometricInfo();
			CHECK_EQUAL(256, geoInfo.getNumRows());
			CHECK_EQUAL(256, geoInfo.getNumColumns());
			CHECK_EQUAL(165, geoInfo.getNumSlices());
			CHECK_EQUAL(256 * 256 * 165, doseAccessor1->getGridSize());
			CHECK_EQUAL(OrientationMatrix(), geoInfo.getOrientationMatrix());

			const VoxelGridID start = 0;
			const VoxelGridIndex3D start3D(0);

			VoxelGridID end, inbetween;
			VoxelGridIndex3D end3D, inbetween3D;

			//3) test dose import accessing dose data and converting

			CHECK_EQUAL(0, doseAccessor1->getValueAt(start));
			CHECK_EQUAL(0, doseAccessor1-> getValueAt(start3D));
			CHECK_EQUAL(doseAccessor1->getValueAt(start), doseAccessor1-> getValueAt(start3D));

			inbetween = int(std::floor(doseAccessor1->getGridSize() / 2.0));
			doseAccessor1->getGeometricInfo().convert(inbetween, inbetween3D);

			CHECK_EQUAL(0, doseAccessor1->getValueAt(inbetween));
			CHECK_EQUAL(0, doseAccessor1-> getValueAt(inbetween3D));
			CHECK_EQUAL(doseAccessor1->getValueAt(inbetween), doseAccessor1-> getValueAt(inbetween3D));

			end = doseAccessor1->getGridSize() - 1;
			doseAccessor1->getGeometricInfo().convert(end, end3D);

			CHECK_EQUAL(0, doseAccessor1->getValueAt(end));
			CHECK_EQUAL(0, doseAccessor1-> getValueAt(end3D));
			CHECK_EQUAL(doseAccessor1->getValueAt(end), doseAccessor1-> getValueAt(end3D));

			/* Dose without plan */
			io::virtuos::VirtuosDoseFileDoseAccessorGenerator doseAccessorGenerator2(RTDOSE2_FILENAME.c_str(),
			        doseAccessor1->getNormalizationDose(),
			        doseAccessor1->getPrescribedDose());
			boost::shared_ptr<io::virtuos::VirtuosDoseAccessor> doseAccessor2 =
			    boost::static_pointer_cast<io::virtuos::VirtuosDoseAccessor>
			    (doseAccessorGenerator2.generateDoseAccessor());


			CHECK_EQUAL(doseAccessor1->getPrescribedDose(), doseAccessor2->getPrescribedDose());
			CHECK_EQUAL(doseAccessor1->getNormalizationDose(), doseAccessor2->getNormalizationDose());


			//2) test dose import if geometric info was set correctly
			core::GeometricInfo geoInfo2 = doseAccessor2->getGeometricInfo();
			CHECK_EQUAL(geoInfo.getNumRows(), geoInfo2.getNumRows());
			CHECK_EQUAL(geoInfo.getNumColumns(), geoInfo2.getNumColumns());
			CHECK_EQUAL(geoInfo.getNumSlices(), geoInfo2.getNumSlices());
			CHECK_EQUAL(doseAccessor1->getGridSize(), doseAccessor2->getGridSize());
			CHECK_EQUAL(geoInfo.getOrientationMatrix(), geoInfo2.getOrientationMatrix());

			//3) test dose import accessing dose data and converting

			CHECK_EQUAL(doseAccessor1->getValueAt(start), doseAccessor2->getValueAt(start));
			CHECK_EQUAL(doseAccessor1-> getValueAt(start3D), doseAccessor2-> getValueAt(start3D));
			CHECK_EQUAL(doseAccessor2->getValueAt(start), doseAccessor2-> getValueAt(start3D));

			inbetween = int(std::floor(doseAccessor2->getGridSize() / 2.0));
			doseAccessor2->getGeometricInfo().convert(inbetween, inbetween3D);

			CHECK_EQUAL(doseAccessor1->getValueAt(inbetween), doseAccessor2->getValueAt(inbetween));
			CHECK_EQUAL(doseAccessor1-> getValueAt(inbetween3D), doseAccessor2-> getValueAt(inbetween3D));
			CHECK_EQUAL(doseAccessor2->getValueAt(inbetween), doseAccessor2-> getValueAt(inbetween3D));

			end = doseAccessor2->getGridSize() - 1;
			doseAccessor2->getGeometricInfo().convert(end, end3D);

			CHECK_EQUAL(doseAccessor1->getValueAt(end), doseAccessor2->getValueAt(end));
			CHECK_EQUAL(doseAccessor1-> getValueAt(end3D), doseAccessor2-> getValueAt(end3D));
			CHECK_EQUAL(doseAccessor2->getValueAt(end), doseAccessor2-> getValueAt(end3D));

			/* Import Trip dose */
			io::virtuos::VirtuosDoseFileDoseAccessorGenerator doseAccessorGenerator3(RTDOSE2_FILENAME.c_str(),
			        doseAccessor1->getNormalizationDose(),
			        doseAccessor1->getPrescribedDose());
			boost::shared_ptr<io::virtuos::VirtuosDoseAccessor> doseAccessor3 =
			    boost::static_pointer_cast<io::virtuos::VirtuosDoseAccessor>
			    (doseAccessorGenerator3.generateDoseAccessor());

			CHECK_EQUAL(doseAccessor1->getPrescribedDose(), doseAccessor3->getPrescribedDose());
			CHECK_EQUAL(doseAccessor1->getNormalizationDose(), doseAccessor3->getNormalizationDose());


			//2) test dose import if geometric info was set correctly
			core::GeometricInfo geoInfo3 = doseAccessor3->getGeometricInfo();
			CHECK_EQUAL(geoInfo.getNumRows(), geoInfo3.getNumRows());
			CHECK_EQUAL(geoInfo.getNumColumns(), geoInfo3.getNumColumns());
			CHECK_EQUAL(geoInfo.getNumSlices(), geoInfo3.getNumSlices());
			CHECK_EQUAL(doseAccessor1->getGridSize(), doseAccessor3->getGridSize());
			CHECK_EQUAL(geoInfo.getOrientationMatrix(), geoInfo3.getOrientationMatrix());

			//3) test dose import accessing dose data and converting

			CHECK_EQUAL(doseAccessor1->getValueAt(start), doseAccessor3->getValueAt(start));
			CHECK_EQUAL(doseAccessor1-> getValueAt(start3D), doseAccessor3-> getValueAt(start3D));
			CHECK_EQUAL(doseAccessor3->getValueAt(start), doseAccessor3-> getValueAt(start3D));

			inbetween = int(std::floor(doseAccessor3->getGridSize() / 2.0));
			doseAccessor3->getGeometricInfo().convert(inbetween, inbetween3D);

			CHECK_EQUAL(doseAccessor1->getValueAt(inbetween), doseAccessor3->getValueAt(inbetween));
			CHECK_EQUAL(doseAccessor1-> getValueAt(inbetween3D), doseAccessor3-> getValueAt(inbetween3D));
			CHECK_EQUAL(doseAccessor3->getValueAt(inbetween), doseAccessor3-> getValueAt(inbetween3D));

			io::virtuos::VirtuosPlanFileDoseAccessorGenerator doseAccessorGenerator4(RTDOSE2_FILENAME.c_str(),
			        RTPLAN_FILENAME.c_str());
			boost::shared_ptr<io::virtuos::VirtuosDoseAccessor> spDoseAccessor4 =
			    boost::static_pointer_cast<io::virtuos::VirtuosDoseAccessor>
			    (doseAccessorGenerator4.generateDoseAccessor());
			DoseAccessorPointer doseAccessor4(spDoseAccessor4);

			DoseAccessorTester doseCompare(doseAccessor1, doseAccessor2);
			CHECK_TESTER(doseCompare);

			DoseAccessorTester doseCompare2(doseAccessor3, doseAccessor4);
			CHECK_TESTER(doseCompare2);

			DoseAccessorTester doseCompare3(doseAccessor1, doseAccessor3);
			CHECK_TESTER(doseCompare3);


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

