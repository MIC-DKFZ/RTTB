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
#ifndef __MASK_ACCESSOR_INTERFACE_NEW_H
#define __MASK_ACCESSOR_INTERFACE_NEW_H

#include "rttbCommon.h"
#include "rttbBaseType.h"
#include "rttbMaskVoxel.h"
#include "rttbIndexConversionInterface.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{
	namespace core
	{
        class GeometricInfo;
		/*! @class MaskAccessorInterface
			@brief This class triggers the voxelization and gives acess to the masked voxels.
		*/
		class MaskAccessorInterface: public IndexConversionInterface
		{
		public:
      rttbClassMacro(MaskAccessorInterface, IndexConversionInterface);
			using MaskVoxelList = std::vector<core::MaskVoxel>;
			using MaskVoxelListPointer = boost::shared_ptr<MaskVoxelList>;

		private:
			MaskAccessorInterface(const MaskAccessorInterface&) = delete; //not implemented on purpose -> non-copyable
			MaskAccessorInterface& operator=(const
			                                 MaskAccessorInterface&) = delete;//not implemented on purpose -> non-copyable

		public:
			MaskAccessorInterface() = default;
			~MaskAccessorInterface() override = default;

			/*! @brief Start generation of mask
			@post mask is valid and acessible
			*/
			virtual void updateMask() = 0;

			virtual const GeometricInfo& getGeometricInfo() const = 0;

			/*! @brief Get vector containing all relevant voxels that are inside the given structure.
			*/
			virtual MaskVoxelListPointer getRelevantVoxelVector() = 0;

			/*! @brief get vector containing all relevant voxels that have a relevant volume above the given threshold
			and are inside the given structure
			@pre updateMask should have been called (at least once, to ensure a valid mask).
			*/
			virtual MaskVoxelListPointer getRelevantVoxelVector(float lowerThreshold) = 0;

			/*! @brief Get masked voxel value corresponding to a given VoxelGridID.
				@post after a valid call voxel contains the mask information corresponding to aID.
				If aID is not valid, voxel values are undefined.
				@return Indicates if a MaskVoxel for the given ID exists and therefore if parameter voxel containes
				valid values.
			@pre updateMask should have been called (at least once, to ensure a valid mask).
			*/
			virtual bool getMaskAt(const VoxelGridID aID, MaskVoxel& voxel) const = 0;

			/*! @brief Get masked voxel value corresponding to a given VoxelGridIndex.
				@post after a valid call voxel contains the mask information corresponding to gridIndex.
				If gridIndex is not valid, voxel values are undefined.
				@return Indicates if a MaskVoxel for the given index exists and therefore if parameter voxel containes
				valid values.
			@pre updateMask should have been called (at least once, to ensure a valid mask).
			*/
			virtual bool getMaskAt(const VoxelGridIndex3D& gridIndex, MaskVoxel& voxel) const = 0;

			/* @brief Is true if dose is on a homogeneous grid.
			   @note Inhomogeneous grids are not supported at the moment, but if they will
			   be supported in the future the interface does not need to change.
			*/
			virtual bool isGridHomogeneous() const
			{
				return true;
			}

			virtual IDType getMaskUID() const = 0;
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
