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
#ifndef __MUTABLE_DOSE_ACCESSOR_INTERFACE_NEW_H
#define __MUTABLE_DOSE_ACCESSOR_INTERFACE_NEW_H

#include "rttbDoseAccessorInterface.h"
#include "rttbBaseType.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{
	namespace core
	{

		/*! @class MutableDoseAccessorInterface
			@brief Extends the DoseAccessorInterface to provide writing access to the data.
		*/
		class MutableDoseAccessorInterface: public DoseAccessorInterface
		{

		public:
			using MutableDoseAccessorPointer = boost::shared_ptr<MutableDoseAccessorInterface>;

			virtual void setDoseAt(const VoxelGridID aID, DoseTypeGy value) = 0;

			virtual void setDoseAt(const VoxelGridIndex3D& aIndex, DoseTypeGy value) = 0;

		};
	}
}

#endif
