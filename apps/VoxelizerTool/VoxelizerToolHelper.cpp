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
// @version $Revision: 1127 $ (last changed revision)
// @date    $Date: 2015-10-01 13:33:33 +0200 (Do, 01 Okt 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/
#include <iostream>

#include "rttbBoostMaskAccessor.h"

#include "itkMacro.h"

#include "VoxelizerToolHelper.h"
#include "VoxelizerToolApplicationData.h"

#include "rttbITKImageMaskAccessorConverter.h"
#include "rttbImageWriter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkAddImageFilter.h"

#include <regex>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>

#include "rttbDicomFileStructureSetGenerator.h"

void rttb::apps::voxelizerTool::removeSpecialCharacters(std::string& label)
{

	//Replace / to avoid problems with directories (struct "Magen/DD" --> Magen/DD.mhd), delete trailing . to avoid filenames with two trailing points (Niere re. --> Niere re..mhd)
	while (label.find("/") != std::string::npos)
	{
		label.replace(label.find("/"), 1, "_");
	}

	if (*label.rbegin() == '.')
	{
		label.replace(label.size() - 1, 1, "");
	}
}

std::string rttb::apps::voxelizerTool::getFilenameWithoutEnding(const std::string& outfilename)
{
	boost::filesystem::path p(outfilename);
	return p.replace_extension("").string();
}

std::string rttb::apps::voxelizerTool::getFileEnding(const std::string& outfilename)
{
	boost::filesystem::path p(outfilename);
	return p.extension().string();
}

rttb::core::MaskAccessorInterface::MaskAccessorPointer rttb::apps::voxelizerTool::createMask(
    rttb::core::DoseAccessorInterface::DoseAccessorPointer doseAccessorPtr,
    rttb::core::Structure::StructTypePointer structurePtr,
    bool strict)
{
    rttb::core::MaskAccessorInterface::MaskAccessorPointer maskAccessorPtr;

    if (doseAccessorPtr != nullptr && structurePtr != nullptr)
    {
        maskAccessorPtr = boost::make_shared<rttb::masks::boost::BoostMaskAccessor>
            (structurePtr, doseAccessorPtr->getGeometricInfo(),
                strict);

        maskAccessorPtr->updateMask();
    }

    return maskAccessorPtr;
}

void rttb::apps::voxelizerTool::writeMaskToFile(std::vector<core::MaskAccessorInterface::MaskAccessorPointer> maskVector,
    const std::string& outputFileName, bool voxelization)
{
    if (!maskVector.empty())
    {
        io::itk::ITKImageMaskAccessor::ITKMaskImageType::ConstPointer itkImage;

        if (maskVector.size() > 1)
        {
            itkImage = addMultipleStructsToImage(maskVector);
        }
        else
        {
            io::itk::ITKImageMaskAccessorConverter maskAccessorConverter(maskVector.at(0));
            maskAccessorConverter.process();
            itkImage = maskAccessorConverter.getITKImage();
        }

        if (voxelization)
        {
            itkImage = applyThresholdFilter(itkImage);
        }

        io::itk::ImageWriter writer(outputFileName, itkImage);
        writer.writeFile();
    }
}

rttb::io::itk::ITKImageMaskAccessor::ITKMaskImageType::ConstPointer rttb::apps::voxelizerTool::addMultipleStructsToImage(
    std::vector<core::MaskAccessorInterface::MaskAccessorPointer> maskVector)
{
    std::vector<io::itk::ITKImageMaskAccessor::ITKMaskImageType::Pointer> listOfITKImages;

    for (const auto & i : maskVector)
    {
        io::itk::ITKImageMaskAccessorConverter maskAccessorConverter(i);
        maskAccessorConverter.process();
        listOfITKImages.push_back(maskAccessorConverter.getITKImage());
    }

    itk::AddImageFilter <itk::Image<rttb::DoseTypeGy, 3>, itk::Image<rttb::DoseTypeGy, 3>>::Pointer addFilter = 
        itk::AddImageFilter <itk::Image<rttb::DoseTypeGy, 3>, itk::Image<rttb::DoseTypeGy, 3>>::New();
    io::itk::ITKImageMaskAccessor::ITKMaskImageType::Pointer filterResult;

    for (int k = 1; k < listOfITKImages.size(); k++)
    {
        if (k == 1)
        {
            addFilter->SetInput1(listOfITKImages.at(0));
        }
        else
        {
            addFilter->SetInput1(filterResult);
        }

        addFilter->SetInput2(listOfITKImages.at(k));

        addFilter->Update();

        filterResult = addFilter->GetOutput();
    }

    io::itk::ITKImageMaskAccessor::ITKMaskImageType::ConstPointer filterResultConst(filterResult);
    return filterResultConst;
}

rttb::io::itk::ITKImageMaskAccessor::ITKMaskImageType::ConstPointer rttb::apps::voxelizerTool::applyThresholdFilter(
    io::itk::ITKImageMaskAccessor::ITKMaskImageType::ConstPointer itkImage)
{
    itk::BinaryThresholdImageFilter< itk::Image<rttb::DoseTypeGy, 3>, itk::Image<rttb::DoseTypeGy, 3> >::Pointer filter = 
        itk::BinaryThresholdImageFilter< itk::Image<rttb::DoseTypeGy, 3>, itk::Image<rttb::DoseTypeGy, 3> >::New();

    filter->SetInput(itkImage);
    filter->SetLowerThreshold(0.5);
    filter->SetUpperThreshold(1.0);
    filter->SetInsideValue(1.0);

    filter->Update();

    return filter->GetOutput();
}

void rttb::apps::voxelizerTool::writeITKImageToFile(io::itk::ITKImageMaskAccessor::ITKMaskImageType::ConstPointer& itkImage,
    const std::string& outputfilename, bool useCompression)
{
    itk::ImageFileWriter< itk::Image<rttb::DoseTypeGy, 3> >::Pointer writer = 
        itk::ImageFileWriter< itk::Image<rttb::DoseTypeGy, 3> >::New();
    writer->SetFileName(outputfilename);
    writer->SetUseCompression(useCompression);
    writer->SetInput(itkImage);

    writer->Update();
}

void rttb::apps::voxelizerTool::processData(rttb::apps::voxelizerTool::ApplicationData& appData) {
    if (appData._struct->getNumberOfStructures()>0)
    {
        std::vector<core::MaskAccessorInterface::MaskAccessorPointer> maskVector;            

        if (appData._addStructures)
        {
            for (size_t i=0; i<appData._struct->getNumberOfStructures(); i++ )
            {
              std::cout << "creating mask #" << i << "...";
                maskVector.push_back(createMask(appData._dose, appData._struct->getStructure(i),
                    !appData._noStrictVoxelization));
                std::cout << "done" << std::endl;
            }
            std::cout << "writing mask to file...";
            writeMaskToFile(maskVector, appData._outputFilename, appData._binaryVoxelization);
            std::cout << "done" << std::endl;
        }
        else
        {
          //default behavior: read only first struct that matches the regex
            unsigned int maxIterationCount = 1;

            //only if specified: read all structs that matches the regex
            if (appData._multipleStructs)
            {
                maxIterationCount = appData._struct->getNumberOfStructures();
            }

            for (size_t i = 0; i<maxIterationCount; i++)
            {
                std::cout << "creating mask #" << i << "...";
                auto currentMask = createMask(appData._dose, appData._struct->getStructure(i),
                    !appData._noStrictVoxelization);
                std::cout << "done" << std::endl;
                std::string labelOfInterest = appData._struct->getStructure(i)->getLabel();
                removeSpecialCharacters(labelOfInterest);

                std::string outputName = appData._outputFilename;

                if (appData._multipleStructs)
                {
                    std::string fileName = getFilenameWithoutEnding(
                        appData._outputFilename);
                    std::string fileEnding = getFileEnding(appData._outputFilename);
                    outputName = fileName + "_" + labelOfInterest + fileEnding;
                }
                std::vector<rttb::core::MaskAccessorInterface::MaskAccessorPointer> currenMaskVector{ currentMask };
                std::cout << "writing mask #" << i << " to file...";
                writeMaskToFile(currenMaskVector, outputName, appData._binaryVoxelization);
                std::cout << "done" << std::endl;
            }
        }
    }
    else
    {
        std::cout << "No struct found" << std::endl;
    }
}