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

#ifndef __PHYSICAL_INFO_H
#define __PHYSICAL_INFO_H


#include "rttbBaseType.h"


namespace rttb{

	namespace core{

		/*! @class PhysicalInfo
		*/
		class PhysicalInfo
			{
			public:
				void setFullPath(PathType aPath);
				PathType getFullPath();
			private:
				PathType _fullPath;

			};

		}
	}

#endif
