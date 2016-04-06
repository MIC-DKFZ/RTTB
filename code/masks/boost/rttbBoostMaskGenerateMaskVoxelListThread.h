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
// @version $Revision: 1127 $ (last changed revision)
// @date    $Date: 2015-10-01 13:33:33 +0200 (Do, 01 Okt 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#ifndef __BOOST_MASK_GENERATE_MASK_VOXEL_LIST_H
#define __BOOST_MASK_GENERATE_MASK_VOXEL_LIST_H

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbMaskVoxel.h"

#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lockfree/queue.hpp>

namespace rttb
{
	namespace masks
	{
		namespace boostRedesign
		{
			/*! @class BoostMaskGenerateMaskVoxelListThread
			*
			*/
			class BoostMaskGenerateMaskVoxelListThread
			{

			public:
				typedef ::boost::shared_ptr<rttb::core::GeometricInfo> GeometricInfoPointer;
				typedef ::boost::multi_array<double, 2> BoostArray2D;
				typedef std::map<double, BoostArray2D> BoostArrayMap;
				typedef ::boost::shared_ptr<rttb::core::MaskVoxel> MaskVoxelPointer;
				typedef ::boost::shared_ptr<::boost::lockfree::queue<core::MaskVoxel*>> MaskVoxelQueuePointer;

				BoostMaskGenerateMaskVoxelListThread(int aGlobalBoundingBoxSize0,
				                                     int aGlobalBoundingBoxSize1,
				                                     const rttb::VoxelGridIndex3D& aMinIndex,
				                                     const rttb::VoxelGridIndex3D& aMaxIndex,
				                                     GeometricInfoPointer aGeometricInfo,
				                                     const BoostArrayMap& aVoxelizationMap,
				                                     double aVoxelizationThickness,
				                                     unsigned int aBeginSlice,
				                                     unsigned int aEndSlice,
				                                     MaskVoxelQueuePointer aResultMaskVoxelQueue);
				void operator()();

			private:
				int _globalBoundingBoxSize0;
				int _globalBoundingBoxSize1;
				rttb::VoxelGridIndex3D _minIndex;
				rttb::VoxelGridIndex3D _maxIndex;
				GeometricInfoPointer _geometricInfo;
				BoostArrayMap _voxelizationMap;
				//(for example, the first contour has the double grid index 0.1, the second 0.3, the third 0.5, then the thickness is 0.2)
				double _voxelizationThickness;

				unsigned int _beginSlice;
				unsigned int _endSlice;

				MaskVoxelQueuePointer _resultMaskVoxelQueue;

				/*! @brief For each dose grid index z, calculate the weight vector for each structure contour
				*/
				void calcWeightVector(const rttb::VoxelGridID& aIndexZ,
				                      std::map<double, double>& weightVector) const;
			};

		}


	}
}

#endif