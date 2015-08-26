// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision: 5603 $ (last changed revision)
// @date    $Date: 2015-08-24 15:17:30 +0200 (Mo, 24 Aug 2015) $ (last change date)
// @author  $Author: strubel $ (last changed by)
*/

#include "litCheckMacros.h"
#include <boost/filesystem.hpp>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include <vector>

/*! @brief VoxelizerToolTest5.
Search the coordinate at the Image and return the Voxel(Pixel) value.
*/
int main(int argc, char* argv[])
{
	PREPARE_DEFAULT_TEST_REPORTING;

	typedef itk::Image< double, 3 > ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;

	if (argc > 3)
	{
		const std::string pathToBinaryDirectory = argv[1];
		const std::string pathToTestDirectory = argv[2];
		const std::string RTToolBoxTestingDirectory = argv[3];

		const std::string StructCommand = pathToBinaryDirectory +
		                                  "Release/VoxelizerTool -s " + RTToolBoxTestingDirectory +
		                                  "data/DICOM/StructureSet/RS1.3.6.1.4.1.2452.6.841242143.1311652612.1170940299.4217870819.dcm -r " +
		                                  RTToolBoxTestingDirectory + "data/DICOM/TestDose/ConstantTwo.dcm -e Leber";
		int returnValue = system(StructCommand.c_str());
		CHECK_EQUAL(returnValue, 0);

		std::string pathToFile = pathToTestDirectory + "/out_Leber.hdr";

		ImageType::IndexType pixelWithOne_1 =  {{20, 30, 30}};
		ImageType::IndexType pixelWithOne_2 =  {{30, 10, 40}};

		ImageType::IndexType pixelWithZero_1 =  {{40, 30, 30}};
		ImageType::IndexType pixelWithZero_2 =  {{10, 40, 30}};

		ImageType::IndexType pixelAtBorder_1 =  {{12, 23, 27}};
		ImageType::IndexType pixelAtBorder_2 =  {{34, 21, 31}};

		std::vector<ImageType::IndexType> pixelWithOne;
		std::vector<ImageType::IndexType> pixelWithZero;
		std::vector<ImageType::IndexType> pixelAtBorder;

		pixelWithOne.push_back(pixelWithOne_1);
		pixelWithOne.push_back(pixelWithOne_2);

		pixelWithZero.push_back(pixelWithZero_1);
		pixelWithZero.push_back(pixelWithZero_2);

		pixelAtBorder.push_back(pixelAtBorder_1);
		pixelAtBorder.push_back(pixelAtBorder_2);

		if (boost::filesystem::exists(pathToFile))
		{
			ReaderType::Pointer reader = ReaderType::New();
			reader->SetFileName(pathToFile);
			reader->Update();

			auto image = reader->GetOutput();
			ImageType::PixelType pixelValue;

			for (int i = 0; i < pixelWithOne.size(); i++)
			{
				pixelValue = image->GetPixel(pixelWithOne.at(i));
				CHECK_EQUAL(pixelValue, 1.0);
			}

			for (int i = 0; i < pixelWithZero.size(); i++)
			{
				pixelValue = image->GetPixel(pixelWithZero.at(i));
				CHECK_EQUAL(pixelValue, 0.0);
			}

			for (int i = 0; i < pixelAtBorder.size(); i++)
			{
				pixelValue = image->GetPixel(pixelAtBorder.at(i));

				if (i == 0)
				{
					CHECK_CLOSE(pixelValue, 0.265865, 1e-3);
				}
				else if (i == 1)
				{
					CHECK_CLOSE(pixelValue, 0.819613, 1e-3);
				}
			}

			if (boost::filesystem::exists(pathToTestDirectory + "/out_Leber.img"))
			{
				boost::filesystem::remove(pathToTestDirectory + "/out_Leber.img");
			}

			if (boost::filesystem::exists(pathToTestDirectory + "/out_Leber.hdr"))
			{
				boost::filesystem::remove(pathToTestDirectory + "/out_Leber.hdr");
			}
		}
	}

	RETURN_AND_REPORT_TEST_SUCCESS;
}