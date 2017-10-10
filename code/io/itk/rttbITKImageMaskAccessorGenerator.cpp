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


#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "rttbITKImageMaskAccessorGenerator.h"
#include "rttbException.h"
#include "rttbInvalidDoseException.h"


namespace rttb
{
	namespace io
	{
		namespace itk
		{
			ITKImageMaskAccessorGenerator::ITKImageMaskAccessorGenerator(const
			        ITKImageMaskAccessor::ITKMaskImageType* aMaskImage)
			{
				if (aMaskImage == nullptr)
				{
					throw core::InvalidDoseException("MaskImage is nullptr");
				}

				_maskPtr = aMaskImage;
			}

			core::MaskAccessorGeneratorBase::MaskAccessorPointer
			ITKImageMaskAccessorGenerator::generateMaskAccessor()
			{
				_maskAccessor = boost::make_shared<ITKImageMaskAccessor>(_maskPtr);
				return _maskAccessor;
			}


		}//end namespace mask
	}//end namespace io
}//end namespace rttb

