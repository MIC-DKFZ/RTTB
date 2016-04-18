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


#ifndef __BOOST_MASK_R_H
#define __BOOST_MASK_R_H

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
#include <boost/multi_array.hpp>

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
				typedef ::boost::geometry::model::polygon< ::boost::geometry::model::d2::point_xy<double> >
				BoostPolygon2D;
				typedef ::boost::geometry::model::ring< ::boost::geometry::model::d2::point_xy<double> >
				BoostRing2D;
				typedef std::deque<BoostPolygon2D> BoostPolygonDeque;
				typedef std::vector<BoostRing2D> BoostRingVector;//polygon without holes
				typedef std::vector<BoostPolygon2D> BoostPolygonVector;//polygon with or without holes
				typedef std::vector<rttb::VoxelGridIndex3D> VoxelIndexVector;
				typedef std::map<double, BoostPolygonVector>
				BoostPolygonMap;//map of the z index with the vector of boost 2d polygon
				typedef std::map<double, BoostRingVector>
				BoostRingMap;//map of the z index with the vector of boost 2d ring
				typedef ::boost::multi_array<double, 2> BoostArray2D;
				typedef std::map<double, BoostArray2D> BoostArrayMap;
				typedef ::boost::array<double, 1> BoostArray1D;
				typedef std::vector<std::map<double, double>> WeightVector;

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
				VoxelIndexVector _globalBoundingBox;

				/*! @brief The voxelization map
				*	key: the converted double z grid index of a contour plane
				*	value: the 2d mask, array[i][j] = the mask value of the position (i,j) in the global bounding box,
				*			i: 0 - (_globalBoundingBoxSize0-1), j: 0 - (_globalBoundingBoxSize1-1)
				*/
				BoostArrayMap _voxelizationMap;

				//The thickness of the voxelization plane (the contour plane), in double dose grid index
				//(for example, the first contour has the double grid index 0.1, the second 0.3, the third 0.5, then the thickness is 0.2)
				double _voxelizationThickness;

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
				*	For all contours in a struct:
				*	1) Transfer the contour polygons into boost::geometry structures
				*		1a) Convert the contur points from world coordinates into geometry coordinates.
				*		1b) get min and max for x/y/z of a contour
				*	2) Tilt check: if difference of z_min and z_max is larger then a tolerance value -> there is a tilt. Throw rttb::TiltedMaskPlaneException.
				*	3) Get struct-bounding-box: get x_min_struct, y_min_struct, x_max_struct, y_max_struct to define the bounding box that containes all contours of a struct in x-y-dimensions.
				*/
				void preprocessing();

				/*! @brief The voxelization step, wich computes the voxelization planes (in x/y) for all contours of an struct.

				*	For each contour (that is in the z-Range of the reference geometry) of the struct:
				*	1) Allocate result array (voxelization plane) based on the bounding box (see Preprocessing Step 3)
				*	2) Generate voxelization plane for the contour (based on the x-y-raster of the reference geometry).
				*	3) Add result Array (key is the z-Value of the contour)
				*/
				void voxelization();

				/*! @final mask voxel Generation step which transfers the voxilization planes into the (z-)geometry of the reference geometry.
				*	It consists of following Sub steps :
				*	For all "slices" in the reference geometry :
				*	1) generate weight vector for all voxelization planes for a given z - value of a slice
				*		Itterate over the bounding box of a struct.For each voxel :
				*	2) Compute weighted sum of all voxelization planes(use weight vector, step 1)
				*		2a) If sum > 0 : Add mask voxel for the current x / y(inner Loop) and z value(outer Loop).
				*	3) return mask voxel list.
				*/
				void generateMaskVoxelList();

				/*! @brief Convert the rttb polygon with world corrdinate to the rttb polygon with double geometry coordinate, calculate the current min/max
				*			and check if the polygon is planar
				*	@param minimum the current global minimum
				*	@param maximum the current global maximum
				*	@return Return true if the polygon is planar, which means that the minminal and maximal z-coordinate of the polygon is not larger than a error constant
				*/
				bool preprocessingPolygon(const rttb::PolygonType& aRTTBPolygon,
				                          rttb::PolygonType& geometryCoordinatePolygon, rttb::DoubleVoxelGridIndex3D& minimum,
				                          rttb::DoubleVoxelGridIndex3D& maximum, double aErrorConstant) const;

				/*! @brief Convert a rttb 3d polygon to a 2d boost ring*/
				BoostRing2D convertRTTBPolygonToBoostRing(const rttb::PolygonType& aRTTBPolygon) const;

				/*! @brief Convert a rttb 3d polygon to a map of z index with a vector of boost 2d ring, because of tilt check use the first z index of the polygon as the map key*/
				BoostRingMap convertRTTBPolygonSequenceToBoostRingMap(const rttb::PolygonSequenceType&
				        aRTTBPolygonVector);

				/*! @brief Find the key with error constant to aIndex
				*	@pre aBoostRingMap should not be empty
				*	@return Return aBoostRingMap.end() if the key is not found
				*/
				BoostMask::BoostRingMap::iterator findNearestKey(BoostMask::BoostRingMap& aBoostRingMap,
				        double aIndex, double aErrorConstant);

				/*! @brief If 2 rings in the vector build a donut, convert the 2 rings to a donut polygon, other rings unchanged*/
				BoostPolygonVector checkDonutAndConvert(const BoostRingVector& aRingVector) const;

				/*! @brief Get the voxel 2d contour polygon in geometry coordinate*/
				BoostRing2D get2DContour(const rttb::VoxelGridIndex3D& aVoxelGrid3D) const;

				/*! @brief Get intersection polygons of the contour and a voxel polygon
				* @param aVoxelIndex3D The 3d grid index of the voxel
				* @param intersectionSlicePolygons The polygons of the slice intersecting the voxel
				* @return Return all intersetion polygons of the structure and the voxel
				*/
				BoostPolygonDeque getIntersections(const rttb::VoxelGridIndex3D& aVoxelIndex3D,
				                                   const BoostPolygonVector& intersectionSlicePolygons) const;

				/*! @brief Calculate the area of all polygons
				* @param aPolygonDeque The deque of polygons
				* @return Return the area of all polygons
				*/
				double calcArea(const BoostPolygonDeque& aPolygonDeque) const;

				/*! @brief Calculate the voxelization thickness.
				Return false, if the voxelization plane is not homogeneous
				*/
				bool calcVoxelizationThickness(double& aThickness) const;

				/*! @brief For each dose grid index z, calculate the weight vector for each structure contour
				*/
				void calcWeightVector(const rttb::VoxelGridID& aIndexZ,
				                      std::map<double, double>& weightVector) const;
			};

		}


	}
}

#endif