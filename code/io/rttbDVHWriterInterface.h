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
#ifndef __DVH_WRITER_INTERFACE_H
#define __DVH_WRITER_INTERFACE_H


#include "rttbDVH.h"

#include "RTTBDicomIOExports.h"

namespace rttb
{
	namespace io
	{
		/*! @class DVHWriterInterface
		@brief Interface for classes that write DVH data to files.
		*/
        class RTTBDicomIO_EXPORT DVHWriterInterface
		{
			typedef core::DVH::DVHPointer DVHPointer;
			/*! @brief Write aDvh
			*/
		public:
			virtual void writeDVH(DVHPointer aDvh) = 0;
		};
	}

}

#endif
