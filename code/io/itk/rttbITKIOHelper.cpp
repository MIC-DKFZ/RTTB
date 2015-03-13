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


#include "rttbITKIOHelper.h"
#include "rttbException.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"


namespace rttb
{
	namespace io
	{
		namespace itk
		{

			ITKDoseImageType::Pointer readITKDoubleImage(FileNameType aITKImageFile){
				ITKDoseImageType::Pointer itkDoubleImage;

				GenericImageReader::Pointer spReader = GenericImageReader::New();

				spReader->setFileName(aITKImageFile);

				GenericImageReader::GenericOutputImageType::Pointer itkGenericImage;

				ITKDoseImageType::ConstPointer itkDoubleImageConst;

				try
				{
					unsigned int loadedDimensions;
					GenericImageReader::LoadedPixelType loadedPixelType;
					GenericImageReader::LoadedComponentType loadedComponentType;
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
						itkDoubleImage = dynamic_cast<ITKDoseImageType*>(itkGenericImage.GetPointer());
					}
					else
					{
						itkDoubleImage = handleGenericImage(itkGenericImage, loadedComponentType);
					}

					if (itkDoubleImage.IsNull())
					{
						throw core::InvalidDoseException("Error!!! unable to load input image. File is not existing or has an unsupported format.");
					}
				}
				catch (::itk::ExceptionObject& e)
				{
					std::cerr << "Error!!!" << std::endl;
					std::cerr << e << std::endl;
					throw rttb::core::InvalidDoseException(e.GetDescription());
				}

				return itkDoubleImage;
			}

			ITKDoseImageType::Pointer handleGenericImage(
				GenericImageReader::GenericOutputImageType* itkGenericImage,
				::itk::ImageIOBase::IOComponentType& loadedComponentType)
			{
				ITKDoseImageType::Pointer itkDoubleImage;

				switch (loadedComponentType)
				{
				case ::itk::ImageIOBase::UCHAR:
					{
						itkDoubleImage = doCasting<unsigned char>(itkGenericImage);
						break;
					}

				case ::itk::ImageIOBase::CHAR:
					{
						itkDoubleImage = doCasting<char>(itkGenericImage);
						break;
					}

				case ::itk::ImageIOBase::USHORT:
					{
						itkDoubleImage = doCasting<unsigned short>(itkGenericImage);
						break;
					}

				case ::itk::ImageIOBase::SHORT:
					{
						itkDoubleImage = doCasting<short>(itkGenericImage);
						break;
					}

				case ::itk::ImageIOBase::UINT:
					{
						itkDoubleImage = doCasting<unsigned int>(itkGenericImage);
						break;
					}

				case ::itk::ImageIOBase::INT:
					{
						itkDoubleImage = doCasting<int>(itkGenericImage);
						break;
					}

				case ::itk::ImageIOBase::ULONG:
					{
						itkDoubleImage = doCasting<unsigned long>(itkGenericImage);
						break;
					}

				case ::itk::ImageIOBase::LONG:
					{
						itkDoubleImage = doCasting<long>(itkGenericImage);
						break;
					}

				case ::itk::ImageIOBase::FLOAT:
					{
						itkDoubleImage = doCasting<float>(itkGenericImage);
						break;
					}

				case ::itk::ImageIOBase::DOUBLE:
					{
						itkDoubleImage = doCasting<double>(itkGenericImage);
						break;
					}

				default:
					{
						throw core::InvalidParameterException("image type unknown");
					}
				}
				return itkDoubleImage;
			}


		}//end namespace itk
	}//end namespace io
}//end namespace rttb

