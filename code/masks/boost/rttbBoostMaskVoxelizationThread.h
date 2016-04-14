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

#include "rttbBaseType.h"
#include "rttbGeometricInfo.h"
#include "rttbMaskVoxel.h"
#include "rttbInvalidParameterException.h"

#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/ring.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>

namespace rttb
{
	namespace masks
	{
		namespace boostRedesign
		{
			/*! @class BoostMaskGenerateMaskVoxelListThread
			*
			*/
			class BoostMaskVoxelizationThread
			{

			public:
				typedef ::boost::geometry::model::polygon< ::boost::geometry::model::d2::point_xy<double> >
				BoostPolygon2D;
				typedef std::vector<BoostPolygon2D> BoostPolygonVector;//polygon with or without holes
				typedef std::map<double, BoostPolygonVector>
				BoostPolygonMap;//map of the z index with the vector of boost 2d polygon
				typedef std::vector<rttb::VoxelGridIndex3D> VoxelIndexVector;
				typedef ::boost::multi_array<double, 2> BoostArray2D;
				typedef ::boost::shared_ptr<::boost::lockfree::queue<BoostArray2D*>>
				        VoxelizationQueuePointer;
				typedef ::boost::shared_ptr<::boost::lockfree::queue<double>>
				        VoxelizationIndexQueuePointer;

				BoostMaskVoxelizationThread(const BoostPolygonMap& APolygonMap,
				                            const VoxelIndexVector& aGlobalBoundingBox, VoxelizationIndexQueuePointer aResultIndexQueue,
				                            VoxelizationQueuePointer aVoxelizationQueue);


				void operator()();


			private:


				typedef std::deque<BoostPolygon2D> BoostPolygonDeque;
				typedef ::boost::geometry::model::ring< ::boost::geometry::model::d2::point_xy<double> >
				BoostRing2D;
				typedef ::boost::geometry::model::d2::point_xy<double> BoostPoint2D;


				BoostPolygonMap _geometryCoordinateBoostPolygonMap;
				VoxelIndexVector _globalBoundingBox;
				VoxelizationQueuePointer _resultVoxelizationQueue;
				VoxelizationIndexQueuePointer _resultIndexQueue;

				/*! @brief Get intersection polygons of the contour and a voxel polygon
				* @param aVoxelIndex3D The 3d grid index of the voxel
				* @param intersectionSlicePolygons The polygons of the slice intersecting the voxel
				* @return Return all intersetion polygons of the structure and the voxel
				*/
				BoostPolygonDeque getIntersections(const rttb::VoxelGridIndex3D& aVoxelIndex3D,
				                                   const BoostPolygonVector& intersectionSlicePolygons) const;

				/*! @brief Get the voxel 2d contour polygon in geometry coordinate*/
				BoostRing2D get2DContour(const rttb::VoxelGridIndex3D& aVoxelGrid3D) const;

				/*! @brief Calculate the area of all polygons
				* @param aPolygonDeque The deque of polygons
				* @return Return the area of all polygons
				*/
				double calcArea(const BoostPolygonDeque& aPolygonDeque) const;
			};

		}


	}
}

#endif