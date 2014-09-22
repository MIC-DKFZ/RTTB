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
// Subversion HeadURL: $HeadURL: https://svn/sbr/Sources/SBR-Projects/MatchPoint/trunk/Code/IO/include/mapImageReader.h $
*/


#ifndef __RTTB_IMAGE_READER_H
#define __RTTB_IMAGE_READER_H

#include "itkImage.h"
#include "itkImageSource.h"

namespace rttb
{
	namespace io
	{
		namespace itk {
			struct ImageSeriesReadStyle
			{
				enum Type
				{
					Default = 0, //* - Depending on the file extension (DICOM images (*.dcm, *.ima): Dicom; others: None; No image file: Dicom)
					None = 1, //* - No series reading, only the specified file
					Dicom = 2, //* - Use series reader and DCMTKSeriesFileNames
					Numeric = 3, //* - Use series reader and NumericSeriesFileNames
					GDCM = 4 //* - Use series reader and GDCMSeriesFileNames
				};
			};


			/** @class ImageReader
			* @brief Helper class manages the loading of 2D/3D images based on itk but with some convenience features ...
			*
			* ImageReader is used to load 2D or 3D images in an itk like style, but also offers
			* some convenience features and the specialties of different formats into account.\n
			* 2D images will be loaded directly by the IO classes of itk, so in this case the
			* ImageReader is only a layer of abstraction.\n
			* 3D images will be handled different, depending on the type of files and the chosen
			* series read style:\n
			*  - Default: Depending on the file extension (DICOM images (*.dcm, *.ima): Dicom; others: None; No image file: Dicom
			*	- None: directly by the itk io (no series reading)
			*	- Dicom: uses itk series reader, currently a list of files will be generated
						 that is similar to GDCM (old ::itk::DICOMKSeriesFileNames is not supported any more by itk > 4.3.x.)
			*	- GDCM: uses itk series reader, the list of files will be generated
			*			 by the ::itk::GDCMSeriesFileNames in the specified path ordered by
			*			 imagePositionPatient.
			*	- Numeric: Will be considered as series of images. The list of files will be
			*			  created by ::itk::NumericSeriesFileNames, so in this case the given
			*			  file name is already masked by %d for the increasing index within
			*			  the file name.
			* .
			* @todo implement Reader as an ::itk::ImageSource in the behavior.
			* @note code copied from MatchPoint, see documentation (http://sourceforge.net/projects/matchpoint/)
			*/
			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension = 2>
			class ImageReader
			{
			public:
				typedef ::itk::Image<TInputPixel, iDimension>  InputImageType;
				typedef ::itk::Image<TOutputPixel, iDimension> OutputImageType;
				typedef TInputPixel RescaleValueType;
				typedef std::vector<::itk::MetaDataDictionary> MetaDataDictionaryArrayType;

				typedef std::string String;

				typedef std::ostringstream OStringStream;
				typedef std::istringstream IStringStream;

				virtual const char* GetNameOfClass() const
				{
					return "ImageReader";
				}

			private:
				/** Loaded Image.*/
				typename OutputImageType::Pointer _spImage;
				/** The file name of the image. */
				String  _fileName;
				/** The upper limit for the searching of series files in the path.*/
				unsigned int _upperSeriesLimit;
				/** Indicates if the image data is up to date or should be read again.*/
				bool _upToDate;
				/** Indicates if the output image intensity should be rescaled.*/
				bool _rescaleImage;
				/** Indicates the minimum of the output.*/
				RescaleValueType _rescaleMin;
				/** Indicates the maximum of the output.*/
				RescaleValueType _rescaleMax;
				/** Defines if the specified image file is part of a series and the
				* whole series should be read into one image. Only relevant for 3D images.*/
				typename ImageSeriesReadStyle::Type _seriesReadStyle;

				MetaDataDictionaryArrayType _dictionaryArray;

				void load2D();

				typename ::itk::ImageSource<InputImageType>::Pointer prepareNumericSource() const;
				typename ::itk::ImageSource<InputImageType>::Pointer prepareDICOMSource() const;
				typename ::itk::ImageSource<InputImageType>::Pointer prepareNormalSource() const;
				typename ::itk::ImageSource<InputImageType>::Pointer prepareGDCMSource() const;

				void load3D();

				typedef std::vector<::itk::MetaDataDictionary*> ITKMetaDataDictionaryArray;
				void copyMetaDictionaryArray(const ITKMetaDataDictionaryArray* fromArray,
											 MetaDataDictionaryArrayType& toArray);

			public:
				/** Function to access the member variable _FileName. _FileName represents the filename of the
				* headerfile. The path must be included, the file extension may left away.
				* @return File name of the header file.*/
				const String&  getFileName() const;

				/** Function to access the member variable _FileName. _FileName represents the filename of the
				* headerfile. The path must be included, the file extension may left away.
				* @param [in] sFileName The file name of the header file.*/
				void setFileName(const String&  sFileName);

				/** Function to access the member variable _rescaleMin. _rescaleMin represents
				* the minimum of the intensity rescale filter.
				* @return The minimum of the intensity rescale filter.*/
				const RescaleValueType& getRescaleMinimum() const;

				/** Function to access the member variable _rescaleMin. _rescaleMin represents
				* the minimum of the intensity rescale filter. Changing the rescale minimum out dates the ImageReader.
				* @param [in] dRescaleMin The minimum of the intensity rescale filter.*/
				void setRescaleMinimum(const RescaleValueType& rescaleMin);

				/** Function to access the member variable _rescaleMin. _rescaleMax represents
				* the minimum of the intensity rescale filter.
				* @return The minimum of the intensity rescale filter.*/
				const RescaleValueType& getRescaleMaximum() const;

				/** Function to access the member variable _rescaleMin. _rescaleMax represents
				* the minimum of the intensity rescale filter. Changing the rescale maximum out dates the ImageReader.
				* @param [in] dRescaleMax The minimum of the intensity rescale filter.*/
				void setRescaleMaximum(const RescaleValueType& rescaleMax);

				/** Function to access the member variable _rescaleImage. _rescaleImage indicates if a
				* loaded image should be rescaled regarding its intensities.
				* @return If the ImageReader converts images to iso-voxel.*/
				const bool getRescaleImage() const;

				/** Function to access the member variable _rescaleImage. _rescaleImage indicates if a
				* loaded image should be rescaled regarding its intensities. Changing the rescale option out dates the ImageReader.
				* @param [in] rescaleImage Specifies if image should be converted to isovoxel.*/
				void setRescaleImage(const bool rescaleImage);

				/** Function to access the member variable _upperSeriesLimit. _upperSeriesLimit represents
				* the upper limit for the series file search.
				* @return The upper limit of the series search.*/
				const unsigned int getUpperSeriesLimit() const;

				/** Function to access the member variable _upperSeriesLimit. _upperSeriesLimit represents
				* the upper limit for the series file search. Changing the series limit out dates the ImageReader.
				* @remark It is only relevant if series style is set to "Numeric".
				* @param [in] upperLimit The upper limit of the header file.*/
				void setUpperSeriesLimit(const unsigned int upperLimit);

				/** Function to access the member variable _seriesReadStyle (see member description for more information).*/
				const typename ImageSeriesReadStyle::Type getSeriesReadStyle() const;

				/** Function to access the member variable _seriesReadStyle (see member description for more information).
				* Changing the style out dates the ImageReader.*/
				void setSeriesReadStyle(typename ImageSeriesReadStyle::Type readStyle);

				/** Function loads the image if needed and returns the data.
				* @return Pointer to loaded image.*/
				OutputImageType*  GetOutput(void);

				/** Function returns the reference to the meta data dictionary(ies) of the latest file(s) loaded by this class.
				 * Array may be empty if no MetaDictionary exists.*/
				const MetaDataDictionaryArrayType& getMetaDictionaryArray();

				ImageReader();

				virtual ~ImageReader();
			};


			/**
			* @brief Helper function for the use of ImageReader in on statement ...
			*
			* for specific informations please see the documentation of ImageReader.
			* @param pLoadedDictArray Pass a pointer to valid array to receive the meta dictionaries
			* loaded with the image. If the pointer is null, no dictionaries will be transfered. The array
			* will be reseted before the loaded dictionaries will be added.
			* @sa ImageReader
			* @ingroup Utils
			*/
			template <typename TInputPixel, typename TOutputPixel, unsigned int iDimension>
			typename ImageReader<TInputPixel, TOutputPixel, iDimension>::OutputImageType::Pointer
			readImage(const std::string& fileName,
					  ImageSeriesReadStyle::Type readStyle = ImageSeriesReadStyle::Default,
					  bool rescaleImage = false,
					  typename ImageReader<TInputPixel, TOutputPixel, iDimension>::RescaleValueType rescaleMin = 0,
					  typename ImageReader<TInputPixel, TOutputPixel, iDimension>::RescaleValueType rescaleMax = 255,
					  unsigned int upperNumericSeriesLimit = 100,
					  typename ImageReader<TInputPixel, TOutputPixel, iDimension>::MetaDataDictionaryArrayType*
					  pLoadedDictArray = NULL);
		}//end namespace itk
	}//end namespace io
}//end namespace rttb

#include "rttbImageReader.tpp"

#endif
