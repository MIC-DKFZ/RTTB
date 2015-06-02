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
// @version $Revision: 707 $ (last changed revision)
// @date    $Date: 2014-09-04 16:37:24 +0200 (Do, 04 Sep 2014) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/


#ifndef __BOOST_MASK_H
#define __BOOST_MASK_H

#include "rttbBaseType.h"
#include "rttbStructure.h"
#include "rttbGeometricInfo.h"
#include "rttbMaskVoxel.h"
#include "rttbMaskAccessorInterface.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/shared_ptr.hpp>

namespace rttb
{
	namespace masks
	{
		namespace boost
		{
			class BoostMask
			{

			public:
				typedef ::boost::shared_ptr<rttb::core::GeometricInfo> GeometricInfoPointer;
				typedef core::Structure::StructTypePointer StructPointer;
				typedef core::MaskAccessorInterface::MaskVoxelList MaskVoxelList;
				typedef core::MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;

				/*! @brief Constructor
				* @exception rttb::core::NullPointerException thrown if aDoseGeoInfo or aStructure is NULL
				*/
				BoostMask(GeometricInfoPointer aDoseGeoInfo, StructPointer aStructure);

				/*! @brief Generate mask and return the voxels in the mask
				* @exception rttb::core::InvalidParameterException thrown if the structure has self intersections
				*/
				MaskVoxelListPointer getRelevantVoxelVector();

			private:
				typedef ::boost::geometry::model::d2::point_xy<double> BoostPoint2D;
				typedef ::boost::geometry::model::polygon< ::boost::geometry::model::d2::point_xy<double> > BoostPolygon2D;
				typedef ::boost::geometry::model::ring< ::boost::geometry::model::d2::point_xy<double> > BoostRing2D;
				typedef std::deque<BoostPolygon2D> BoostPolygonDeque;
				typedef std::vector<BoostRing2D> BoostRingVector;
				typedef std::vector<rttb::VoxelGridIndex3D> VoxelIndexVector;

				GeometricInfoPointer _geometricInfo;

				StructPointer _structure;

				//vector of the MaskVoxel inside the structure
				MaskVoxelListPointer _voxelInStructure;

				/*! @brief If the mask is up to date
				*/
				bool _isUpToDate;

				/*! @brief Voxelization and generate mask
				*/
				void calcMask();

				/*! @brief Check if the structure has self intersections*/
				bool hasSelfIntersections();

				/*! @brief Get the min/max voxel index of the bounding box of the polygon in the slice with sliceNumber
				 * @param sliceNumber slice number between 0 and _geometricInfo->getNumSlices()
				 * @param intersectionSlicePolygons Get the polygons intersecting the slice
				 * @exception InvalidParameterException thrown if sliceNumber < 0 or sliceNumber >=  _geometricInfo->getNumSlices()
				 * @return Return the 4 voxel index of the bounding box
				*/
				VoxelIndexVector getBoundingBox(unsigned int sliceNumber, const BoostRingVector& intersectionSlicePolygons);

				/*! @brief Get intersection polygons of the contour and a voxel polygon
				 * @param aVoxelIndex3D The 3d grid index of the voxel
				 * @param intersectionSlicePolygons The polygons of the slice intersecting the voxel
				 * @return Return all intersetion polygons of the structure and the voxel
				*/
				BoostPolygonDeque getIntersections(const rttb::VoxelGridIndex3D& aVoxelIndex3D,
				                                   const BoostRingVector& intersectionSlicePolygons);

				/*! @brief Calculate the area of all polygons
				 * @param aPolygonDeque The deque of polygons
				 * @return Return the area of all polygons
				*/
				double calcArea(const BoostPolygonDeque& aPolygonDeque);

				/*! @brief Get grid index of a mask voxel
				 * @param aMaskVoxel A mask voxel
				 * @return Return the 3d grid index of the mask voxel
				*/
				VoxelGridIndex3D getGridIndex3D(const core::MaskVoxel& aMaskVoxel);

				/*! @brief Convert RTTB polygon to boost polygon*/
				BoostRing2D convert(const rttb::PolygonType& aRTTBPolygon);

				/*! @brief Get the intersection slice of the voxel, return the polygons in the slice
				 * @param aVoxelGridIndexZ The z grid index (slice number) of the voxel
				 * @return Return the structure polygons intersecting the slice
				*/
				BoostRingVector getIntersectionSlicePolygons(const rttb::GridIndexType aVoxelGridIndexZ);

				/*! @brief Get the voxel 2d contour polygon*/
				BoostRing2D get2DContour(const rttb::VoxelGridIndex3D& aVoxelGrid3D);

			};

		}


	}
}

#endif