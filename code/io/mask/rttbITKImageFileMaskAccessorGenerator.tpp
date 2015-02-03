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
// @version $Revision: 793 $ (last changed revision)
// @date    $Date: 2014-10-10 10:24:45 +0200 (Fr, 10 Okt 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
// Subversion HeadURL: $HeadURL: https://svn/sbr/Sources/SBR-Projects/MatchPoint/trunk/Code/IO/include/mapImageReader.tpp $
*/

#ifndef __RTTB_ITK_IMAGE_FILE_MASK_ACCESSOR_GENERATOR_TPP
#define __RTTB_ITK_IMAGE_FILE_MASK_ACCESSOR_GENERATOR_TPP

#include "rttbITKImageFileMaskAccessorGenerator.h"
#include "rttbInvalidParameterException.h"
#include "rttbInvalidDoseException.h"
#include "rttbITKImageMaskAccessor.h"

namespace rttb
{
	namespace io
	{
		namespace mask
		{

			template <typename TPixelType> void ITKImageFileMaskAccessorGenerator::doCasting(
			    io::itk::GenericImageReader::GenericOutputImageType* genericImage)
			{
				typedef ::itk::Image<TPixelType, 3> InputImageType;
				typedef ITKImageMaskAccessor::ITKMaskImageType OutputImageType;
				InputImageType::Pointer pCastedInput = dynamic_cast<InputImageType*>(genericImage);
				typedef ::itk::CastImageFilter<InputImageType, OutputImageType> CastFilterType;
				CastFilterType::Pointer castFilter = CastFilterType::New();
				castFilter->SetInput(pCastedInput);

				try
				{
					//important to update the filter!
					castFilter->Update();
					_itkDoubleImage = castFilter->GetOutput();
				}
				catch (::itk::ExceptionObject& e)
				{
					std::cerr << "ITK Error!!!" << std::endl;
					std::cerr << e << std::endl;
				}
			}

		}
	}
}
#endif
