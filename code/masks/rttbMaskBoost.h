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


#ifndef __RT_MASK_BOOST_H
#define __RT_MASK_BOOST_H

#include "rttbBaseType.h"
#include "rttbStructure.h"
#include "rttbGeometricInfo.h"
#include "rttbMaskVoxel.h"
#include "rttbMaskAccessorInterface.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>

namespace rttb{
	namespace masks{

		class MaskBoost{	

		public:
			typedef boost::shared_ptr<core::GeometricInfo> GeometricInfoPtr;
			typedef core::Structure::StructTypePointer StrPtr;
			typedef core::MaskAccessorInterface::MaskVoxelList MaskVoxelList;
			typedef core::MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;
			typedef boost::geometry::model::d2::point_xy<double> BoostPoint2D;
			typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > BoostPolygon2D;
			typedef boost::geometry::model::ring<boost::geometry::model::d2::point_xy<double> > BoostRing2D;

			MaskBoost(GeometricInfoPtr aDoseGeoInfo, StrPtr aStructure);

			MaskVoxelListPointer getRelevantVoxelVector();

		private:

			GeometricInfoPtr _geometricInfo;

			StrPtr _structure;

			 //vector of the MaskVoxel inside the structure
			MaskVoxelListPointer _voxelInStructure;

			/* Check if the structure has self intersections*/
			bool hasSelfIntersections();


			/*Get the min/max voxel index of the bounding box of the polygon in the slice with sliceNumber*/
			std::vector<rttb::VoxelGridIndex3D> getBoundingBox(unsigned int sliceNumber);


			/*Get intersection polygons of the contour and a voxel polygon*/
			std::deque<BoostPolygon2D> getIntersections(const rttb::VoxelGridIndex3D& aVoxelIndex3D);


			/*Calculate the intersection area*/
			double calcArea(std::deque<BoostPolygon2D> aPolygonDeque);

			VoxelGridIndex3D getGridIndex3D(const core::MaskVoxel& aMaskVoxel);


			/*Convert RTTB polygon to boost polygon*/
			BoostRing2D convert(const rttb::PolygonType& aRTTBPolygon);

			/*Get the intersection slice of the voxel, return the polygons in the slice*/
			std::vector<BoostRing2D> getIntersectionSlicePolygons(const rttb::VoxelGridIndex3D& aVoxelGrid3D);

			/*Get the voxel 2d contour polygon*/
			BoostRing2D get2DContour(const rttb::VoxelGridIndex3D& aVoxelGrid3D);

		};

	}
}

#endif