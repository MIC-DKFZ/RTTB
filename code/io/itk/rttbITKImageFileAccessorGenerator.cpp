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

#include <boost/make_shared.hpp>

#include "rttbITKImageFileAccessorGenerator.h"
#include "rttbITKIOHelper.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
			ITKImageFileAccessorGenerator::~ITKImageFileAccessorGenerator()
			= default;

			ITKImageFileAccessorGenerator::ITKImageFileAccessorGenerator(const FileNameType& fileName, const bool& useDicom)
			{
				_fileName = fileName;
				_useDicom = useDicom;
			}

			rttb::core::DoseAccessorGeneratorBase::DoseAccessorPointer ITKImageFileAccessorGenerator::generateDoseAccessor() {
				_itkDoubleImage = readITKDoubleImage(_fileName, _useDicom);

				_doseAccessor = boost::make_shared<ITKImageAccessor>(_itkDoubleImage.GetPointer());
				return _doseAccessor;
			}

		}//end namespace itk
	}//end namespace io
}//end namespace rttb

