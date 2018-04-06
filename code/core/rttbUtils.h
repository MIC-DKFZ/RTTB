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

#ifndef __RTTB_UTILS_H
#define __RTTB_UTILS_H

#include <map>
#include <vector>
#include <rttbBaseType.h>

#include "RTTBCoreExports.h"

namespace rttb
{
	namespace core
	{

		bool RTTBCore_EXPORT isKey(const std::map<double, double>& values, double value);

		bool RTTBCore_EXPORT isKey(const std::vector<double>& values, double value);

		bool RTTBCore_EXPORT valueIsClose(double value1, double value2, double specificErrorConstant = 1e-5);

		bool RTTBCore_EXPORT isFile(FileNameType aName);

		bool RTTBCore_EXPORT isDirectory(FileNameType aName);

		std::string RTTBCore_EXPORT getFilenameWithoutEnding(const std::string& outfilename);
	
		std::string RTTBCore_EXPORT getFileEnding(const std::string& outfilename);
	
	}
}

#endif
