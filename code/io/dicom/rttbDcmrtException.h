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

#ifndef __DCMRT_EXCEPTION_H
#define __DCMRT_EXCEPTION_H

#include <string>
#include <exception>

#include "rttbException.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{

			/*! @class DcmrtException
			@brief This class represents a DcmrtException. Any dcmrt error will throw this exception.
			*/
			class DcmrtException: public core::Exception
			{
			public:
				DcmrtException(const std::string& aWhat): Exception(aWhat) {}

				virtual ~DcmrtException() throw() {}

				/*! @brief Get the exception description
				*/
				const char* what() const throw();

				/*! @brief Get the name of the class that was thrown
				*/
				const char* GetNameOfClass() const;
			};
		}
	}

}

#endif
