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

#ifndef __MAPPING_OUTSIDE_OF_IMAGE_EXCEPTION_H
#define __MAPPING_OUTSIDE_OF_IMAGE_EXCEPTION_H

#include <string>
#include <exception>

#include "rttbException.h"

#include "RTTBCoreExports.h"


namespace rttb
{
	namespace core
	{

		/*! @class MappingOutsideOfImageException
			@brief This exception will be thrown if a transformation leads to an invalid position outside of the image.
		*/
		class RTTBCore_EXPORT MappingOutsideOfImageException : public Exception
		{
		public:
			MappingOutsideOfImageException(const std::string& aWhat): Exception(aWhat) {}

			~MappingOutsideOfImageException() throw() override {}

			/*! @brief Get the exception description
			*/
			const char* what() const throw() override;

			/*! @brief Get the name of the exception class
			*/
			virtual const char* GetNameOfClass() const;
		};

	}
}

#endif
