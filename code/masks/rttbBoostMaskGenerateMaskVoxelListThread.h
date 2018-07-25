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
#include "rttbMaskAccessorInterface.h"

#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace rttb
{
	namespace masks
	{
		namespace boost
		{
			/*! @class BoostMaskGenerateMaskVoxelListThread
			*
			*/
			class BoostMaskGenerateMaskVoxelListThread
			{

			public:
				typedef ::boost::multi_array<double, 2> BoostArray2D;
                using BoostArray2DPointer = ::boost::shared_ptr<BoostArray2D>;
                typedef ::boost::shared_ptr<std::map<double, BoostArray2DPointer> > BoostArrayMapPointer;
				using VoxelIndexVector = std::vector<rttb::VoxelGridIndex3D>;
                using MaskVoxelListPointer = core::MaskAccessorInterface::MaskVoxelListPointer;

				BoostMaskGenerateMaskVoxelListThread(const VoxelIndexVector& aGlobalBoundingBox,
          core::GeometricInfo::Pointer aGeometricInfo,
                                                     BoostArrayMapPointer aVoxelizationMap,
				                                     double aVoxelizationThickness,
				                                     unsigned int aBeginSlice,
				                                     unsigned int aEndSlice,
                                                     MaskVoxelListPointer aMaskVoxelList, bool strictVoxelization, ::boost::shared_ptr<::boost::shared_mutex> aMutex);
				void operator()();

			private:
				VoxelIndexVector _globalBoundingBox;
				core::GeometricInfo::Pointer _geometricInfo;
        BoostArrayMapPointer _voxelizationMap;
        bool _strictVoxelization=true;
				//(for example, the first contour has the double grid index 0.1, the second 0.3, the third 0.5, then the thickness is 0.2)
				double _voxelizationThickness;

				unsigned int _beginSlice;
				/*! @brief _endSlice is the first index not to be processed (like a end iterator)
				*/
				unsigned int _endSlice;

                MaskVoxelListPointer _resultMaskVoxelList;
                ::boost::shared_ptr<::boost::shared_mutex> _mutex;

				/*! @brief For each dose grid index z, calculate the weight vector for each structure contour
				*/
				void calcWeightVector(const rttb::VoxelGridID& aIndexZ,
				                      std::map<double, double>& weightVector) const;

      };

		}


	}
}

#endif