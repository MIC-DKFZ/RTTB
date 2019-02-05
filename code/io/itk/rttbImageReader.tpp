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

#ifndef __RTTB_IMAGE_READER_TPP
#define __RTTB_IMAGE_READER_TPP

#include "rttbImageReader.h"
#include "rttbFileDispatch.h"

#include "RTToolboxConfigure.h"

#ifdef RTTB_DISABLE_ITK_IO_FACTORY_AUTO_REGISTER
#undef ITK_IO_FACTORY_REGISTER_MANAGER
#endif

#include "itkImageFileReader.h"

#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkGDCMSeriesFileNames.h"

#include "itkRescaleIntensityImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkFixedArray.h"
#include "itksys/SystemTools.hxx"

#include <iostream>
#include <locale>

namespace rttb
{
	namespace io
	{
		namespace itk
		{

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			void
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			load2D()
			{
				using ImageReaderType = ::itk::ImageFileReader< InputImageType  >;
				typedef ::itk::RescaleIntensityImageFilter< InputImageType, InputImageType > RescaleFilterType;
				typedef ::itk::CastImageFilter< InputImageType, OutputImageType > CastFilterType;

				typename CastFilterType::Pointer  imageCaster =  CastFilterType::New();
				typename ImageReaderType::Pointer imageReader  = ImageReaderType::New();
				typename RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();

				rescaleFilter->SetOutputMinimum(static_cast<TInputPixel>(_rescaleMin));
				rescaleFilter->SetOutputMaximum(static_cast<TInputPixel>(_rescaleMax));

				imageReader->SetFileName(_fileName.c_str());
				rescaleFilter->SetInput(imageReader->GetOutput());

				if (_rescaleImage)
				{
					imageCaster->SetInput(rescaleFilter->GetOutput());
				}
				else
				{
					imageCaster->SetInput(imageReader->GetOutput());
				}

				_spImage = imageCaster->GetOutput();

				imageCaster->Update();

				_dictionaryArray.clear();
				_dictionaryArray.push_back(imageReader->GetImageIO()->GetMetaDataDictionary());

				_upToDate = true;
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			const typename ImageReader<TInputPixel, TOutputPixel, iDimension>::MetaDataDictionaryArrayType&
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			getMetaDictionaryArray()
			{
				return _dictionaryArray;
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			void
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			copyMetaDictionaryArray(const ITKMetaDataDictionaryArray* fromArray,
			                        MetaDataDictionaryArrayType& toArray)
			{
				toArray.clear();

				auto itr = fromArray->begin();
				auto end = fromArray->end();

				while (itr != end)
				{
					toArray.push_back(*(*itr));
					++itr;
				}
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			typename ::itk::ImageSource<typename ImageReader<TInputPixel, TOutputPixel, iDimension>::InputImageType>::Pointer
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			prepareNumericSource() const
			{
				//mumeric series image reader
				using SeriesReaderType = ::itk::ImageSeriesReader<InputImageType>;
				using NamesType = ::itk::NumericSeriesFileNames;

				typename SeriesReaderType::Pointer  seriesReader  = SeriesReaderType::New();
				NamesType::Pointer names = NamesType::New();

				names->SetStartIndex(1);
				names->SetEndIndex(_upperSeriesLimit);
				names->SetSeriesFormat(_fileName.c_str());

				seriesReader->SetFileNames(names->GetFileNames());

				if (seriesReader->GetFileNames().size() == 0)
				{
					throw ::itk::ExceptionObject("Image reader is not correctly configured. Preparing a series reading of a numeric source no(!) files were found.");
				}

				typename ::itk::ImageSource<typename ImageReader<TInputPixel, TOutputPixel, iDimension>::InputImageType>::Pointer
				genericReader = seriesReader.GetPointer();
				return genericReader;
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			typename ::itk::ImageSource<typename ImageReader<TInputPixel, TOutputPixel, iDimension>::InputImageType>::Pointer
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			prepareDICOMSource() const
			{
				//ITK > v4.3.x  removed old DICOMSeriesFileNames. Thus currently only support GDCM as source by default
				return prepareGDCMSource();
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			typename ::itk::ImageSource<typename ImageReader<TInputPixel, TOutputPixel, iDimension>::InputImageType>::Pointer
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			prepareGDCMSource() const
			{
				core::FileDispatch dispatch(_fileName);

				FileNameString  dir = dispatch.getPath();
				FileNameString  strippedFileName = dispatch.getFullName();

				using NamesGeneratorType = ::itk::GDCMSeriesFileNames;
				NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
				nameGenerator->SetInputDirectory(dir);
				nameGenerator->SetUseSeriesDetails(true);

				::itk::FilenamesContainer fileNames;

				if (strippedFileName.empty())
				{
					std::cout << "No file name specified. Use first DICOM series found in directory." << std::endl;
					fileNames = nameGenerator->GetInputFileNames();
				}
				else
				{
					::itk::SerieUIDContainer seriesUIDs = nameGenerator->GetSeriesUIDs();

					std::cout << "Checking found DICOM series." << std::endl;

					//check the found series for the filename to pick the right series correlated to the passed filename
					while (seriesUIDs.size() > 0)
					{
						fileNames = nameGenerator->GetFileNames(seriesUIDs.back());
						std::cout << "Checking series: " << seriesUIDs.back() << " (file count: " <<
						          fileNames.size() << ")" << std::endl;
						seriesUIDs.pop_back();

						for (::itk::SerieUIDContainer::const_iterator pos = fileNames.begin(); pos != fileNames.end();
						     ++pos)
						{
							if (pos->find(strippedFileName) != FileNameString::npos)
							{
								//this series containes the passed filename ->
								//we have the right block of files -> we are done.
								std::cout << "Found right series!" << std::endl;
								seriesUIDs.clear();
								break;
							}
						}
					}
				}

				using SeriesReaderType = ::itk::ImageSeriesReader<InputImageType>;
				typename SeriesReaderType::Pointer  seriesReader  = SeriesReaderType::New();

				seriesReader->SetFileNames(fileNames);

				if (seriesReader->GetFileNames().size() == 0)
				{
					throw ::itk::ExceptionObject("Image reader is not correctly configured. Preparing a series reading of a DICOM source no(!) dicom files were found. search location: "
					                             + _fileName);
				}

				typename ::itk::ImageSource<typename ImageReader<TInputPixel, TOutputPixel, iDimension>::InputImageType>::Pointer
				genericReader = seriesReader.GetPointer();
				return genericReader;
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			typename ::itk::ImageSource<typename ImageReader<TInputPixel, TOutputPixel, iDimension>::InputImageType>::Pointer
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			prepareNormalSource() const
			{
				//Normal image reader (no series read style)
				using ImageReaderType = ::itk::ImageFileReader< InputImageType  >;
				typename ImageReaderType::Pointer  imageReader  = ImageReaderType::New();
				imageReader->SetFileName(_fileName.c_str());

				typename ::itk::ImageSource<typename ImageReader<TInputPixel, TOutputPixel, iDimension>::InputImageType>::Pointer
				genericReader = imageReader.GetPointer();
				return genericReader;
			};


			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			void
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			load3D()
			{
				core::FileDispatch dispatch(_fileName);

				FileNameString	sTemp = dispatch.getExtension();

				//Convert to lowercase
				for (char & spos : sTemp)
				{
					spos = std::tolower(spos, std::locale(""));
				}

				typedef ::itk::RescaleIntensityImageFilter< InputImageType, InputImageType > RescaleFilterType;
				typedef ::itk::CastImageFilter< InputImageType, OutputImageType > CastFilterType;
				typename CastFilterType::Pointer  imageCaster =  CastFilterType::New();
				typename RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
				typename ::itk::ImageSource<InputImageType>::Pointer spReader;

				rescaleFilter->SetOutputMinimum(static_cast<TInputPixel>(_rescaleMin));
				rescaleFilter->SetOutputMaximum(static_cast<TInputPixel>(_rescaleMax));

				if (_seriesReadStyle == ImageSeriesReadStyle::Numeric)
				{
					spReader = prepareNumericSource();
				}
				else if (_seriesReadStyle == ImageSeriesReadStyle::Dicom)
				{
					spReader = prepareDICOMSource();
				}
				else if (_seriesReadStyle == ImageSeriesReadStyle::GDCM)
				{
					spReader = prepareGDCMSource();
				}
				else if (_seriesReadStyle == ImageSeriesReadStyle::Default)
				{
					bool isDir = itksys::SystemTools::FileIsDirectory(_fileName.c_str());

					if (isDir || sTemp == ".dcm" || sTemp == ".ima")
					{
						spReader = prepareDICOMSource();
					}
					else
					{
						spReader = prepareNormalSource();
					}
				}
				else
				{
					//style is none
					spReader = prepareNormalSource();
				}

				if (_rescaleImage)
				{
					rescaleFilter->SetInput(spReader->GetOutput());
					imageCaster->SetInput(rescaleFilter->GetOutput());
				}
				else
				{
					imageCaster->SetInput(spReader->GetOutput());
				}

				imageCaster->Update();

				_spImage = imageCaster->GetOutput();

				using ImageReaderType = ::itk::ImageFileReader< InputImageType  >;
				using ImageSeriesReaderType = ::itk::ImageSeriesReader<InputImageType>;
				auto* pFileReader = dynamic_cast<ImageReaderType*>(spReader.GetPointer());
				auto* pSeriesReader = dynamic_cast<ImageSeriesReaderType*>(spReader.GetPointer());

				if (pFileReader)
				{
					_dictionaryArray.clear();
					_dictionaryArray.push_back(pFileReader->GetImageIO()->GetMetaDataDictionary());
				}
				else if (pSeriesReader)
				{
					copyMetaDictionaryArray(pSeriesReader->GetMetaDataDictionaryArray(), _dictionaryArray);
				}
				else
				{
					throw ::itk::ExceptionObject("Image reader is not valid. Internal reader seams not to be itk::ImageFileReader or itk::ImageSeriesReader.");
				}

				_upToDate = true;
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			const FileNameString&
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			getFileName() const
			{
				return _fileName;
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			void
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			setFileName(const FileNameString& fileName)
			{
				if (fileName != _fileName)
				{
					_upToDate = false;
					_fileName = fileName;
				}
			}

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			const typename ImageReader<TInputPixel, TOutputPixel, iDimension>::RescaleValueType&
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			getRescaleMinimum() const
			{
				return _rescaleMin;
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			void
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			setRescaleMinimum(const RescaleValueType& dRescaleMin)
			{
				if (dRescaleMin != _rescaleMin)
				{
					_upToDate = false;
					_rescaleMin = dRescaleMin;
				};
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			const typename ImageReader<TInputPixel, TOutputPixel, iDimension>::RescaleValueType&
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			getRescaleMaximum() const
			{
				return _rescaleMax;
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			void
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			setRescaleMaximum(const RescaleValueType& dRescaleMax)
			{
				if (dRescaleMax != _rescaleMax)
				{
					_upToDate = false;
					_rescaleMax = dRescaleMax;
				};
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			const bool
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			getRescaleImage() const
			{
				return _rescaleImage;
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			void
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			setRescaleImage(const bool rescaleImage)
			{
				if (rescaleImage != _rescaleImage)
				{
					_upToDate = false;
					_rescaleImage = rescaleImage;
				};
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			const unsigned int
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			getUpperSeriesLimit() const
			{
				return _upperSeriesLimit;
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			void
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			setUpperSeriesLimit(const unsigned int upperLimit)
			{
				if (upperLimit != _upperSeriesLimit)
				{
					_upToDate = false;
					_upperSeriesLimit = upperLimit;
				};
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			const ImageSeriesReadStyle::Type
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			getSeriesReadStyle() const
			{
				return _seriesReadStyle;
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			void
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			setSeriesReadStyle(ImageSeriesReadStyle::Type readStyle)
			{
				if (readStyle != _seriesReadStyle)
				{
					_upToDate = false;
					_seriesReadStyle = readStyle;
				};
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			typename ImageReader<TInputPixel, TOutputPixel, iDimension>::OutputImageType*
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			GetOutput()
			{
				if (!_upToDate)
				{
					switch (OutputImageType::GetImageDimension())
					{
						case 2:
							load2D();
							break;

						case 3:
							load3D();
							break;

						default:
							throw ::itk::ExceptionObject("Image reader only accepts 2 or 3 dimensional images.");
					};
				};

				return _spImage;
			};


			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			ImageReader()
			{
				_fileName = "";
				_upperSeriesLimit = 255;
				_upToDate = false;
				_rescaleImage = false;
				_rescaleMax = 255;
				_rescaleMin = 0;
				_seriesReadStyle = ImageSeriesReadStyle::Default;
			};

			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			ImageReader<TInputPixel, TOutputPixel, iDimension>::
			~ImageReader()
			= default;


			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			typename ImageReader<TInputPixel, TOutputPixel, iDimension>::OutputImageType::Pointer readImage(
			    const FileNameString& fileName,
			    ImageSeriesReadStyle::Type readStyle,
			    bool rescaleImage,
			    typename ImageReader<TInputPixel, TOutputPixel, iDimension>::RescaleValueType rescaleMin,
			    typename ImageReader<TInputPixel, TOutputPixel, iDimension>::RescaleValueType rescaleMax,
			    unsigned int upperNumericSeriesLimit,
			    typename ImageReader<TInputPixel, TOutputPixel, iDimension>::MetaDataDictionaryArrayType*
			    pLoadedDictArray)
			{
				ImageReader<TInputPixel, TOutputPixel, iDimension> reader;

				reader.setFileName(fileName);
				reader.setSeriesReadStyle(readStyle);
				reader.setRescaleImage(rescaleImage);
				reader.setRescaleMaximum(rescaleMax);
				reader.setRescaleMinimum(rescaleMin);
				reader.setUpperSeriesLimit(upperNumericSeriesLimit);

				typename ImageReader<TInputPixel, TOutputPixel, iDimension>::OutputImageType::Pointer spResult =
				    reader.GetOutput();

				if (pLoadedDictArray)
				{
					*pLoadedDictArray = reader.getMetaDictionaryArray();
				};

				return spResult;
			};

		}
	}
}
#endif
