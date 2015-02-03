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
// @version $Revision: 793 $ (last changed revision)
// @date    $Date: 2014-10-10 10:24:45 +0200 (Fr, 10 Okt 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "rttbITKImageFileMaskAccessorGenerator.h"
#include "rttbException.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"


namespace rttb
{
	namespace io
	{
		namespace mask
		{
			ITKImageFileMaskAccessorGenerator::~ITKImageFileMaskAccessorGenerator()
			{

			}

			ITKImageFileMaskAccessorGenerator::ITKImageFileMaskAccessorGenerator(const FileNameType& fileName)
			{
				_itkMaskFileName = fileName;
			}

			rttb::core::MaskAccessorGeneratorBase::MaskAccessorPointer
			ITKImageFileMaskAccessorGenerator::generateMaskAccessor()
			{
				io::itk::GenericImageReader::Pointer spReader = io::itk::GenericImageReader::New();

				spReader->setFileName(_itkMaskFileName);

				io::itk::GenericImageReader::GenericOutputImageType::Pointer itkGenericImage;

				ITKImageMaskAccessor::ITKMaskImageType::ConstPointer itkDoubleImageConst;

				try
				{
					unsigned int loadedDimensions;
					io::itk::GenericImageReader::LoadedPixelType loadedPixelType;
					io::itk::GenericImageReader::LoadedComponentType loadedComponentType;
					itkGenericImage = spReader->GetOutput(loadedDimensions, loadedPixelType, loadedComponentType);

					if (loadedDimensions != 3)
					{
						throw core::InvalidParameterException("image dimensions != 3. Only dim = 3 supported.");
					}

					if (loadedPixelType != ::itk::ImageIOBase::SCALAR)
					{
						throw core::InvalidParameterException("image component type != SCALAR. Only SCALAR supported.");
					}

					if (loadedComponentType == ::itk::ImageIOBase::DOUBLE)
					{
						_itkDoubleImage = dynamic_cast<ITKImageMaskAccessor::ITKMaskImageType*>(itkGenericImage.GetPointer());
					}
					else
					{
						handleGenericImage(itkGenericImage, loadedComponentType);
					}

					if (_itkDoubleImage.IsNull())
					{
						throw core::InvalidDoseException("Error!!! unable to load input image. File is not existing or has an unsupported format.");
						return core::MaskAccessorGeneratorInterface::MaskAccessorPointer();
					}
				}
				catch (::itk::ExceptionObject& e)
				{
					std::cerr << "Error!!!" << std::endl;
					std::cerr << e << std::endl;
					throw rttb::core::InvalidDoseException(e.GetDescription());
					return core::MaskAccessorGeneratorInterface::MaskAccessorPointer();
				}

				_maskAccessor = boost::make_shared<ITKImageMaskAccessor>(_itkDoubleImage.GetPointer());
				return _maskAccessor;
			}

			void ITKImageFileMaskAccessorGenerator::handleGenericImage(
			    io::itk::GenericImageReader::GenericOutputImageType* itkGenericImage,
			    ::itk::ImageIOBase::IOComponentType& loadedComponentType)
			{
				switch (loadedComponentType)
				{
					case ::itk::ImageIOBase::UCHAR:
					{
						doCasting<unsigned char>(itkGenericImage);
						break;
					}

					case ::itk::ImageIOBase::CHAR:
					{
						doCasting<char>(itkGenericImage);
						break;
					}

					case ::itk::ImageIOBase::USHORT:
					{
						doCasting<unsigned short>(itkGenericImage);
						break;
					}

					case ::itk::ImageIOBase::SHORT:
					{
						doCasting<short>(itkGenericImage);
						break;
					}

					case ::itk::ImageIOBase::UINT:
					{
						doCasting<unsigned int>(itkGenericImage);
						break;
					}

					case ::itk::ImageIOBase::INT:
					{
						doCasting<int>(itkGenericImage);
						break;
					}

					case ::itk::ImageIOBase::ULONG:
					{
						doCasting<unsigned long>(itkGenericImage);
						break;
					}

					case ::itk::ImageIOBase::LONG:
					{
						doCasting<long>(itkGenericImage);
						break;
					}

					case ::itk::ImageIOBase::FLOAT:
					{
						doCasting<float>(itkGenericImage);
						break;
					}

					case ::itk::ImageIOBase::DOUBLE:
					{
						doCasting<double>(itkGenericImage);
						break;
					}

					default:
					{
						throw core::InvalidParameterException("image type unknown");
					}
				}
			}
		}//end namespace itk
	}//end namespace io
}//end namespace rttb

