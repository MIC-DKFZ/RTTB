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
#include <boost/shared_ptr.hpp>

#include <iomanip>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomFileDoseAccessorWriter.h"
#include "rttbInvalidDoseException.h"
#include "rttbDcmrtException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{

	namespace testing
	{

		/*!@brief DicomDoseAccessorGeneratorTest - test the generators for dicom data
		1) test dicom file generator generateDoseAccessor()
		2) test dicom IOD generator generateDoseAccessor()
		*/

		int DicomDoseAccessorConverterTest(int argc, char* argv[])
		{
			typedef rttb::core::DoseAccessorInterface::Pointer DoseAccessorPointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			//ARGUMENTS:
			//           1: the file name of the dose to read
			//           2: the file name of the dose to write

			std::string RTDOSE_FILENAME_R;
			std::string RTDOSE_FILENAME_W;

			if (argc > 2)
			{
				RTDOSE_FILENAME_R = argv[1];
				RTDOSE_FILENAME_W = argv[2];
			}

			double errorConstantDDA = 1e-3;
			DoseAccessorPointer doseAccessor_r = io::dicom::DicomFileDoseAccessorGenerator(
			        RTDOSE_FILENAME_R.c_str()).generateDoseAccessor();

			CHECK_NO_THROW(io::dicom::DicomFileDoseAccessorWriter());
			io::dicom::DicomFileDoseAccessorWriter doseConverter;
			CHECK_NO_THROW(doseConverter.setDoseAccessor(doseAccessor_r));
			CHECK_NO_THROW(doseConverter.setFileName(RTDOSE_FILENAME_W));
			CHECK_EQUAL(doseConverter.process(), true);


			DoseAccessorPointer doseAccessor_w = io::dicom::DicomFileDoseAccessorGenerator(
			        RTDOSE_FILENAME_W).generateDoseAccessor();

			//Check geometricinfo
      CHECK(doseAccessor_r->getGeometricInfo().getImagePositionPatient().equalsAlmost(
        doseAccessor_w->getGeometricInfo().getImagePositionPatient(), errorConstantDDA));

      CHECK(doseAccessor_r->getGeometricInfo().getOrientationMatrix().equalsAlmost(
        doseAccessor_w->getGeometricInfo().getOrientationMatrix(), errorConstantDDA));

      CHECK(doseAccessor_r->getGeometricInfo().getSpacing().equalsAlmost(
        doseAccessor_w->getGeometricInfo().getSpacing(), errorConstantDDA));

			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getNumColumns(),
			            doseAccessor_w->getGeometricInfo().getNumColumns(),
				errorConstantDDA);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getNumRows(),
			            doseAccessor_w->getGeometricInfo().getNumRows(),
				errorConstantDDA);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getNumSlices(),
			            doseAccessor_w->getGeometricInfo().getNumSlices(),
				errorConstantDDA);

			//Check pixel data
			unsigned int size = doseAccessor_r->getGeometricInfo().getNumColumns() *
			                    doseAccessor_r->getGeometricInfo().getNumRows() *
			                    doseAccessor_r->getGeometricInfo().getNumSlices() ;

			for (unsigned int index = 0; index < 30; index++)
			{
				CHECK_CLOSE(doseAccessor_r->getValueAt(index), doseAccessor_w->getValueAt(index), errorConstantDDA);

				if (size / 2 - index >= 0 && size / 2 - index < size)
				{
					CHECK_CLOSE(doseAccessor_r->getValueAt(size / 2 - index),
					            doseAccessor_w->getValueAt(size / 2 - index), errorConstantDDA);
				}

				CHECK_CLOSE(doseAccessor_r->getValueAt(size - index - 1),
				            doseAccessor_w->getValueAt(size - index - 1), errorConstantDDA);
			}


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

