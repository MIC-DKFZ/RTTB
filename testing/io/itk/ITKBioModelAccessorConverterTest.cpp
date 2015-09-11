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
// @version $Revision: 793 $ (last changed revision)
// @date $Date: 2014-10-10 10:24:45 +0200 (Fr, 10 Okt 2014) $ (last change date)
// @author $Author: hentsch $ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"
#include "litImageTester.h"
#include "litTestImageIO.h"

#include "itkImage.h"
#include "itkImageFileReader.h"

#include "rttbBaseType.h"
#include "rttbDoseIteratorInterface.h"
#include "rttbAccessorInterface.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbLQModelAccessor.h"
#include "rttbITKImageAccessor.h"
#include "rttbITKImageFileAccessorGenerator.h"
#include "rttbITKImageAccessorGenerator.h"
#include "rttbITKImageAccessorConverter.h"
#include "rttbDoseAccessorProcessorBase.h"
#include "rttbDoseAccessorConversionSettingInterface.h"
#include "rttbInvalidDoseException.h"


namespace rttb
{

	namespace testing
	{

		/*!@brief ITKBioModelAccessorConverterTest - test the conversion rttb BioModel accessor ->itk
		1) test with dicom file (DicomDoseAccessorGenerator)
		*/

		int ITKBioModelAccessorConverterTest(int argc, char* argv[])
		{
			typedef core::AccessorInterface::AccessorPointer AccessorPointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			std::string RTDOSE_FILENAME;

			if (argc > 1)
			{
				RTDOSE_FILENAME = argv[1];
			}

			//1) Read dicomFile and test process() and  getITKImage()

			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator(RTDOSE_FILENAME.c_str());
			auto doseAccessor(doseAccessorGenerator.generateDoseAccessor());

			AccessorPointer LQWithConstantDose = boost::make_shared<models::LQModelAccessor>(doseAccessor, 0.2, 0.02);

			io::itk::ITKImageAccessorConverter itkConverter(LQWithConstantDose);

			CHECK_NO_THROW(itkConverter.process());
			CHECK_NO_THROW(itkConverter.getITKImage());

			io::itk::ITKImageAccessor::ITKImageType::IndexType itkIndex;
			itkIndex[0] = itkIndex[1] = itkIndex[2] = 0;

			VoxelGridIndex3D rttbIndex(0, 0, 0);

			CHECK_EQUAL(itkConverter.getITKImage()->GetPixel(itkIndex), LQWithConstantDose->getValueAt(rttbIndex));

			itkIndex[0] = rttbIndex[0] = doseAccessor->getGeometricInfo().getNumColumns() / 2;
			itkIndex[1] = rttbIndex[1] = doseAccessor->getGeometricInfo().getNumRows() / 2;
			itkIndex[2] = rttbIndex[2] = doseAccessor->getGeometricInfo().getNumSlices() / 2;

			CHECK_EQUAL(itkConverter.getITKImage()->GetPixel(itkIndex), LQWithConstantDose->getValueAt(rttbIndex));

			itkIndex[0] = rttbIndex[0] = doseAccessor->getGeometricInfo().getNumColumns() - 1;
			itkIndex[1] = rttbIndex[1] = doseAccessor->getGeometricInfo().getNumRows() - 1;
			itkIndex[2] = rttbIndex[2] = doseAccessor->getGeometricInfo().getNumSlices() - 1;

			CHECK_EQUAL(itkConverter.getITKImage()->GetPixel(itkIndex), LQWithConstantDose->getValueAt(rttbIndex));

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

