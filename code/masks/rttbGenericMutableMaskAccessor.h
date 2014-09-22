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
#ifndef __GENERIC_MUTABLE_MASK_ACCESSOR_H
#define __GENERIC_MUTABLE_MASK_ACCESSOR_H

#include "rttbMutableMaskAccessorInterface.h"
#include "rttbBaseType.h"
#include "rttbMaskVoxel.h"

namespace rttb
{
	namespace masks
	{

		/*! @class GenericMutableMaskAccessor
		@brief Default implementation of MutableMaskAccessorInterface.
		@see MutableMaskAccessorInterface
		*/
		class GenericMutableMaskAccessor: public core::MutableMaskAccessorInterface
		{
		public:
			typedef core::MutableMaskAccessorInterface::MaskVoxelList MaskVoxelList;
			typedef core::MutableMaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;

		private:
			core::GeometricInfo _geoInfo;

			/*! vector containing list of mask voxels*/
			MaskVoxelListPointer _spRelevantVoxelVector;

			IDType _maskUID;

			GenericMutableMaskAccessor(const
			                           GenericMutableMaskAccessor&); //not implemented on purpose -> non-copyable
			GenericMutableMaskAccessor& operator=(const
			                                      GenericMutableMaskAccessor&);//not implemented on purpose -> non-copyable

		public:
			~GenericMutableMaskAccessor();

			GenericMutableMaskAccessor(const core::GeometricInfo& aGeometricInfo);

			/*! @brief initialize mask structure if _spRelevantVoxelVector was not previously initialized*/
			void updateMask();

			/*! @brief get vector containing all relevant voxels that are inside the given structure*/
			MaskVoxelListPointer getRelevantVoxelVector();
			/*! @brief get vector containing all relevant voxels that have a relevant volume above the given threshold and are inside the given structure*/
			MaskVoxelListPointer getRelevantVoxelVector(float lowerThreshold);

			/*!@brief determine how a given voxel on the dose grid is masked
			* @param aID ID of the voxel in grid.
			* @param voxel Reference to the voxel.
			* @post after a valid call voxel contains the information of the specified grid voxel. If aID is not valid, voxel values are undefined.
			* The relevant volume fraction will be set to zero.
			* @return Indicates if the voxel exists and therefore if parameter voxel contains valid values.*/
			bool getMaskAt(const VoxelGridID aID, core::MaskVoxel& voxel) const;

			bool getMaskAt(const VoxelGridIndex3D& aIndex, core::MaskVoxel& voxel) const;

			/* @ brief is true if dose is on a homogeneous grid */
			// Inhomogeneous grids are not supported at the moment, but if they will
			// be supported in the future the interface does not need to change.
			bool isGridHomogeneous() const
			{
				return true;
			};

			/*! @brief give access to GeometricInfo*/
			inline const core::GeometricInfo& getGeometricInfo() const
			{
				return _geoInfo;
			};

			IDType getMaskUID() const
			{
				return _maskUID;
			};


			void setMaskAt(VoxelGridID aID, const core::MaskVoxel& voxel);

			void setMaskAt(const VoxelGridIndex3D& gridIndex, const core::MaskVoxel& voxel);

			void setRelevantVoxelVector(MaskVoxelListPointer aVoxelListPointer);

		};
	}
}
#endif