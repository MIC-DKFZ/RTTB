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

#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include <string>
#include <exception>
#include <iostream>

#include "RTTBCoreExports.h"

namespace rttb
{
	namespace core
	{

		/*! @class Exception
			@brief Exception interface used by all RTToolbox exceptions.
		*/

		class RTTBCore_EXPORT Exception : public std::exception
		{
		protected:
			std::string rttb_what;

		public:
			explicit Exception(const std::string& aWhat)
				: rttb_what(aWhat)
			{}
			~Exception() throw() override {}

			/*! @brief Get the exception description
			*/
			const char* what() const throw() override;

			/*! @brief Get the name of the exception class that was thrown
			*/
			const char* GetNameOfClass() const;
		};

	}
}

#endif
