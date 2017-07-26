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
#ifndef __DUMMY_MASK_ACCESSOR_H
#define __DUMMY_MASK_ACCESSOR_H

#include <vector>

#include "rttbMaskAccessorInterface.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"


namespace rttb
{
	namespace testing
	{

		/*! @class DumyMaskAccessor
			@brief Create a dummy MaskAccessor for testing.
		*/
		class DummyMaskAccessor: public core::MaskAccessorInterface
		{
		public:
			typedef core::MaskAccessorInterface::MaskVoxelList MaskVoxelList;
			typedef core::MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;

		private:
			/*! vector containing list of mask voxels*/
			MaskVoxelListPointer _spRelevantVoxelVector;

			core::GeometricInfo _geoInfo;

			IDType _maskUID;

		public:
			DummyMaskAccessor(const core::GeometricInfo& aGeometricInfo);
			DummyMaskAccessor(const core::GeometricInfo& aGeometricInfo, MaskVoxelListPointer voxelListPtr);
			~DummyMaskAccessor() {};

			void updateMask();

			const core::GeometricInfo& getGeometricInfo() const
			{
				return _geoInfo;
			};

			MaskVoxelListPointer getRelevantVoxelVector();

			MaskVoxelListPointer getRelevantVoxelVector(float lowerThreshold);

			bool getMaskAt(const VoxelGridID aID, core::MaskVoxel& voxel) const;

			bool getMaskAt(const VoxelGridIndex3D& gridIndex, core::MaskVoxel& voxel) const;

			IDType getMaskUID() const
			{
				return _maskUID;
			};
		};
	}
}

#endif
