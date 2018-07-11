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
#ifndef __MUTABLE_MASK_ACCESSOR_INTERFACE_H
#define __MUTABLE_MASK_ACCESSOR_INTERFACE_H

#include "rttbCommon.h"

#include "rttbMaskAccessorInterface.h"
#include "rttbBaseType.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{
	namespace core
	{
        class MaskVoxel;
		/*! @class MutableMaskAccessorInterface
		@brief Extends the MaskAccessorInterface to provide writing access to the data.
		@details This interface is created for external manipulation of generated masks. For example to store
		the results of arithmetic operations on other masks.
		*/
		class MutableMaskAccessorInterface: public MaskAccessorInterface
		{
		public:
      rttbClassMacro(MutableMaskAccessorInterface, MaskAccessorInterface)
			using MaskVoxelList = core::MaskAccessorInterface::MaskVoxelList;
			using MaskVoxelListPointer = core::MaskAccessorInterface::MaskVoxelListPointer;

			virtual void setRelevantVoxelVector(MaskVoxelListPointer aVoxelListPointer) = 0;

			virtual void setMaskAt(VoxelGridID aID, const MaskVoxel& voxel) = 0;

			virtual void setMaskAt(const VoxelGridIndex3D& gridIndex, const MaskVoxel& voxel) = 0;

		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
