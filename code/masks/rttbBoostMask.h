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

#ifndef __BOOST_MASK_R_H
#define __BOOST_MASK_R_H

#include "rttbBaseType.h"
#include "rttbStructure.h"
#include "rttbGeometricInfo.h"
#include "rttbMaskAccessorInterface.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/multi_array.hpp>

namespace rttb
{
	namespace masks
	{
		namespace boost
		{
			/*! @class BoostMask
			*   @brief Implementation of voxelization using boost::geometry.
			*   @attention If "strict" is set to true, an exception will be thrown when the given structure has self intersection.
			*   (A structure without self intersection means all contours of the structure have no self intersection, and
			*   the polygons on the same slice have no intersection between each other, unless the case of a donut. A donut is accepted.)
			*   If "strict" is set to false, debug information will be displayed when the given structure has self intersection. Self intersections will be ignored
			*   and the mask will be calculated, however, it may cause errors in the mask results.
			*/
			class BoostMask
			{

			public:
				using MaskVoxelList = core::MaskAccessorInterface::MaskVoxelList;
				using MaskVoxelListPointer = core::MaskAccessorInterface::MaskVoxelListPointer;

				/*! @brief Constructor
				* @exception rttb::core::NullPointerException thrown if aDoseGeoInfo or aStructure is nullptr
                * @param aDoseGeoInfo the GeometricInfo
                * @param aStructure the structure set
				* @param strict indicates whether to allow self intersection in the structure. If it is set to true, an exception will be thrown when the given structure has self intersection.
				* @param numberOfThreads number of threads used for voxelization. default value 0 means automatic detection, using the number of Hardware thread/cores
				* @exception InvalidParameterException thrown if strict is true and the structure has self intersections
				*/
				BoostMask(core::GeometricInfo::Pointer aDoseGeoInfo, core::Structure::Pointer aStructure,
				          bool strict = true, unsigned int numberOfThreads = 0);

				/*! @brief Generate mask and return the voxels in the mask
				* @exception rttb::core::InvalidParameterException thrown if the structure has self intersections
				*/
				MaskVoxelListPointer getRelevantVoxelVector();

			private:
				using BoostPoint2D = ::boost::geometry::model::d2::point_xy<double>;
				using BoostPolygon2D = ::boost::geometry::model::polygon< ::boost::geometry::model::d2::point_xy<double> >;
				using BoostRing2D = ::boost::geometry::model::ring< ::boost::geometry::model::d2::point_xy<double> >;
				using BoostRingVector = std::vector<BoostRing2D>;//polygon without holes
				using BoostPolygonVector = std::vector<BoostPolygon2D>;//polygon with or without holes
				using VoxelIndexVector = std::vector<rttb::VoxelGridIndex3D>;
				typedef std::map<double, BoostPolygonVector>
				BoostPolygonMap;//map of the z index with the vector of boost 2d polygon
				typedef std::map<double, BoostRingVector>
				BoostRingMap;//map of the z index with the vector of boost 2d ring
				typedef ::boost::multi_array<double, 2> BoostArray2D;
                using BoostArray2DPointer = ::boost::shared_ptr<BoostArray2D>;
                typedef ::boost::shared_ptr<std::map<double, BoostArray2DPointer> > BoostArrayMapPointer;

        core::GeometricInfo::Pointer _geometricInfo;

        core::Structure::Pointer _structure;

        bool _strict;

        /*! @brief The number of threads
        */
        unsigned int _numberOfThreads;

        //@brief The thickness of the voxelization plane (the contour plane), in double dose grid index
        //@details for example, the first contour has the double grid index 0.1, the second 0.3, the third 0.5, then the thickness is 0.2
        double _voxelizationThickness;

        //@brief vector of the MaskVoxel inside the structure
        MaskVoxelListPointer _voxelInStructure;

				/*! @brief The map of z index and a vector of boost ring 2d (without holes)
				*	@details Key: the double z grid index
				*	Value: the vector of boost ring 2d (without holes)
				*/
				BoostRingMap _ringMap;

				/*! @brief The min and max index of the global bounding box.
				*	@details The first index has the minimum for x/y/z of the global bounding box.
				*	The second index has the maximum for x/y/z of the global bounding index.
				*/
				VoxelIndexVector _globalBoundingBox;

				/*! @brief The voxelization map
				*	@details key: the converted double z grid index of a contour plane
				*	value: the 2d mask, array[i][j] = the mask value of the position (i,j) in the global bounding box,
				*			i: 0 - (_globalBoundingBoxSize0-1), j: 0 - (_globalBoundingBoxSize1-1)
				*/
                BoostArrayMapPointer _voxelizationMap;

				/*! @brief If the mask is up to date
				*/
				bool _isUpToDate;

				/*! @brief Voxelization and generate mask
				*/
				void calcMask();

				/*! @brief The preprocessing step, wich consists of the following logic and Sub setps:
				*	@details For all contours in a struct:
				*	1) Transfer the contour polygons into boost::geometry structures
				*		1a) Convert the contur points from world coordinates into geometry coordinates.
				*		1b) get min and max for x/y/z of a contour
				*	2) Tilt check: if difference of z_min and z_max is larger then a tolerance value -> there is a tilt. Throw rttb::TiltedMaskPlaneException.
				*	3) Get struct-bounding-box: get x_min_struct, y_min_struct, x_max_struct, y_max_struct to define the bounding box that containes all contours of a struct in x-y-dimensions.
				*/
				void preprocessing();

				/*! @brief The voxelization step, which computes the voxelization planes (in x/y) for all contours of an struct.

				*	@details For each contour (that is in the z-Range of the reference geometry) of the struct:
				*	1) Allocate result array (voxelization plane) based on the bounding box (see Preprocessing Step 3)
				*	2) Generate voxelization plane for the contour (based on the x-y-raster of the reference geometry).
				*	3) Add result Array (key is the z-Value of the contour)
				*/
				void voxelization();

				/*! @brief mask voxel Generation step which transfers the voxelization planes into the (z-)geometry of the reference geometry.
				*	@details It consists of following Sub steps :
				*	For all "slices" in the reference geometry :
				*	1) generate weight vector for all voxelization planes for a given z - value of a slice
				*		Iterate over the bounding box of a struct.For each voxel :
				*	2) Compute weighted sum of all voxelization planes(use weight vector, step 1)
				*		2a) If sum > 0 : Add mask voxel for the current x / y(inner Loop) and z value(outer Loop).
				*	3) return mask voxel list.
				*/
				void generateMaskVoxelList();

				/*! @brief Convert the rttb polygon with world coordinate to the rttb polygon with double geometry coordinate, calculate the current min/max
				*			and check if the polygon is planar
				*	@param minimum the current global minimum
				*	@param maximum the current global maximum
				*	@return Return true if the polygon is planar, which means that the minimal and maximal z-coordinate of the polygon is not larger than a error constant
				*/
				bool preprocessingPolygon(const rttb::PolygonType& aRTTBPolygon,
				                          rttb::PolygonType& geometryCoordinatePolygon, rttb::DoubleVoxelGridIndex3D& minimum,
				                          rttb::DoubleVoxelGridIndex3D& maximum, double aErrorConstant) const;

				/*! @brief Convert a rttb 3d polygon to a 2d boost ring*/
				BoostRing2D convertRTTBPolygonToBoostRing(const rttb::PolygonType& aRTTBPolygon) const;

				/*! @brief Convert a rttb 3d polygon to a map of z index with a vector of boost 2d ring, because of tilt check use the first z index of the polygon as the map key*/
				BoostRingMap convertRTTBPolygonSequenceToBoostRingMap(const rttb::PolygonSequenceType&
				        aRTTBPolygonVector) const;

				/*! @brief Find the key with error constant to aIndex
				*	@pre aBoostRingMap should not be empty
				*	@return Return aBoostRingMap.end() if the key is not found
				*/
				BoostMask::BoostRingMap::iterator findNearestKey(BoostMask::BoostRingMap& aBoostRingMap,
				        double aIndex, double aErrorConstant) const;

				/*! @brief If 2 rings in the vector build a donut, convert the 2 rings to a donut polygon, other rings unchanged*/
				BoostPolygonVector checkDonutAndConvert(const BoostRingVector& aRingVector) const;

				/*! @brief Calculate the voxelization thickness.
				Return false, if the voxelization plane is not homogeneous
				*/
				bool calcVoxelizationThickness(double& aThickness) const;

			};

		}


	}
}

#endif
