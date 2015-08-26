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
#include "rttbMaskWriter.h"

#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkAddImageFilter.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizer
		{
			MaskWriter::MaskWriter(std::vector<MaskAccessorPointer> maskPointer,
			                       bool voxelization) : _MaskPointerList(maskPointer), _booleanvoxelization(voxelization)
			{
			}

			void MaskWriter::writeMaskToFile(const std::string& outputFileName)
			{
				if (!_MaskPointerList.empty())
				{
					ITKImageTypeConstPointer itkImage;

					if (_MaskPointerList.size() > 1)
					{
						itkImage = AddMultipleStructsToImage();
					}
					else
					{
						io::itk::ITKImageMaskAccessorConverter maskAccessorConverter(_MaskPointerList.at(0));
						maskAccessorConverter.process();
						itkImage = maskAccessorConverter.getITKImage();
					}

					if (_booleanvoxelization)
					{
						itkImage = ApplyThresholdFilter(itkImage);
					}

					WriteITKImageToFile(itkImage, outputFileName);
				}
			}

			MaskWriter::ITKImageTypeConstPointer MaskWriter::AddMultipleStructsToImage()
			{
				std::vector<ITKImageTypePointer> listOfITKImages;

				for (int i = 0; i < _MaskPointerList.size(); i++)
				{
					io::itk::ITKImageMaskAccessorConverter maskAccessorConverter(_MaskPointerList.at(i));
					maskAccessorConverter.process();
					listOfITKImages.push_back(maskAccessorConverter.getITKImage());
				}

				typedef itk::AddImageFilter <ITKMaskImageType, ITKMaskImageType>AddImageFilterType;

				AddImageFilterType::Pointer addFilter = AddImageFilterType::New();
				ITKImageTypePointer filterResult;

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

					try
					{
						addFilter->Update();
					}
					catch (itk::ExceptionObject& err)
					{
						std::cerr << "ExceptionObject caught !" << std::endl;
						std::cerr << err << std::endl;
					}

					filterResult = addFilter->GetOutput();
				}

				return filterResult;
			}

			MaskWriter::ITKImageTypeConstPointer MaskWriter::ApplyThresholdFilter(
			    ITKImageTypeConstPointer itkImage)
			{

				typedef itk::BinaryThresholdImageFilter< ITKMaskImageType, ITKMaskImageType >  FilterType;
				FilterType::Pointer filter = FilterType::New();

				filter->SetInput(itkImage);
				filter->SetLowerThreshold(0.5);
				filter->SetUpperThreshold(1.0);
				filter->SetOutsideValue(0.0);
				filter->SetInsideValue(1.0);

				try
				{
					filter->Update();
				}
				catch (itk::ExceptionObject& err)
				{
					std::cerr << "ExceptionObject caught !" << std::endl;
					std::cerr << err << std::endl;
				}

				return filter->GetOutput();
			}

			void MaskWriter::WriteITKImageToFile(ITKImageTypeConstPointer itkImage,
			                                     const std::string& outputfilename)
			{
				typedef  itk::ImageFileWriter< ITKMaskImageType > WriterType;
				WriterType::Pointer writer = WriterType::New();
				writer->SetFileName(outputfilename);

				writer->SetInput(itkImage);

				try
				{
					writer->Update();
				}
				catch (itk::ExceptionObject& err)
				{
					std::cerr << "ExceptionObject caught !" << std::endl;
					std::cerr << err << std::endl;

				}
			}
		}
	}
}