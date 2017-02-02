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

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "rttbGeometricInfo.h"

namespace rttb
{
	namespace core
	{

		void GeometricInfo::setSpacing(const SpacingVectorType3D& aSpacingVector)
		{
			_spacing = aSpacingVector;
		}

		const SpacingVectorType3D& GeometricInfo::getSpacing() const
		{
			return _spacing;
		}

		void GeometricInfo::setImagePositionPatient(const WorldCoordinate3D& aImagePositionPatient)
		{
			_imagePositionPatient = aImagePositionPatient;
		}

		const WorldCoordinate3D& GeometricInfo::getImagePositionPatient() const
		{
			return _imagePositionPatient;
		}

		void GeometricInfo::setOrientationMatrix(const OrientationMatrix& anOrientationMatrix)
		{
			_orientationMatrix = anOrientationMatrix;
			computeInvertOrientation();
		}

		bool GeometricInfo::computeInvertOrientation()
		{
			typedef boost::numeric::ublas::permutation_matrix<std::size_t> pmatrix;

			boost::numeric::ublas::matrix<WorldCoordinate> A(_orientationMatrix);

			// create a permutation matrix for the LU-factorization
			pmatrix pm(A.size1());

			size_t res = boost::numeric::ublas::lu_factorize(A, pm);

			if (res != 0)
			{
				return false;
			}

			_invertedOrientationMatrix.assign(boost::numeric::ublas::identity_matrix<WorldCoordinate>
			                                  (A.size1()));

			// backsubstitute to get the inverse
			boost::numeric::ublas::lu_substitute(A, pm, _invertedOrientationMatrix);

			return true;
		}

		const ImageOrientation GeometricInfo::getImageOrientationRow() const
		{
			ImageOrientation _imageOrientationRow(0);
			_imageOrientationRow(0) = _orientationMatrix(0, 0);
			_imageOrientationRow(1) = _orientationMatrix(1, 0);
			_imageOrientationRow(2) = _orientationMatrix(2, 0);
			return _imageOrientationRow;
		}

		const ImageOrientation GeometricInfo::getImageOrientationColumn() const
		{
			ImageOrientation _imageOrientationColumn(0);
			_imageOrientationColumn(0) = _orientationMatrix(0, 1);
			_imageOrientationColumn(1) = _orientationMatrix(1, 1);
			_imageOrientationColumn(2) = _orientationMatrix(2, 1);
			return _imageOrientationColumn;
		}

		void GeometricInfo::setPixelSpacingRow(const GridVolumeType aValue)
		{
			_spacing(0) = aValue;
		}

		const GridVolumeType GeometricInfo::getPixelSpacingRow() const
		{
			return _spacing(0);
		}

		void GeometricInfo::setPixelSpacingColumn(const GridVolumeType aValue)
		{
			_spacing(1) = aValue;
		}

		const GridVolumeType GeometricInfo::getPixelSpacingColumn() const
		{
			return _spacing(1);
		}

		void GeometricInfo::setSliceThickness(const GridVolumeType aValue)
		{
			_spacing(2) = aValue;
		}

		const GridVolumeType GeometricInfo::getSliceThickness() const
		{
			return _spacing(2);
		}


		void GeometricInfo::setImageSize(const ImageSize& aSize)
		{
			setNumColumns(aSize(0));
			setNumRows(aSize(1));
			setNumSlices(aSize(2));
		}

		const ImageSize GeometricInfo::getImageSize() const
		{
			return ImageSize(getNumColumns(), getNumRows(), getNumSlices());
		}

		void GeometricInfo::setNumColumns(const VoxelGridDimensionType aValue)
		{
			_numberOfColumns = aValue;
		}

		const VoxelGridDimensionType GeometricInfo::getNumColumns() const
		{
			return _numberOfColumns;
		}

		void GeometricInfo::setNumRows(const VoxelGridDimensionType aValue)
		{
			_numberOfRows = aValue;
		}

		const VoxelGridDimensionType GeometricInfo::getNumRows() const
		{
			return _numberOfRows;
		}

		void GeometricInfo::setNumSlices(const VoxelGridDimensionType aValue)
		{
			_numberOfFrames = aValue;
		}

		const VoxelGridDimensionType GeometricInfo::getNumSlices() const
		{
			return _numberOfFrames;
		}

		bool operator==(const GeometricInfo& gInfo, const GeometricInfo& gInfo1)
		{
			return (gInfo.getImagePositionPatient() == gInfo1.getImagePositionPatient()
			        && gInfo.getOrientationMatrix() == gInfo1.getOrientationMatrix()
			        && gInfo.getSpacing() == gInfo1.getSpacing() && gInfo.getNumColumns() == gInfo1.getNumColumns()
			        && gInfo.getNumRows() == gInfo1.getNumRows() && gInfo.getNumSlices() == gInfo1.getNumSlices());
		}


		bool GeometricInfo::equalsAlmost(const GeometricInfo& another,
		                                 double errorConstant /*= 1e-5*/) const
		{
			return (getImagePositionPatient().equalsAlmost(another.getImagePositionPatient(), errorConstant)
			        && getOrientationMatrix().equalsAlmost(another.getOrientationMatrix(), errorConstant)
			        && getSpacing().equalsAlmost(another.getSpacing(), errorConstant)
			        && getNumColumns() == another.getNumColumns()
			        && getNumRows() == another.getNumRows() && getNumSlices() == another.getNumSlices());
		}

		bool GeometricInfo::worldCoordinateToGeometryCoordinate(const WorldCoordinate3D& aWorldCoordinate,
		        DoubleVoxelGridIndex3D& aIndex)
		const
		{
			WorldCoordinate3D distanceToIP;
			distanceToIP = aWorldCoordinate - _imagePositionPatient;

			boost::numeric::ublas::vector<WorldCoordinate> result = boost::numeric::ublas::prod(
			            _invertedOrientationMatrix,
			            distanceToIP);

			boost::numeric::ublas::vector<WorldCoordinate> resultS = boost::numeric::ublas::element_div(result,
			        _spacing);

			aIndex = DoubleVoxelGridIndex3D(resultS(0), resultS(1), resultS(2));

            //if we convert DoubleVoxelGridIndex3D (double) to VoxelGridIndex3D (unsigned int), we can't find out if it's negative. 
            //So we have to check before.
            if (aIndex(0) < -0.5 || aIndex(1) < -0.5 || aIndex(2) < -0.5){
                return false;
            }
            else {
                //check if it is inside
                VoxelGridIndex3D indexInt = VoxelGridIndex3D(GridIndexType(aIndex(0) + 0.5),
                    GridIndexType(aIndex(1) + 0.5),
                    GridIndexType(aIndex(2) + 0.5));

                return isInside(indexInt);
            }
		}

		bool GeometricInfo::worldCoordinateToIndex(const WorldCoordinate3D& aWorldCoordinate,
		        VoxelGridIndex3D& aIndex)
		const
		{
			DoubleVoxelGridIndex3D doubleIndex;
			bool inside = worldCoordinateToGeometryCoordinate(aWorldCoordinate, doubleIndex);

			aIndex = VoxelGridIndex3D(GridIndexType(doubleIndex(0)+0.5), GridIndexType(doubleIndex(1)+0.5),
			                          GridIndexType(doubleIndex(2)+0.5));

			return inside;
		}


		bool GeometricInfo::geometryCoordinateToWorldCoordinate(const DoubleVoxelGridIndex3D& aIndex,
		        WorldCoordinate3D& aWorldCoordinate)
		const
		{

			boost::numeric::ublas::vector<WorldCoordinate> resultS = boost::numeric::ublas::element_prod(
			            aIndex,
			            _spacing);

			boost::numeric::ublas::vector<WorldCoordinate> result = boost::numeric::ublas::prod(
			            _orientationMatrix,
			            resultS);

			aWorldCoordinate = result + _imagePositionPatient;

            //if we convert DoubleVoxelGridIndex3D (double) to VoxelGridIndex3D (unsigned int), we can't find out if it's negative. 
            //So we have to check before.
            if (aIndex(0) < -0.5 || aIndex(1) < -0.5 || aIndex(2) < -0.5){
                return false;
            }
            else {
                VoxelGridIndex3D indexInt = VoxelGridIndex3D(GridIndexType(aIndex(0) + 0.5),
                    GridIndexType(aIndex(1) + 0.5),
                    GridIndexType(aIndex(2) + 0.5));
                return isInside(indexInt);
            }
		}


		bool GeometricInfo::indexToWorldCoordinate(const VoxelGridIndex3D& aIndex,
		        WorldCoordinate3D& aWorldCoordinate)
		const
		{
			DoubleVoxelGridIndex3D indexDouble = DoubleVoxelGridIndex3D(aIndex(0), aIndex(1),
			                                     aIndex(2));
			return geometryCoordinateToWorldCoordinate(indexDouble, aWorldCoordinate);
		}

		bool GeometricInfo::isInside(const VoxelGridIndex3D& aIndex) const
		{
			return (aIndex(0) >= 0 && aIndex(1) >= 0 && aIndex(2) >= 0
			        && aIndex(0) < static_cast<unsigned int>(_numberOfColumns)
			        && aIndex(1) < static_cast<unsigned int>(_numberOfRows)
			        && aIndex(2) < static_cast<unsigned int>(_numberOfFrames));
		}

		bool GeometricInfo::isInside(const WorldCoordinate3D& aWorldCoordinate)
		{
			VoxelGridIndex3D currentIndex;
			return (worldCoordinateToIndex(aWorldCoordinate, currentIndex));
		}

		const GridSizeType GeometricInfo::getNumberOfVoxels() const
		{
			GridSizeType nVoxels = static_cast<GridSizeType>(_numberOfRows * _numberOfColumns * _numberOfFrames);
			return nVoxels;
		}

		bool GeometricInfo::convert(const VoxelGridID& gridID, VoxelGridIndex3D& gridIndex) const
		{
			if (validID(gridID))
			{
				gridIndex(0) = gridID % getNumColumns();
				VoxelGridID tempID = (gridID - gridIndex.x()) / getNumColumns();
				gridIndex(1) = tempID % getNumRows();
				gridIndex(2) = (tempID - gridIndex.y()) / getNumRows();
				return true;
			}

			return false;
		}

		bool GeometricInfo::convert(const VoxelGridIndex3D& gridIndex, VoxelGridID& gridID) const
		{
			if ((gridIndex.x() >= static_cast<unsigned int>(getNumColumns()))
			    || (gridIndex.y() >= static_cast<unsigned int>(getNumRows()))
			    || (gridIndex.z() >= static_cast<unsigned int>(getNumSlices())))
			{
				return false;
			}
			else
			{
				gridID = gridIndex.z() * getNumColumns() * getNumRows() + gridIndex.y() *
				         getNumColumns()
				         + gridIndex.x();
				return validID(gridID);
			}
		}

		bool GeometricInfo::validID(const VoxelGridID aID) const
		{
			return (aID >= 0 && aID < getNumberOfVoxels());
		}

		bool GeometricInfo::validIndex(const VoxelGridIndex3D& aIndex) const
		{
			VoxelGridID aID;

			if (!convert(aIndex, aID))
			{
				return false;
			}
			else
			{
				return validID(aID);
			}
		}

		std::ostream& operator<<(std::ostream& s, const GeometricInfo& anGeometricInfo)
		{
			s << "[ " << anGeometricInfo.getImagePositionPatient() << "; " <<
			  anGeometricInfo.getOrientationMatrix()
			  << "; " << anGeometricInfo.getSpacing() << "; " << "; " << anGeometricInfo.getNumColumns()
			  << "; " << anGeometricInfo.getNumRows() << "; " << anGeometricInfo.getNumSlices() << " ]";
			return s;
		}

	}//end namespace core
}//end namespace rttb


