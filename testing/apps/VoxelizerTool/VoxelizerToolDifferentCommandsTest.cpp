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

#include "litCheckMacros.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include <vector>

/*! @brief VoxelizerToolTest3.
Test the output, multipleStructs and the booleanVoxelization parameter.
*/
namespace rttb
{
	namespace testing
	{
		//path to the current running directory. VoxelizerTool is in the same directory (Debug/Release)
		extern const char* _callingAppPath;
		int VoxelizerToolDifferentCommandsTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			typedef itk::Image< double, 3 > ImageType;
			typedef itk::ImageFileReader<ImageType> ReaderType;

			std::string voxelizerToolExe;
			std::string tempDirectory;
			std::string structFile;
			std::string referenceFile;

			if (argc > 4)
			{
				voxelizerToolExe = argv[1];
				tempDirectory = argv[2];
				structFile = argv[3];
				referenceFile = argv[4];
			}

			std::vector<std::string> commands;
			commands.push_back("\"Niere.*\" -m -o Test.nrrd");
			commands.push_back("\"Rueckenmark\" -o Boolean.nrrd -z");

			std::vector<std::string> filenames;
			filenames.push_back("Test_Niere li");
			filenames.push_back("Test_Niere re");
			filenames.push_back("Boolean");

			std::vector<std::string> structnames;
			structnames.push_back("Boost");
			structnames.push_back("PTV");
			structnames.push_back("Ref.Pkt");
			structnames.push_back("Darm");
			structnames.push_back("Leber");
			structnames.push_back("Magen_DD");
			structnames.push_back("Niere li");
			structnames.push_back("Niere re");
			structnames.push_back("Rueckenmark");
			structnames.push_back("Aussenkontur");

		    std::vector<std::pair<ImageType::IndexType, ImageType::PixelType> > voxelsToTestInside;
		    std::vector<std::pair<ImageType::IndexType, ImageType::PixelType> > voxelsToTestOutside;
		    voxelsToTestInside.push_back(std::make_pair(ImageType::IndexType{ 48, 31, 18 }, 1.0)); //Niere li inside
		    voxelsToTestOutside.push_back(std::make_pair(ImageType::IndexType{ 19, 31, 18 }, 0.0)); //Niere li outside
		    voxelsToTestInside.push_back(std::make_pair(ImageType::IndexType{ 19, 31, 18 }, 1.0)); //Niere re inside
		    voxelsToTestOutside.push_back(std::make_pair(ImageType::IndexType{ 48, 31, 18 }, 0.0)); //Niere re outside
		    voxelsToTestInside.push_back(std::make_pair(ImageType::IndexType{ 35, 32, 30 }, 1.0)); //Rueckenmark inside
		    voxelsToTestOutside.push_back(std::make_pair(ImageType::IndexType{ 35, 30, 23 }, 0.0)); //Rueckenmark outside

			boost::filesystem::path callingPath(_callingAppPath);
			std::string voxelizerToolExeWithPath = callingPath.parent_path().string() + "/" + voxelizerToolExe;

			std::string baseCommand = voxelizerToolExeWithPath;
			baseCommand += " -s \"" + structFile + "\"";
			baseCommand += " -r \"" + referenceFile + "\"";
			baseCommand += " -e ";

			for (size_t i = 0; i < commands.size(); i++)
			{
				std::string command = baseCommand + commands.at(i);
				int returnValue = system(command.c_str());
				std::cout << "Command line call: " + command << std::endl;
				CHECK_EQUAL(returnValue, 0);
			}

			for (size_t i = 0; i < filenames.size(); i++)
			{
				const std::string NRRDfileName = tempDirectory + "/" + filenames.at(i) + ".nrrd";
				boost::filesystem::path NRRDFile(NRRDfileName);

				CHECK_EQUAL(boost::filesystem::exists(NRRDFile), true);

				//check voxel values
				if (boost::filesystem::exists(NRRDFile))
				{
				  ReaderType::Pointer reader = ReaderType::New();
				  reader->SetFileName(NRRDfileName);
				  reader->Update();

				  ReaderType::OutputImageType::ConstPointer image = reader->GetOutput();

				  ImageType::PixelType voxelValueInside = image->GetPixel(voxelsToTestInside.at(i).first);
				  ImageType::PixelType expectedVoxelValueInside = voxelsToTestInside.at(i).second;
				  CHECK_EQUAL(voxelValueInside, expectedVoxelValueInside);

				  ImageType::PixelType voxelValueOutside = image->GetPixel(voxelsToTestOutside.at(i).first);
				  ImageType::PixelType expectedVoxelValueOutside = voxelsToTestOutside.at(i).second;
				  CHECK_EQUAL(voxelValueOutside, expectedVoxelValueOutside);
				}

				if (boost::filesystem::exists(NRRDFile))
				{
					boost::filesystem::remove(NRRDFile);
				}
			}

			std::string allStructsCommand = voxelizerToolExeWithPath + " -s \"" + structFile + "\"";
			allStructsCommand += " -r \"" + referenceFile + "\" -f";
			int allStructsReturnValue = system(allStructsCommand.c_str());
			std::cout << "Command line call: " + allStructsCommand << std::endl;
			CHECK_EQUAL(allStructsReturnValue, 0);

			for (size_t i = 0; i < structnames.size(); i++)
			{
				const std::string structName = tempDirectory + "/output_" + structnames.at(i) + ".nrrd";
				boost::filesystem::path structFilePath(structName);
				CHECK_EQUAL(boost::filesystem::exists(structFilePath), true);
				if (boost::filesystem::exists(structFilePath))
				{
					boost::filesystem::remove(structFilePath);
				}
			}

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}
}
