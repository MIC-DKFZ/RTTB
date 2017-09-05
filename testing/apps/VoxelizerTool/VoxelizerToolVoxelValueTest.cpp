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
// @version $Revision: 1333 $ (last changed revision)
// @date    $Date: 2016-04-22 11:12:14 +0200 (Fr, 22 Apr 2016) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "litCheckMacros.h"
#include <boost/filesystem.hpp>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include <vector>
#include <boost/assign/list_of.hpp>

/*! @brief VoxelizerToolTest5.
Search the coordinate at the Image and return the Voxel(Pixel) value.
*/

namespace rttb
{
	namespace testing
	{
		//path to the current running directory. VoxelizerTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;
		int VoxelizerToolVoxelValue(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			typedef itk::Image< double, 3 > ImageType;
			typedef itk::ImageFileReader<ImageType> ReaderType;

			std::string voxelizerToolExe;
			std::string tempDirectory;
			std::string structFile;
			std::string referenceFile;
			std::string structName;

			if (argc > 5)
			{
				voxelizerToolExe = argv[1];
				tempDirectory = argv[2];
				structFile = argv[3];
				referenceFile = argv[4];
				structName = argv[5];
			}

			boost::filesystem::path callingPath(_callingAppPath);
			std::string voxelizerToolExeWithPath = callingPath.parent_path().string() + "/" + voxelizerToolExe;

			std::string command = voxelizerToolExeWithPath;
			command += " -s \"" + structFile + "\"";
			command += " -r \"" + referenceFile + "\"";
			command += " -e " + structName;
            command += " -o testOutputVoxelValue.hdr";

			int returnValue = system(command.c_str());
			CHECK_EQUAL(returnValue, 0);

			//image values taken in Mevislab
			//Index inside
			ImageType::IndexType voxelInside1 =  {{20, 30, 30}};
			ImageType::IndexType voxelInside2 =  {{30, 10, 40}};

			//Outside index
			ImageType::IndexType voxelOutside1 =  {{40, 30, 30}};
			ImageType::IndexType voxelOutside2 =  {{10, 40, 30}};

			//Border index
			ImageType::IndexType voxelBorder1 =  {{12, 23, 27}};
			ImageType::IndexType voxelBorder2 =  {{34, 21, 31}};

			std::vector<ImageType::IndexType> voxelIndices = boost::assign::list_of(voxelInside1)(voxelInside2)(
			            voxelOutside1)(
			            voxelOutside2)(voxelBorder1)(voxelBorder2);

			std::vector<ImageType::PixelType> expectedVoxelValues = boost::assign::list_of(1.0)(1.0)(0.0)(0.0)(
			            0.265865)(0.819613);

			std::string filenameHDRWithVoxelization = tempDirectory + "/testOutputVoxelValue.hdr";
			std::string filenameIMGWithVoxelization = tempDirectory + "/testOutputVoxelValue.img";

			CHECK(boost::filesystem::exists(filenameHDRWithVoxelization));
			CHECK(boost::filesystem::exists(filenameIMGWithVoxelization));

			if (boost::filesystem::exists(filenameHDRWithVoxelization))
			{
				ReaderType::Pointer reader = ReaderType::New();
				reader->SetFileName(filenameHDRWithVoxelization);
				reader->Update();

				ReaderType::OutputImageType::ConstPointer image = reader->GetOutput();

				for (size_t i = 0; i < voxelIndices.size(); i++)
				{
					ImageType::PixelType voxelValue = image->GetPixel(voxelIndices.at(i));
					ImageType::PixelType expectedVoxelValue = expectedVoxelValues.at(i);

					if (expectedVoxelValue == 0.0 || expectedVoxelValue == 1.0)
					{
						CHECK_EQUAL(voxelValue, expectedVoxelValue);
					}
					else
					{
						CHECK_CLOSE(voxelValue, expectedVoxelValue, 1e-4);
					}
				}
                
				if (boost::filesystem::exists(filenameHDRWithVoxelization))
				{
					boost::filesystem::remove(filenameHDRWithVoxelization);
				}

				if (boost::filesystem::exists(filenameIMGWithVoxelization))
				{
					boost::filesystem::remove(filenameIMGWithVoxelization);
				}
			}

			RETURN_AND_REPORT_TEST_SUCCESS;
		}
	}
}
