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

#ifndef __RTTB_IMAGE_WRITER_H
#define __RTTB_IMAGE_WRITER_H

#include "RTToolboxConfigure.h"
#ifdef RTTB_DISABLE_ITK_IO_FACTORY_AUTO_REGISTER
#undef ITK_IO_FACTORY_REGISTER_MANAGER
#endif

#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "rttbBaseType.h"

#include "RTTBITKIOExports.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{


			/** @class ImageWriter
			* @brief Helper class writing 3D images to file ...
			*
			*/
            class RTTBITKIO_EXPORT ImageWriter
			{
				typedef ::itk::Image<DoseTypeGy, 3> ITKMaskImageType;
				using ITKImageTypeConstPointer = ITKMaskImageType::ConstPointer;
				using WriterType = ::itk::ImageFileWriter<ITKMaskImageType>;

			private:
				FileNameString _fileName;
				ITKImageTypeConstPointer _itkImage;


			public:
				ImageWriter(const FileNameString& aFileName, ITKImageTypeConstPointer aITKImage);

				/*! @brief Write itk image to file
				@return Return true if successful.
				@exception InvalidParameterException thrown if itk exception by writing the image
				*/
				bool writeFile();



			};
		}//end namespace itk
	}//end namespace io
}//end namespace rttb


#endif
