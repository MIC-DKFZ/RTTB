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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/
#ifndef __MaskWriter_h
#define __MaskWriter_h

#include "rttbITKImageMaskAccessorConverter.h"
#include "rttbITKImageFileMaskAccessorGenerator.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizer
		{
			/*! @class MaskWriter
			@brief MaskWriter write a mask into a file
			*/
			class MaskWriter
			{

			public:
				typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;
				typedef io::itk::ITKImageMaskAccessor::ITKMaskImageType::Pointer ITKImageTypePointer;
				typedef io::itk::ITKImageMaskAccessor::ITKMaskImageType::ConstPointer ITKImageTypeConstPointer;
				typedef itk::Image<rttb::DoseTypeGy, 3> ITKMaskImageType;


				/*!@brief Constructor
				@details save the object parameter into _maskAccessorPtr objekt
				@param vector with MaskAccessorPointer object/s
				*/
				MaskWriter(std::vector<MaskAccessorPointer> maskPointer, bool voxelization);

				/**@brief write the mask into the outputfile
				@param Outputfilename
				*/
				void writeMaskToFile(const std::string& outputFileName) const;

			private:

				ITKImageTypeConstPointer addMultipleStructsToImage() const;
				ITKImageTypeConstPointer applyThresholdFilter(ITKImageTypeConstPointer itkImage) const;
				void writeITKImageToFile(ITKImageTypeConstPointer itkImage, const std::string& outputfilename) const;

				//MaskAccessorPointer _maskAccessorPtr;
				bool _booleanvoxelization;
				std::vector<MaskAccessorPointer> _maskPointerVector;
			};
		}
	}
}
#endif