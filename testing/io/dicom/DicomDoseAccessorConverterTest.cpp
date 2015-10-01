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
			typedef boost::shared_ptr<DRTDoseIOD> DRTDoseIODPointer;
			typedef rttb::io::dicom::DicomDoseAccessor::DoseAccessorPointer DoseAccessorPointer;

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

			double errorConstant = 1e-3;
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
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getImagePositionPatient().x(),
			            doseAccessor_w->getGeometricInfo().getImagePositionPatient().x(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getImagePositionPatient().y(),
			            doseAccessor_w->getGeometricInfo().getImagePositionPatient().y(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getImagePositionPatient().z(),
			            doseAccessor_w->getGeometricInfo().getImagePositionPatient().z(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getImageOrientationColumn().x(),
			            doseAccessor_w->getGeometricInfo().getImageOrientationColumn().x(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getImageOrientationColumn().y(),
			            doseAccessor_w->getGeometricInfo().getImageOrientationColumn().y(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getImageOrientationColumn().z(),
			            doseAccessor_w->getGeometricInfo().getImageOrientationColumn().z(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getImageOrientationRow().x(),
			            doseAccessor_w->getGeometricInfo().getImageOrientationRow().x(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getImageOrientationRow().y(),
			            doseAccessor_w->getGeometricInfo().getImageOrientationRow().y(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getImageOrientationRow().z(),
			            doseAccessor_w->getGeometricInfo().getImageOrientationRow().z(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getSpacing().x(), doseAccessor_w->getGeometricInfo().getSpacing().x(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getSpacing().y(), doseAccessor_w->getGeometricInfo().getSpacing().y(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getSpacing().z(), doseAccessor_w->getGeometricInfo().getSpacing().z(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getNumColumns(), doseAccessor_w->getGeometricInfo().getNumColumns(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getNumRows(), doseAccessor_w->getGeometricInfo().getNumRows(),
			            errorConstant);
			CHECK_CLOSE(doseAccessor_r->getGeometricInfo().getNumSlices(), doseAccessor_w->getGeometricInfo().getNumSlices(),
			            errorConstant);

			//Check pixel data
			int size = doseAccessor_r->getGeometricInfo().getNumColumns() * doseAccessor_r->getGeometricInfo().getNumRows() *
			           doseAccessor_r->getGeometricInfo().getNumSlices() ;

			for (unsigned int index = 0; index < 30; index++)
			{
				CHECK_CLOSE(doseAccessor_r->getValueAt(index), doseAccessor_w->getValueAt(index), errorConstant);

				if (size / 2 - index >= 0 && size / 2 - index < size)
				{
					CHECK_CLOSE(doseAccessor_r->getValueAt(size / 2 - index), doseAccessor_w->getValueAt(size / 2 - index), errorConstant);
				}

				CHECK_CLOSE(doseAccessor_r->getValueAt(size - index - 1), doseAccessor_w->getValueAt(size - index - 1), errorConstant);
			}


			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

