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
// @version $Revision: 484 $ (last changed revision)
// @date    $Date: 2014-03-26 16:16:16 +0100 (Mi, 26 Mrz 2014) $ (last change date)
// @author  $Author: zhangl $ (last changed by)
*/
#ifndef __BOOST_MASK_ACCESSOR__H
#define __BOOST_MASK_ACCESSOR__H

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbMaskVoxel.h"
#include "rttbBoostMask.h"
#include "rttbMaskAccessorInterface.h"
#include "rttbGenericDoseIterator.h"
#include "rttbStructure.h"

#include <boost/shared_ptr.hpp>


namespace rttb
{

	namespace masks
	{
		/*! @class OTBMaskAccessor
		*   @brief Implementation of original toolbox voxelization by M. Hub.
		*/
		class BoostMaskAccessor: public core::MaskAccessorInterface
		{
		public:
			typedef core::MaskAccessorInterface::MaskVoxelList MaskVoxelList;
			typedef core::MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;

			typedef core::Structure::StructTypePointer StructTypePointer;
			typedef boost::shared_ptr<core::GeometricInfo> GeometricInfoPtr;

		private:

			GeometricInfoPtr _spGeoInfo;

			/*! vector containing list of mask voxels*/
			MaskVoxelListPointer _spRelevantVoxelVector;

			StructTypePointer _spStructure;

			IDType _maskUID;


		public:

			~BoostMaskAccessor();

			// import of structure sets (loading from data) is done elsewhere. Structures are only voxelized here.
			// here the original RTToolbox voxelization shall be implemented
			BoostMaskAccessor(StructTypePointer aStructurePointer, GeometricInfoPtr aGeometricInfoPtr);

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

			/*! @brief give access to GeometricInfo*/
			const core::GeometricInfo& getGeometricInfo() const;

			/* @ brief is true if dose is on a homogeneous grid */
			// Inhomogeneous grids are not supported at the moment, but if they will
			// be supported in the future the interface does not need to change.
			bool isGridHomogeneous() const
			{
				return true;
			};

			IDType getMaskUID() const
			{
				return _maskUID;
			};

		};
	}
}

#endif
