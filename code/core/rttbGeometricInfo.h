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

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "rttbBaseType.h"

namespace rttb
{

	namespace core
	{
		/*! @brief GeometricInfo objects contain all the information required for transformations between voxel grid
			coordinates and world coordinates. Corresponding converter functions are also available.
			@note ITK Pixel Indexing used (http://www.itk.org/Doxygen45/html/classitk_1_1Image.html): The Index type reverses the order so that with Index[0] = col, Index[1] = row, Index[2] = slice.

		*/
		class GeometricInfo
		{

		private:
			WorldCoordinate3D _imagePositionPatient;

			OrientationMatrix _orientationMatrix;
			OrientationMatrix _invertedOrientationMatrix;

			SpacingVectorType3D _spacing;

			VoxelGridDimensionType _numberOfColumns;
			VoxelGridDimensionType _numberOfRows;
			VoxelGridDimensionType _numberOfFrames;

			/* @brief Matrix inversion routine.
			   Uses lu_factorize and lu_substitute in uBLAS to invert a matrix
			   http://savingyoutime.wordpress.com/2009/09/21/c-matrix-inversion-boostublas/
			*/
			bool computeInvertOrientation();

		public:
			/*! @brief Constructor, initializes orientation matrix, spacing vector and patient
				position with zeros.
			*/
			GeometricInfo() : _orientationMatrix(0), _spacing(0), _imagePositionPatient(0), _numberOfFrames(0), _numberOfRows(0),
				_numberOfColumns(0) {}

			void setSpacing(const SpacingVectorType3D& aSpacingVector);

			const SpacingVectorType3D& getSpacing() const;

			void setImagePositionPatient(const WorldCoordinate3D& aImagePositionPatient);

			const WorldCoordinate3D& getImagePositionPatient() const;

			void setOrientationMatrix(const OrientationMatrix& anOrientationMatrix);

			const OrientationMatrix getOrientationMatrix() const
			{
				return _orientationMatrix;
			};

			/*! @brief Returns the 1st row of the OrientationMatrix.
				@deprecated please use getOrientationMatrix() (x,0) instead*/
			const ImageOrientation getImageOrientationRow() const;

			/*! @brief Returns the 2nd row of the OrientationMatrix.
				@deprecated please use getOrientationMatrix() (x,1) instead*/
			const ImageOrientation getImageOrientationColumn() const;

			void setPixelSpacingRow(const GridVolumeType aValue);

			/*! @brief Returns the spacing in the x-dimension (rows) of the data grid.
				@deprecated please use getSpacing() (0) instead*/
			const GridVolumeType getPixelSpacingRow() const;

			void setPixelSpacingColumn(const GridVolumeType aValue);

			/*! @brief Returns the spacing in the y-dimension (columns) of the data grid.
				@deprecated please use getSpacing() (1) instead*/
			const GridVolumeType getPixelSpacingColumn() const;

			void setSliceThickness(const GridVolumeType aValue);

			/*! @brief Returns the spacing in the z-dimension (slices) of the data grid.
				@deprecated please use getSpacing() (2) instead*/
			const GridVolumeType getSliceThickness() const;

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
			friend bool operator==(const GeometricInfo& gInfo, const GeometricInfo& gInfo1);

			/*! @brief convert world coordinates to voxel grid index.
				The conversion of values is done even if the target index is not inside the given voxel grid (return false).
				If the target is inside the grid return true.
			*/
			bool worldCoordinateToIndex(const WorldCoordinate3D& aWorldCoordinate,
			                            VoxelGridIndex3D& aIndex) const;
			/*! @brief voxel grid index to convert world coordinates.
				The conversion of values is done even if the target is not inside the given voxel grid (return false).
				If the target is inside the voxel grid return true.
			*/
			bool indexToWorldCoordinate(const VoxelGridIndex3D& aIndex ,
			                            WorldCoordinate3D& aWorldCoordinate) const;

			/*! @brief check if a given voxel grid index is inside the given voxel grid.*/
			bool isInside(const VoxelGridIndex3D& aIndex) const;

			/*! @brief check if a given world coordinate is inside the given voxel grid.*/
			bool isInside(const WorldCoordinate3D& aWorldCoordinate);

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
			friend std::ostream& operator<<(std::ostream& s, const GeometricInfo& anGeometricInfo);
		};
	}
}

#endif