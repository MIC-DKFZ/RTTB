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
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"
#include "../itk/rttbITKIOHelper.h"


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
				_itkDoubleImage = rttb::io::itk::readITKDoubleImage(_itkMaskFileName);
				
				_maskAccessor = boost::make_shared<ITKImageMaskAccessor>(_itkDoubleImage.GetPointer());
				return _maskAccessor;
			}

		}//end namespace itk
	}//end namespace io
}//end namespace rttb

