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

#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbLQModelAccessor.h"
#include "rttbITKImageAccessor.h"
#include "rttbITKImageAccessorConverter.h"

namespace rttb
{

	namespace testing
	{

		/*!@brief ITKBioModelAccessorConverterTest - test the conversion rttb BioModel accessor ->itk
		1) test with dicom file (DicomDoseAccessorGenerator)
		*/

		int ITKBioModelAccessorConverterTest(int argc, char* argv[])
		{
			typedef core::AccessorInterface::Pointer AccessorPointer;
			typedef core::DoseAccessorInterface::Pointer DoseAccessorPointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			std::string RTDOSE_FILENAME;

			if (argc > 1)
			{
				RTDOSE_FILENAME = argv[1];
			}

			//1) Read dicomFile and test process() and  getITKImage()

			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator(RTDOSE_FILENAME.c_str());
			DoseAccessorPointer doseAccessor(doseAccessorGenerator.generateDoseAccessor());

			AccessorPointer LQWithConstantDose = boost::make_shared<models::LQModelAccessor>(doseAccessor, 0.2,
			                                     0.02);

			io::itk::ITKImageAccessorConverter itkConverter(LQWithConstantDose);

			CHECK_NO_THROW(itkConverter.process());
			CHECK_NO_THROW(itkConverter.getITKImage());

			io::itk::ITKImageAccessor::ITKImageType::IndexType itkIndex;
			itkIndex[0] = itkIndex[1] = itkIndex[2] = 0;

			VoxelGridIndex3D rttbIndex(0, 0, 0);

			CHECK_EQUAL(itkConverter.getITKImage()->GetPixel(itkIndex),
			            LQWithConstantDose->getValueAt(rttbIndex));

			itkIndex[0] = rttbIndex[0] = doseAccessor->getGeometricInfo().getNumColumns() / 2;
			itkIndex[1] = rttbIndex[1] = doseAccessor->getGeometricInfo().getNumRows() / 2;
			itkIndex[2] = rttbIndex[2] = doseAccessor->getGeometricInfo().getNumSlices() / 2;

			CHECK_EQUAL(itkConverter.getITKImage()->GetPixel(itkIndex),
			            LQWithConstantDose->getValueAt(rttbIndex));

			itkIndex[0] = rttbIndex[0] = doseAccessor->getGeometricInfo().getNumColumns() - 1;
			itkIndex[1] = rttbIndex[1] = doseAccessor->getGeometricInfo().getNumRows() - 1;
			itkIndex[2] = rttbIndex[2] = doseAccessor->getGeometricInfo().getNumSlices() - 1;

			CHECK_EQUAL(itkConverter.getITKImage()->GetPixel(itkIndex),
			            LQWithConstantDose->getValueAt(rttbIndex));

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

