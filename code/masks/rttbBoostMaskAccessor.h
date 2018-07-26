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

#ifndef __BOOST_MASK_R_ACCESSOR__H
#define __BOOST_MASK_R_ACCESSOR__H

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbMaskAccessorInterface.h"
#include "rttbStructure.h"

#include "RTTBBoostMaskExports.h"

namespace rttb
{
	namespace masks
	{
		namespace boost
		{
			/*! @class BoostMaskAccessor
			*   @brief Using the voxelization based on boost::geometry and generate the mask accessor.
			*   @attention  If "strict" is set to true, an exception will be thrown when the given structure has self intersection.
			*   (A structure without self intersection means all contours of the structure have no self intersection, and
			*   the polygons on the same slice have no intersection between each other, unless the case of a donut. A donut is accepted.)
			*   If "strict" is set to false, debug information will be displayed when the given structure has self intersection. Self intersections will be ignored
			*   and the mask will be calculated, however, it may cause errors in the mask results.
			*/
            class RTTBBoostMask_EXPORT BoostMaskAccessor : public core::MaskAccessorInterface
			{
			public:
				using MaskVoxelList = core::MaskAccessorInterface::MaskVoxelList;
				using MaskVoxelListPointer = core::MaskAccessorInterface::MaskVoxelListPointer;

				using StructTypePointer = core::Structure::Pointer;

			private:
        StructTypePointer _spStructure;
				core::GeometricInfo _geoInfo;
        bool _strict;

				/*! vector containing list of mask voxels*/
				MaskVoxelListPointer _spRelevantVoxelVector;

				IDType _maskUID;

			public:


				/*! @brief Constructor with a structure pointer and a geometric info pointer
				* @param aStructurePointer smart pointer of the structure
				* @param aGeometricInfo smart pointer of the geometricInfo of the dose
				* @param strict indicates whether to allow self intersection in the structure. If it is set to true, an exception will be thrown when the given structure has self intersection.
				* @exception InvalidParameterException thrown if strict is true and the structure has self intersections
				*/
				BoostMaskAccessor(StructTypePointer aStructurePointer, const core::GeometricInfo& aGeometricInfo,
				                  bool strict = true);

				/*! @brief destructor*/
				~BoostMaskAccessor() override;

				/*! @brief voxelization of the given structures using boost algorithms*/
				void updateMask() override;

				/*! @brief get vector containing all relevant voxels that are inside the given structure*/
				MaskVoxelListPointer getRelevantVoxelVector() override;

				/*! @brief get vector containing all relevant voxels that have a relevant volume above the given threshold and are inside the given structure*/
				MaskVoxelListPointer getRelevantVoxelVector(float lowerThreshold) override;

				/*!@brief determine how a given voxel on the dose grid is masked
				* @param aID ID of the voxel in grid.
				* @param voxel Reference to the voxel.
				* @post after a valid call voxel containes the information of the specified grid voxel. If aID is not valid, voxel values are undefined.
				* The relevant volume fraction will be set to zero.
				* @return Indicates of the voxel exists and therefore if parameter voxel containes valid values.*/
				bool getMaskAt(const VoxelGridID aID, core::MaskVoxel& voxel) const override;

				/*!@brief determine how a given voxel on the dose grid is masked
				* @param aIndex 3d index of the voxel in grid.
				* @param voxel Reference to the voxel.
				* @return Indicates of the voxel exists and therefore if parameter voxel containes valid values.*/
				bool getMaskAt(const VoxelGridIndex3D& aIndex, core::MaskVoxel& voxel) const override;

				/*! @brief give access to GeometricInfo*/
				const core::GeometricInfo& getGeometricInfo() const override;

				/* @ brief is true if dose is on a homogeneous grid
				* @remark Inhomogeneous grids are not supported at the moment, but if they will be supported in the future the interface does not need to change.*/
				bool isGridHomogeneous() const override
				{
					return true;
				};

				IDType getMaskUID() const override
				{
					return _maskUID;
				};

			};
		}
	}
}

#endif
