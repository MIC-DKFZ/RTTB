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


#ifndef __BOOST_MASK_VOXELIZATION_THREAD_H
#define __BOOST_MASK_VOXELIZATION_THREAD_H

#include <deque>

#include "rttbBaseType.h"

#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

namespace rttb
{
	namespace masks
	{
		namespace boost
		{
			/*! @class BoostMaskGenerateMaskVoxelListThread
			*
			*/
			class BoostMaskVoxelizationThread
			{

			public:
				using BoostPolygon2D = ::boost::geometry::model::polygon< ::boost::geometry::model::d2::point_xy<double> >;
				using BoostPolygonVector = std::vector<BoostPolygon2D>;//polygon with or without holes
				typedef std::map<double, BoostPolygonVector>
				BoostPolygonMap;//map of the z index with the vector of boost 2d polygon
				using VoxelIndexVector = std::vector<rttb::VoxelGridIndex3D>;
				typedef ::boost::multi_array<double, 2> BoostArray2D;
                using BoostArray2DPointer = ::boost::shared_ptr<BoostArray2D>;
                typedef ::boost::shared_ptr<std::map<double, BoostArray2DPointer > > BoostArrayMapPointer;

                /*! @brief Constructor
                * @param aMutex a mutex for thread-safe handling of the _resultVoxelization
                * @param strict true means that volumeFractions of <0 and >1 are NOT corrected. Otherwise, they are automatically corrected to 0 or 1, respectively.
                */
				BoostMaskVoxelizationThread(const BoostPolygonMap& APolygonMap,
                    const VoxelIndexVector& aGlobalBoundingBox, BoostArrayMapPointer anArrayMap, ::boost::shared_ptr<::boost::shared_mutex> aMutex, bool strict);

				void operator()();


			private:
				using BoostPolygonDeque = std::deque<BoostPolygon2D>;
				using BoostRing2D = ::boost::geometry::model::ring< ::boost::geometry::model::d2::point_xy<double> >;
				using BoostPoint2D = ::boost::geometry::model::d2::point_xy<double>;


				BoostPolygonMap _geometryCoordinateBoostPolygonMap;
				VoxelIndexVector _globalBoundingBox;
                BoostArrayMapPointer _resultVoxelization;
                ::boost::shared_ptr<::boost::shared_mutex> _mutex;
                bool _strict;

				/*! @brief Get intersection polygons of the contour and a voxel polygon
				* @param aVoxelIndex3D The 3d grid index of the voxel
				* @param intersectionSlicePolygons The polygons of the slice intersecting the voxel
				* @return Return all intersection polygons of the structure and the voxel
				*/
				static BoostPolygonDeque getIntersections(const rttb::VoxelGridIndex3D& aVoxelIndex3D,
				        const BoostPolygonVector& intersectionSlicePolygons);

				/*! @brief Get the voxel 2d contour polygon in geometry coordinate*/
				static BoostRing2D get2DContour(const rttb::VoxelGridIndex3D& aVoxelGrid3D);

				/*! @brief Calculate the area of all polygons
				* @param aPolygonDeque The deque of polygons
				* @return Return the area of all polygons
				*/
				static double calcArea(const BoostPolygonDeque& aPolygonDeque);
                /*! @brief Corrects the volumeFraction
                * @details the volume fraction is corrected in case of strict=true. Otherwise, it's only corrected for double imprecision
                * @return The corrected volumeFraction
                */
                double correctForErrorAndStrictness(double volumeFraction, bool strict) const;
            };

		}


	}
}

#endif