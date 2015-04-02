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


#ifndef __RT_BOOST_MASK_H
#define __RT_BOOST_MASK_H

#include "rttbBaseType.h"
#include "rttbStructure.h"
#include "rttbGeometricInfo.h"
#include "rttbMaskVoxel.h"
#include "rttbMaskAccessorInterface.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>

namespace rttb
{
	namespace io
	{
		namespace mask
		{
			class BoostMask{	

			public:
				typedef boost::shared_ptr<core::GeometricInfo> GeometricInfoPointer;
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
				typedef boost::geometry::model::d2::point_xy<double> BoostPoint2D;
				typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > BoostPolygon2D;
				typedef boost::geometry::model::ring<boost::geometry::model::d2::point_xy<double> > BoostRing2D;
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

				/*! @brief Get the min/max voxel index of the bounding box of the polygon in the slice with sliceNumber*/
				VoxelIndexVector getBoundingBox(unsigned int sliceNumber);

				/*! @brief Get intersection polygons of the contour and a voxel polygon*/
				BoostPolygonDeque getIntersections(const rttb::VoxelGridIndex3D& aVoxelIndex3D);

				/*! @brief Calculate the intersection area*/
				double calcArea(BoostPolygonDeque aPolygonDeque);

				/*! @brief Get grid index of a mask voxel*/
				VoxelGridIndex3D getGridIndex3D(const core::MaskVoxel& aMaskVoxel);

				/*! @brief Convert RTTB polygon to boost polygon*/
				BoostRing2D convert(const rttb::PolygonType& aRTTBPolygon);

				/*! @brief Get the intersection slice of the voxel, return the polygons in the slice*/
				BoostRingVector getIntersectionSlicePolygons(const rttb::VoxelGridIndex3D& aVoxelGrid3D);

				/*! @brief Get the voxel 2d contour polygon*/
				BoostRing2D get2DContour(const rttb::VoxelGridIndex3D& aVoxelGrid3D);

			};

		}


	}
}

#endif