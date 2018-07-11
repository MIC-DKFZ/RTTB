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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

#ifndef __GEOMETRIC_INFO_NEW_H
#define __GEOMETRIC_INFO_NEW_H


#include <iostream>
#include <sstream>

#include "rttbBaseType.h"
#include <boost/numeric/ublas/vector.hpp>

#include "rttbCommon.h"

#include "RTTBCoreExports.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{

	namespace core
	{
		/*! @brief GeometricInfo objects contain all the information required for transformations between voxel grid
			coordinates and world coordinates. Corresponding converter functions are also available.
			@note ITK Pixel Indexing used (http://www.itk.org/Doxygen45/html/classitk_1_1Image.html): The Index type reverses the order so that with Index[0] = col, Index[1] = row, Index[2] = slice.

		*/
		class RTTBCore_EXPORT GeometricInfo
		{
    public:
      rttbClassMacroNoParent(GeometricInfo)
		private:
      WorldCoordinate3D _imagePositionPatient{ 0 };

      OrientationMatrix _orientationMatrix{ 0 };
      OrientationMatrix _invertedOrientationMatrix{ 0 };

      SpacingVectorType3D _spacing{ 0 };

			VoxelGridDimensionType _numberOfColumns{0};
			VoxelGridDimensionType _numberOfRows{0};
			VoxelGridDimensionType _numberOfFrames{0};

			/* @brief Matrix inversion routine.
			   Uses lu_factorize and lu_substitute in uBLAS to invert a matrix
			   http://savingyoutime.wordpress.com/2009/09/21/c-matrix-inversion-boostublas/
			*/
			bool computeInvertOrientation();

		public:
			/*! @brief Constructor, initializes orientation matrix, spacing vector and patient
				position with zeros.
			*/
      GeometricInfo()
			= default;

			void setSpacing(const SpacingVectorType3D& aSpacingVector);

			const SpacingVectorType3D& getSpacing() const;

			void setImagePositionPatient(const WorldCoordinate3D& aImagePositionPatient);

			const WorldCoordinate3D& getImagePositionPatient() const;

			void setOrientationMatrix(const OrientationMatrix& anOrientationMatrix);

			const OrientationMatrix getOrientationMatrix() const
			{
				return _orientationMatrix;
			};

			void setImageSize(const ImageSize& aSize);

			const ImageSize getImageSize() const;

			void setNumColumns(const VoxelGridDimensionType aValue);

			const VoxelGridDimensionType getNumColumns() const;

			void setNumRows(const VoxelGridDimensionType aValue);

			const VoxelGridDimensionType getNumRows() const;

			void setNumSlices(const VoxelGridDimensionType aValue);

			const VoxelGridDimensionType getNumSlices() const;

			/*! @brief determines equality of two GeometricInfo objects.
			*/
      friend bool RTTBCore_EXPORT operator == (const GeometricInfo& gInfo, const GeometricInfo& gInfo1);

			bool equalsAlmost(const GeometricInfo& another, double errorConstantGI = 1e-5) const;

			/*! @brief converts world coordinates to voxel grid index.
                @details the voxels world coordinates are defined by spacing, orientation and imagePositionPatient.
                         (-0.5/-0.5/-0.5) --> (0/0/0) and (0.4999/0.4999/0.4999) --> (0/0/0) define the outer coordinates of a voxel
                         with spacing=1, orientation= x y z (identity matrix) and imagePositionPatient=(0/0/0). 
                @sa WorldCoordinate3D VoxelGridIndex3D
				@note The conversion of values is done even if the target index is not inside the given voxel grid.
                @returns false if aWorldCoordinate is outside the voxel grid, true otherwise.
			*/
			bool worldCoordinateToIndex(const WorldCoordinate3D& aWorldCoordinate,
			                            VoxelGridIndex3D& aIndex) const;

			/*! @brief converts world coordinates to double geometry coordinate. 
                @details This is needed because of a double precision voxel coordinate system for voxelization. 
                The world coordinate of the image position patient is the center of the first voxel (0.0/0.0/0.0).
                (-0.5/-0.5/-0.5) --> (-0.5/-0.5/-0.5) and (0.4999/0.4999/0.4999) --> (0.4999/0.4999/0.4999)
                with spacing=1, orientation= x y z (identity matrix) and imagePositionPatient=(0/0/0).
                @sa WorldCoordinate3D, DoubleVoxelGridIndex3D
                @note The conversion of values is done even if the target index is not inside the given voxel grid.
                @returns false if aWorldCoordinate is outside the voxel grid, true otherwise.
			*/
			bool worldCoordinateToGeometryCoordinate(const WorldCoordinate3D& aWorldCoordinate,
			        DoubleVoxelGridIndex3D& aIndex) const;

			/*! @brief converts double geometry coordinate to world coordinates. 
                @details This is needed because of a double precision voxel coordinate system for voxelization.
                The world coordinate of the image position patient is the center of the first voxel (0.0/0.0/0.0).
                (-0.5/-0.5/-0.5) --> (-0.5/-0.5/-0.5) and (5.5/3.2/1.0) --> (5.5/3.2/1.0)
                with spacing=1, orientation= x y z (identity matrix) and imagePositionPatient=(0/0/0).
                @sa DoubleVoxelGridIndex3D, WorldCoordinate3D
                @note The conversion of values is done even if the target index is not inside the given voxel grid.
                @returns false if aWorldCoordinate is outside the voxel grid, true otherwise.
			*/
			bool geometryCoordinateToWorldCoordinate(const DoubleVoxelGridIndex3D& aIndex,
			        WorldCoordinate3D& aWorldCoordinate) const;

			/*! @brief convert voxel grid index to world coordinates
                @details The world coordinate of the image position patient (center of the first voxel) is the center of the first voxel (0.0/0.0/0.0)
                (0/0/0) --> (0.0/0.0/0.0) and (1/1/2) --> (1.0/1.0/2.0) with spacing=1, orientation= x y z (identity matrix) and imagePositionPatient=(0/0/0).
                Thus, the center of the voxel is taken and converted.
                @sa VoxelGridIndex3D, WorldCoordinate3D
                @note The conversion of values is done even if the target index is not inside the given voxel grid.
                @returns false if aWorldCoordinate is outside the voxel grid, true otherwise.
			*/
			bool indexToWorldCoordinate(const VoxelGridIndex3D& aIndex,
			                            WorldCoordinate3D& aWorldCoordinate) const;

			/*! @brief check if a given voxel grid index is inside the given voxel grid.*/
			bool isInside(const VoxelGridIndex3D& aIndex) const;

			/*! @brief check if a given world coordinate is inside the given voxel grid.*/
			bool isInside(const WorldCoordinate3D& aWorldCoordinate) const;

			const GridSizeType getNumberOfVoxels() const;

			bool convert(const VoxelGridID& gridID, VoxelGridIndex3D& gridIndex) const;

			bool convert(const VoxelGridIndex3D& gridIndex, VoxelGridID& gridID) const;

			/*! @brief test if given ID is inside current dose grid
			*/
			bool validID(const VoxelGridID aID) const;

			/*! @brief test if given index is inside current dose grid
			*/
			bool validIndex(const VoxelGridIndex3D& aIndex) const;

			/*!@ brief generates string stream representation of the GeometricInfo object. */
            friend std::ostream& operator << (std::ostream& s, const GeometricInfo& anGeometricInfo);
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
