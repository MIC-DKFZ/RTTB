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
#ifndef __MASK_ACCESSOR_OTB_NEW_H
#define __MASK_ACCESSOR_OTB_NEW_H

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbMaskVoxel.h"
#include "rttbMaskType_LEGACY.h"
#include "rttbBaseType_LEGACY.h"
#include "rttbStructure_LEGACY.h"
#include "rttbMask.h"
#include "rttbContour_LEGACY.h"
#include "rttbPolygonInfo_LEGACY.h"
#include "DoseIteratorInterface_LEGACY.h"
#include "DoseIterator_LEGACY.h"
#include "rttbMaskAccessorInterface.h"
#include "rttbGenericDoseIterator.h"
#include "rttbStructure.h"

#include <boost/shared_ptr.hpp>

#include <RTTBLegacyMaskExports.h>


namespace rttb
{

	namespace masks
	{
		namespace legacy
		{
			/*! @class OTBMaskAccessor
			*   @brief Implementation of original toolbox voxelization by M. Hub.
                @deprecated please use boost::BoostMaskAccessor
			*/
            class RTTBLegacyMask_EXPORT OTBMaskAccessor : public core::MaskAccessorInterface
			{
			public:
				typedef core::MaskAccessorInterface::MaskVoxelList MaskVoxelList;
				typedef core::MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;

				typedef core::Structure::StructTypePointer StructTypePointer;

			private:

				core::GeometricInfo _geoInfo;

				/*! vector containing list of mask voxels*/
				MaskVoxelListPointer _spRelevantVoxelVector;

				StructTypePointer _spStructure;

				legacy::StructureLegacy _legacyStructure;

				IDType _maskUID;


			public:

				~OTBMaskAccessor();

				// import of structure sets (loading from data) is done elsewhere. Structures are only voxelized here.
				// here the original RTToolbox voxelization shall be implemented
				OTBMaskAccessor(StructTypePointer aStructurePointer, const core::GeometricInfo& aGeometricInfo);

				/*! @brief voxelization of the given structures according to the original RTToolbox algorithm*/
				void updateMask();

				/*! @brief get vector conatining al relevant voxels that are inside the given structure*/
				MaskVoxelListPointer getRelevantVoxelVector();
				/*! @brief get vector conatining al relevant voxels that have a relevant volume above the given threshold and are inside the given structure*/
				MaskVoxelListPointer getRelevantVoxelVector(float lowerThreshold);

				/*!@brief determine how a given voxel on the dose grid is masked
				* @param aID ID of the voxel in grid.
				* @param voxel Reference to the voxel.
				* @post after a valid call voxel containes the information of the specified grid voxel. If aID is not valid, voxel values are undefined.
				* The relevant volume fraction will be set to zero.
				* @return Indicates of the voxel exists and therefore if parameter voxel containes valid values.*/
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

			};
		}
	}
}

#endif
