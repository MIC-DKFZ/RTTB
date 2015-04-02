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
// @version $Revision: 747 $ (last changed revision)
// @date    $Date: 2014-09-17 12:01:00 +0200 (Mi, 17 Sep 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include <assert.h>
#include <stdio.h>
#include <stdarg.h>     /* va_list, va_start, va_copy, va_arg, va_end */


#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>

#include "rttbDicomIODDoseAccessorConverter.h"
#include "rttbException.h"
#include "rttbInvalidDoseException.h"
#include "rttbGeometricInfo.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDoseStatistics.h"


namespace rttb
{
	namespace io
	{
		namespace dicom
		{

			DicomIODDoseAccessorConverter::DicomIODDoseAccessorConverter(DoseAccessorPointer accessor)
			{
				setDoseAccessor(accessor);
				_doseIOD = boost::make_shared<DRTDoseIOD>();	
			}


		}//end namespace itk
	}//end namespace io
}//end namespace rttb

