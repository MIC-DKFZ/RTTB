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
#include <boost/lexical_cast.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"

#include "rttbDicomHelaxIODVecDoseAccessorGenerator.h"
#include "rttbDicomHelaxDoseAccessor.h"
#include "rttbInvalidDoseException.h"
#include "rttbDcmrtException.h"
#include "rttbIndexOutOfBoundsException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace io
	{
		namespace helax
		{

			DicomHelaxIODVecDoseAccessorGenerator::~DicomHelaxIODVecDoseAccessorGenerator() {}

			DicomHelaxIODVecDoseAccessorGenerator::DicomHelaxIODVecDoseAccessorGenerator(std::vector<DRTDoseIODPtr>&
			        aDICOMRTDoseVector)
			{
				_dosePtrVector = aDICOMRTDoseVector;

			}

			core::DoseAccessorGeneratorInterface::DoseAccessorPointer DicomHelaxIODVecDoseAccessorGenerator::generateDoseAccessor()
			{
				_doseAccessor = boost::make_shared<io::helax::DicomHelaxDoseAccessor>(_dosePtrVector);
				return _doseAccessor;
			}




		}
	}
}
