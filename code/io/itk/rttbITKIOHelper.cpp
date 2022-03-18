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

			ITKImageType::Pointer readITKDoubleImage(FileNameType aITKImageFile, bool isDicom)
			{
				ITKImageType::Pointer itkDoubleImage;

				GenericImageReader::Pointer spReader = GenericImageReader::New();

				if (isDicom) {
					spReader->setSeriesReadStyle(ImageSeriesReadStyle::Type::Dicom);
				}
				spReader->setFileName(aITKImageFile);

				GenericImageReader::GenericOutputImageType::Pointer itkGenericImage;

				ITKImageType::ConstPointer itkDoubleImageConst;

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

					if (loadedPixelType != ::itk::IOPixelEnum::SCALAR)
					{
						throw core::InvalidParameterException("image component type != SCALAR. Only SCALAR supported.");
					}

					if (loadedComponentType == ::itk::IOComponentEnum::DOUBLE)
					{
						itkDoubleImage = dynamic_cast<ITKImageType*>(itkGenericImage.GetPointer());
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

			ITKImageType::Pointer handleGenericImage(
			    GenericImageReader::GenericOutputImageType* itkGenericImage,
			    ::itk::IOComponentEnum& loadedComponentType)
			{
				ITKImageType::Pointer itkDoubleImage;

				switch (loadedComponentType)
				{
					case ::itk::IOComponentEnum::UCHAR:
					{
						itkDoubleImage = doCasting<unsigned char>(itkGenericImage);
						break;
					}

					case ::itk::IOComponentEnum::CHAR:
					{
						itkDoubleImage = doCasting<char>(itkGenericImage);
						break;
					}

					case ::itk::IOComponentEnum::USHORT:
					{
						itkDoubleImage = doCasting<unsigned short>(itkGenericImage);
						break;
					}

					case ::itk::IOComponentEnum::SHORT:
					{
						itkDoubleImage = doCasting<short>(itkGenericImage);
						break;
					}

					case ::itk::IOComponentEnum::UINT:
					{
						itkDoubleImage = doCasting<unsigned int>(itkGenericImage);
						break;
					}

					case ::itk::IOComponentEnum::INT:
					{
						itkDoubleImage = doCasting<int>(itkGenericImage);
						break;
					}

					case ::itk::IOComponentEnum::ULONG:
					{
						itkDoubleImage = doCasting<unsigned long>(itkGenericImage);
						break;
					}

					case ::itk::IOComponentEnum::LONG:
					{
						itkDoubleImage = doCasting<long>(itkGenericImage);
						break;
					}

					case ::itk::IOComponentEnum::FLOAT:
					{
						itkDoubleImage = doCasting<float>(itkGenericImage);
						break;
					}

					case ::itk::IOComponentEnum::DOUBLE:
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

