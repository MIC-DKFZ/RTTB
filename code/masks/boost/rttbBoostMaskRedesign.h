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
		namespace boostRedesign
		{
			/*! @class BoostMask
			*   @brief Implementation of voxelization using boost::geometry. 
			*   @attention If "strict" is set to true, an exception will be thrown when the given structure has self intersection.
			*   (A structure without self interseciton means all contours of the structure have no self intersection, and
			*   the polygons on the same slice have no intersection between each other, unless the case of a donut. A donut is accepted.)   
			*   If "strict" is set to false, debug information will be displayed when the given structure has self intersection. Self intersections will be ignored 
			*   and the mask will be calculated, however, it may cause errors in the mask results. 
			*/
			class BoostMask
			{

			public:
				typedef ::boost::shared_ptr<rttb::core::GeometricInfo> GeometricInfoPointer;
				typedef core::Structure::StructTypePointer StructPointer;
				typedef core::MaskAccessorInterface::MaskVoxelList MaskVoxelList;
				typedef core::MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;

				/*! @brief Constructor
				* @exception rttb::core::NullPointerException thrown if aDoseGeoInfo or aStructure is NULL
				* @param strict indicates whether to allow self intersection in the structure. If it is set to true, an exception will be thrown when the given structure has self intersection.
				* @exception InvalidParameterException thrown if strict is true and the structure has self intersections
				*/
				BoostMask(GeometricInfoPointer aDoseGeoInfo, StructPointer aStructure, bool strict = true);

				/*! @brief Generate mask and return the voxels in the mask
				* @exception rttb::core::InvalidParameterException thrown if the structure has self intersections
				*/
				MaskVoxelListPointer getRelevantVoxelVector();

			private:
				typedef ::boost::geometry::model::d2::point_xy<double> BoostPoint2D;
				typedef ::boost::geometry::model::polygon< ::boost::geometry::model::d2::point_xy<double> > BoostPolygon2D;
				typedef ::boost::geometry::model::ring< ::boost::geometry::model::d2::point_xy<double> > BoostRing2D;
				typedef std::deque<BoostPolygon2D> BoostPolygonDeque;
				typedef std::vector<BoostRing2D> BoostRingVector;//polygon without holes
				typedef std::vector<BoostPolygon2D> BoostPolygonVector;//polygon with or without holes
				typedef std::vector<rttb::VoxelGridIndex3D> VoxelIndexVector;
				typedef std::map<double, BoostPolygonVector> BoostPolygonMap;//map of the z index with the vector of boost 2d polygon
				typedef std::map<double, BoostRingVector> BoostRingMap;//map of the z index with the vector of boost 2d ring

				GeometricInfoPointer _geometricInfo;

				StructPointer _structure;

				/*! @brief The map of z index and a vector of all boost polygons with the same z index. This will be used to calculate the mask.
				*	Key: the double z grid index 
				*	Value: the vector of all boost 2d polygons with the same z grid index (donut polygon is accepted).
				*/
				BoostPolygonMap _geometryCoordinateBoostPolygonMap;

				/*! @brief The min and max index of the global bounding box. 
				*	The first index has the minimum for x/y/z of the global bounding box.
				*	The second index has the maximum for x/y/z of the global bounding index.
				*/
				std::vector<rttb::DoubleVoxelGridIndex3D> _globalBoundingBox;

				bool _strict;

				//vector of the MaskVoxel inside the structure
				MaskVoxelListPointer _voxelInStructure;

				/*! @brief If the mask is up to date
				*/
				bool _isUpToDate;

				/*! @brief Voxelization and generate mask
				*/
				void calcMask();

				/*! @brief The preprocessing step, wich consists of the following logic and Sub setps:
					*For all contours in a struct:
					*1) Transfer the contour polygons into boost::geometry structures
						*1a) Convert the contur points from world coordinates into geometry coordinates. 
						*1b) get min and max for x/y/z of a contour
					*2) Tilt check: if difference of z_min and z_max is larger then a tolerance value -> there is a tilt. Throw rttb::TiltedMaskPlaneException.
					*3) Get struct-bounding-box: get x_min_struct, y_min_struct, x_max_struct, y_max_struct to define the bounding box that containes all contours of a struct in x-y-dimensions.
				*/
				void preprocessing();

				/*! @brief Convert the rttb polygon with world corrdinate to the rttb polygon with double geometry coordinate
				*/
				rttb::PolygonType worldCoordinateToGeometryCoordinatePolygon(const rttb::PolygonType& aRTTBPolygon);

				/*! @brief Check if the polygon with the minimum and maximum is tilted more than a error constant
				@return Return true if tilted > aErrorConstant
				*/
				bool checkTilt(const rttb::DoubleVoxelGridIndex3D& minimum, const rttb::DoubleVoxelGridIndex3D& maximum, double aErrorConstant);

				/*! @brief Calculate minimum and maximum for x/y/z of the polygon
				*/
				void calcMinMax(const rttb::PolygonType& aRTTBPolygon, rttb::DoubleVoxelGridIndex3D minimum, rttb::DoubleVoxelGridIndex3D maximum);

				/*! @brief If 2 rttb polygons in the vector build a donut, convert the 2 rttb polygons to a donut boost polygon, other rttb polygons unchanged convert to boost ring*/
				

				/*! @brief Convert a rttb 3d polygon to a 2d boost ring*/
				BoostRing2D convertRTTBPolygonToBoostRing(const rttb::PolygonType& aRTTBPolygon);

				/*! @brief Convert a rttb 3d polygon to a map of z index with a vector of boost 2d ring, because of tilt check use the first z index of the polygon as the map key*/
				BoostRingMap convertRTTBPolygonSequenceToBoostRingMap(const rttb::PolygonSequenceType& aRTTBPolygonVector);
				
				/*! @brief If 2 rings in the vector build a donut, convert the 2 rings to a donut polygon, other rings unchanged*/
				BoostPolygonVector checkDonutAndConvert(const BoostRingVector& aRingVector);
			};

		}


	}
}

#endif