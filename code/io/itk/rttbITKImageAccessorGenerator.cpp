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

#include <assert.h>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "rttbITKImageAccessor.h"
#include "rttbITKImageAccessorGenerator.h"
#include "rttbException.h"
#include "rttbInvalidDoseException.h"


namespace rttb
{
	namespace io
	{
		namespace itk
		{

			ITKImageAccessorGenerator::ITKImageAccessorGenerator(const ITKImageType* aDoseImage)
			{
				if (aDoseImage == NULL)
				{
					throw core::InvalidDoseException("doseImage is NULL");
				}

				_dosePtr = aDoseImage;
			}

			core::DoseAccessorGeneratorBase::DoseAccessorPointer ITKImageAccessorGenerator::generateDoseAccessor()
			{
				_doseAccessor = boost::make_shared<ITKImageAccessor>(_dosePtr);
				return _doseAccessor;
			}

		}//end namespace itk
	}//end namespace io
}//end namespace rttb

