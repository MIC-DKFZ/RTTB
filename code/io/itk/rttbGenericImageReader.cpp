// -----------------------------------------------------------------------
// MatchPoint - DKFZ translational registration framework
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See mapCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/MatchPoint/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
// Subversion HeadURL: $HeadURL: https://svn/sbr/Sources/SBR-Projects/MatchPoint/trunk/Code/IO/source/mapGenericImageReader.cpp $
*/

#include "rttbGenericImageReader.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{


			void GenericImageReader::load()
			{
				_spImage = nullptr;

				FileNameString probeFileName = this->_fileName;

				if (this->_seriesReadStyle == ImageSeriesReadStyle::Numeric)
				{
					using NamesType = ::itk::NumericSeriesFileNames;

					NamesType::Pointer names = NamesType::New();

					names->SetStartIndex(1);
					names->SetEndIndex(1);
					names->SetSeriesFormat(this->_fileName.c_str());

					probeFileName = names->GetFileNames()[0];
				}

				::itk::ImageIOBase::Pointer imageIO =
				    ::itk::ImageIOFactory::CreateImageIO(probeFileName.c_str(),
				            ::itk::ImageIOFactory::ReadMode);

				if (!imageIO)
				{
					throw ::itk::ExceptionObject("No ImageIO found for given file. Please check if the file exists and has a supported format. File:"
					                             + probeFileName);
				}

				// Now that we found the appropriate ImageIO class, ask it to
				// read the meta data from the image file.
				imageIO->SetFileName(probeFileName.c_str());
				imageIO->ReadImageInformation();

				this->_loadedComponentType = imageIO->GetComponentType();
				this->_loadedPixelType = imageIO->GetPixelType();

				if (this->_loadedPixelType == ::itk::ImageIOBase::RGB && imageIO->GetNumberOfComponents() == 1)
				{
					//if only one channel per pixel handle as scalar as long as RGB etc. is not supported
					this->_loadedPixelType = ::itk::ImageIOBase::SCALAR;
				}

				this->_loadedComponentTypeStr = imageIO->GetComponentTypeAsString(this->_loadedComponentType);
				this->_loadedPixelTypeStr = imageIO->GetPixelTypeAsString(this->_loadedPixelType);
				this->_loadedDimensions = imageIO->GetNumberOfDimensions();

				if (this->_seriesReadStyle == ImageSeriesReadStyle::Numeric &&
				    this->_loadedDimensions == 2)
				{
					this->_loadedDimensions = 3; //it is a stack of 2D images -> 3D
				}

				if (this->_loadedDimensions < 2 || this->_loadedDimensions > 3)
				{
					throw ::itk::ExceptionObject("The file uses a number of dimensions that is not supported in this application. Only dim<=3 supported ");
				}

				switch (_loadedPixelType)
				{
					case ::itk::ImageIOBase::SCALAR:
					{
						if (this->_loadedDimensions == 2)
						{
							loadScalar<2>();
						}
						else
						{
							loadScalar<3>();
						}

						break;
					}

					default:
					{
						throw ::itk::ExceptionObject("The file uses a pixel type that is not supported in this application. Only SCALAR pixel type supported ");
					}
				}

				_upToDate = true;
			};


			template <unsigned int IDimension>
			void
			GenericImageReader::
			loadScalar()
			{
				// Use the pixel type to instantiate the appropriate reader
				switch (this->_loadedComponentType)
				{
					case ::itk::ImageIOBase::UCHAR:
					{
						this->_spImage = readImage<unsigned char, unsigned char, IDimension>(_fileName, _seriesReadStyle,
						                 false, 0, 0, _upperSeriesLimit, &_dictionaryArray);
						break;
					}

					case ::itk::ImageIOBase::CHAR:
					{
						this->_spImage = readImage<char, char, IDimension>(_fileName, _seriesReadStyle, false, 0, 0,
						                 _upperSeriesLimit, &_dictionaryArray);
						break;
					}

					case ::itk::ImageIOBase::USHORT:
					{
						this->_spImage = readImage<unsigned short, unsigned short, IDimension>(_fileName, _seriesReadStyle,
						                 false, 0, 0, _upperSeriesLimit, &_dictionaryArray);
						break;
					}

					case ::itk::ImageIOBase::SHORT:
					{
						this->_spImage = readImage<short, short, IDimension>(_fileName, _seriesReadStyle, false, 0, 0,
						                 _upperSeriesLimit, &_dictionaryArray);
						break;
					}

					case ::itk::ImageIOBase::UINT:
					{
						this->_spImage = readImage<unsigned int, unsigned int, IDimension>(_fileName, _seriesReadStyle,
						                 false, 0, 0, _upperSeriesLimit, &_dictionaryArray);
						break;
					}

					case ::itk::ImageIOBase::INT:
					{
						this->_spImage = readImage<int, int, IDimension>(_fileName, _seriesReadStyle, false, 0, 0,
						                 _upperSeriesLimit, &_dictionaryArray);
						break;
					}

					case ::itk::ImageIOBase::ULONG:
					{
						this->_spImage = readImage<unsigned long, unsigned long, IDimension>(_fileName, _seriesReadStyle,
						                 false, 0, 0, _upperSeriesLimit, &_dictionaryArray);
						break;
					}

					case ::itk::ImageIOBase::LONG:
					{
						this->_spImage = readImage<long, long, IDimension>(_fileName, _seriesReadStyle, false, 0, 0,
						                 _upperSeriesLimit, &_dictionaryArray);
						break;
					}

					case ::itk::ImageIOBase::FLOAT:
					{
						this->_spImage = readImage<float, float, IDimension>(_fileName, _seriesReadStyle, false, 0, 0,
						                 _upperSeriesLimit, &_dictionaryArray);
						break;
					}

					case ::itk::ImageIOBase::DOUBLE:
					{
						this->_spImage = readImage<double, double, IDimension>(_fileName, _seriesReadStyle, false, 0, 0,
						                 _upperSeriesLimit, &_dictionaryArray);
						break;
					}

					default:
					{
						throw ::itk::ExceptionObject("The file uses a pixel component type that is not supported in this application. ComponentType: "
						                             + this->_loadedComponentTypeStr);
					}
				}
			};

			const FileNameString&
			GenericImageReader::
			getFileName() const
			{
				return _fileName;
			};


			void
			GenericImageReader::
			setFileName(const FileNameString& fileName)
			{
				if (fileName != _fileName)
				{
					_upToDate = false;
					_fileName = fileName;
				}
			}


			const unsigned int
			GenericImageReader::
			getUpperSeriesLimit() const
			{
				return _upperSeriesLimit;
			};


			void
			GenericImageReader::
			setUpperSeriesLimit(const unsigned int upperLimit)
			{
				if (upperLimit != _upperSeriesLimit)
				{
					_upToDate = false;
					_upperSeriesLimit = upperLimit;
				};
			};


			const ImageSeriesReadStyle::Type
			GenericImageReader::
			getSeriesReadStyle() const
			{
				return _seriesReadStyle;
			};


			void
			GenericImageReader::
			setSeriesReadStyle(ImageSeriesReadStyle::Type readStyle)
			{
				if (readStyle != _seriesReadStyle)
				{
					_upToDate = false;
					_seriesReadStyle = readStyle;
				};
			};


			GenericImageReader::GenericOutputImageType*
			GenericImageReader::
			GetOutput(unsigned int& loadedDimensions, LoadedPixelType& loadedPixelType,
			          LoadedComponentType& loadedComponentType)
			{
				if (!_upToDate)
				{
					load();
					loadedPixelType = _loadedPixelType;
					loadedComponentType = _loadedComponentType;
					loadedDimensions = _loadedDimensions;
				};

				return _spImage;
			};



			GenericImageReader::
			GenericImageReader() : _fileName()
				
			{
			};


			GenericImageReader::
			~GenericImageReader()
			= default;

			const GenericImageReader::MetaDataDictionaryArrayType&
			GenericImageReader::
			getMetaDictionaryArray()
			{
				return _dictionaryArray;
			};

		}//end namespace itk
	}//end namespace io
}//end namespace rttb
