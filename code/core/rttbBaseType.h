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

#ifndef __BASE_TYPE_NEW_H
#define __BASE_TYPE_NEW_H

#include <string>
#include <vector>
#include <list>
#include <ostream>

#include <boost/serialization/array_wrapper.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <RTTBCoreExports.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{

	const double errorConstant = 1e-5;
	const double reducedErrorConstant = 0.0001;

	using Index1D = unsigned short;

	/*! @class Index3D
		@brief 3D index.
	*/
	class Index3D: public boost::numeric::ublas::vector<Index1D>
	{
	public:
		Index3D() : boost::numeric::ublas::vector<Index1D>(3,0) {}
		explicit Index3D(const Index1D value) : boost::numeric::ublas::vector<Index1D>(3,
			        value) {}
		Index3D(const Index1D  xValue, const Index1D  yValue,
		                const Index1D  zValue)
			: boost::numeric::ublas::vector<Index1D >(3, xValue)
		{
			(*this)(1) = yValue;
			(*this)(2) = zValue;
		}

		const Index1D x() const
		{
			return (*this)(0);
		}
		const Index1D y() const
		{
			return (*this)(1);
		}
		const Index1D z() const
		{
			return (*this)(2);
		}
		friend bool operator==(const Index3D& gi1, const Index3D& gi2)
		{
			if (gi1.size() != gi2.size())
			{
				return false;
			}

			for (size_t i = 0; i < gi1.size(); i++)
			{
				if (gi1(i) != gi2(i))
				{
					return false;
				}
			}

			return true;
		}

		friend std::ostream& operator<<(std::ostream& s, const Index3D& aVector)
		{
			s << "[ " << aVector(0) << ", " << aVector(1) << ", " << aVector(2) << " ]";
			return s;
		}
	};

	using IndexList = std::list<Index3D>;

	using FileNameString = std::string;

	using ContourGeometricTypeString = std::string;

	using WorldCoordinate = double;

	/*! @class WorldCoordinate3D
		@brief 3D coordinate in real world coordinates like in DICOM to define ImagePositionPatient.
	*/
	class WorldCoordinate3D: public boost::numeric::ublas::vector<WorldCoordinate>
	{
	public:
		WorldCoordinate3D() : boost::numeric::ublas::vector<WorldCoordinate>(3,0) {}
		explicit WorldCoordinate3D(const WorldCoordinate value) : boost::numeric::ublas::vector<WorldCoordinate>(3,
			        value) {}
		WorldCoordinate3D(const WorldCoordinate xValue, const WorldCoordinate yValue,
		                  const WorldCoordinate zValue)
			: boost::numeric::ublas::vector<WorldCoordinate>(3, xValue)
		{
			(*this)(1) = yValue;
			(*this)(2) = zValue;
		}
		WorldCoordinate3D(const WorldCoordinate3D& w): boost::numeric::ublas::vector<WorldCoordinate>(3)
		{
			(*this)(0) = w.x();
			(*this)(1) = w.y();
			(*this)(2) = w.z();
		}

		const WorldCoordinate x() const
		{
			return (*this)(0);
		}
		const WorldCoordinate y() const
		{
			return (*this)(1);
		}
		const WorldCoordinate z() const
		{
			return (*this)(2);
		}

		//vector cross product (not included in boost.ublas)
		WorldCoordinate3D cross(WorldCoordinate3D aVector) const
		{
			WorldCoordinate3D result;
			WorldCoordinate x = (*this)(0);
			WorldCoordinate y = (*this)(1);
			WorldCoordinate z = (*this)(2);

			result(0) = y * aVector(2) - z * aVector(1);
			result(1) = z * aVector(0) - x * aVector(2);
			result(2) = x * aVector(1) - y * aVector(0);

			return result;
		}

		const std::string toString() const
		{
      std::string s = std::to_string(x()) + " " + std::to_string(y()) + " " + std::to_string(z());
			return s;
		}

		WorldCoordinate3D& operator=(const WorldCoordinate3D& wc)
		{
			(*this)(0) = wc.x();
			(*this)(1) = wc.y();
			(*this)(2) = wc.z();
			return (*this);
		}

		WorldCoordinate3D& operator=(const boost::numeric::ublas::vector<WorldCoordinate> wc)
		{
			(*this)(0) = wc(0);
			(*this)(1) = wc(1);
			(*this)(2) = wc(2);
			return (*this);
		}

		WorldCoordinate3D operator-(const boost::numeric::ublas::vector<WorldCoordinate> wc)
		{
			return WorldCoordinate3D((*this)(0) - wc(0), (*this)(1) - wc(1), (*this)(2) - wc(2));
		}

		WorldCoordinate3D operator+(const boost::numeric::ublas::vector<WorldCoordinate> wc)
		{
			return WorldCoordinate3D((*this)(0) + wc(0), (*this)(1) + wc(1), (*this)(2) + wc(2));
		}

		friend bool operator==(const WorldCoordinate3D& wc1, const WorldCoordinate3D& wc2)
		{
			if (wc1.size() != wc2.size())
			{
				return false;
			}

			for (size_t i = 0; i < wc1.size(); i++)
			{
				if (wc1(i) != wc2(i))
				{
					return false;
				}
			}

			return true;
		}

		bool equalsAlmost(const WorldCoordinate3D& another, double errorConstantWC = 1e-5) const
		{
			if (size() != another.size())
			{
				return false;
			}

			double dist = norm_2(*this - another);
			return dist < errorConstantWC;
		}

		friend std::ostream& operator<<(std::ostream& s, const WorldCoordinate3D& aVector)
		{
			s << "[ " << aVector(0) << ", " << aVector(1) << ", " << aVector(2) << " ]";
			return s;
		}

	};

    /* ! @brief continuous index */
	using ContinousVoxelGridIndex3D = rttb::WorldCoordinate3D;

	using ImageSize = rttb::Index3D;

	using GridVolumeType = double;

	/*! @class SpacingVectorType3D
		@brief 3D spacing vector.
		@pre values of this vector may not be negative.
	*/
	class SpacingVectorType3D: public boost::numeric::ublas::vector<GridVolumeType>
	{
	public:
		SpacingVectorType3D() : boost::numeric::ublas::vector<GridVolumeType>(3,0) {}
		explicit SpacingVectorType3D(const GridVolumeType value) : boost::numeric::ublas::vector<GridVolumeType>(3,
			        value) 
		{
			if (value < 0) {
				throw std::invalid_argument("received negative value");
			}
		}

		SpacingVectorType3D(const GridVolumeType xValue, const GridVolumeType yValue,
		                    const GridVolumeType zValue)
			: boost::numeric::ublas::vector<GridVolumeType>(3)
		{
			if (xValue < 0 || yValue < 0 || zValue < 0) {
				throw std::invalid_argument("received negative value");
			}
			(*this)(0) = xValue;
			(*this)(1) = yValue;
			(*this)(2) = zValue;
		}

		SpacingVectorType3D(const SpacingVectorType3D& w) : SpacingVectorType3D(w.x(), w.y(), w.z()) { }

		const GridVolumeType x() const
		{
			return (*this)(0);
		}

		const GridVolumeType y() const
		{
			return (*this)(1);
		}

		const GridVolumeType z() const
		{
			return (*this)(2);
		}

		const std::string toString() const
		{
      std::string s = std::to_string(x()) + " " + std::to_string(y()) + " " + std::to_string(z());
      return s;
		}

		SpacingVectorType3D& operator=(const SpacingVectorType3D& wc)
		{
			(*this)(0) = wc.x();
			(*this)(1) = wc.y();
			(*this)(2) = wc.z();
			return (*this);
		}

		SpacingVectorType3D& operator=(const WorldCoordinate3D& wc)
		{
			(*this)(0) = GridVolumeType(wc.x());
			(*this)(1) = GridVolumeType(wc.y());
			(*this)(2) = GridVolumeType(wc.z());
			return (*this);
		}

		SpacingVectorType3D& operator=(const boost::numeric::ublas::vector<GridVolumeType> wc)
		{
			(*this)(0) = wc(0);
			(*this)(1) = wc(1);
			(*this)(2) = wc(2);
			return (*this);
		}

		friend bool operator==(const SpacingVectorType3D& wc1, const SpacingVectorType3D& wc2)
		{
			if (wc1.size() != wc2.size())
			{
				return false;
			}

			for (size_t i = 0; i < wc1.size(); i++)
			{
				if (wc1(i) != wc2(i))
				{
					return false;
				}
			}

			return true;
		}

		bool equalsAlmost(const SpacingVectorType3D& another, double errorConstantSV = 1e-5) const
		{
			if ((*this).size() != another.size())
			{
				return false;
			}

			double dist = norm_2(*this - another);
			return dist < errorConstantSV;
		}

		friend std::ostream& operator<<(std::ostream& s, const SpacingVectorType3D& aVector)
		{
			s << "[ " << aVector(0) << ", " << aVector(1) << ", " << aVector(2) << " ]";
			return s;
		}
	};

	/*!	@class OrientationMatrix
		@brief Used to store image orientation information
	*/
	class OrientationMatrix : public boost::numeric::ublas::matrix<WorldCoordinate>
	{
	public:
		/*! The default constructor generates a 3x3 unit matrix
		*/
		OrientationMatrix() : boost::numeric::ublas::matrix<WorldCoordinate>(3, 3, 0)
		{
			for (std::size_t m = 0; m < (*this).size1(); m++)
			{
				(*this)(m, m) = 1;
			}
		}
		explicit OrientationMatrix(const WorldCoordinate value) : boost::numeric::ublas::matrix<WorldCoordinate>(3,
			        3, value) {}

		bool equalsAlmost(const OrientationMatrix& anOrientationMatrix, double errorConstantOM=1e-5) const
		{
			if (anOrientationMatrix.size1() == (*this).size1())
			{
				if (anOrientationMatrix.size2() == (*this).size2())
				{
					for (std::size_t m = 0; m < anOrientationMatrix.size1(); m++)
					{
						for (std::size_t n = 0; n < anOrientationMatrix.size2(); n++)
						{
							if ((std::abs((*this)(m, n) - anOrientationMatrix(m, n)) > errorConstantOM))
							{
								return false;
							}
						}
					}// end element comparison
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}

			return true;
		}

		friend bool operator==(const OrientationMatrix& om1, const OrientationMatrix& om2)
		{
			return om1.equalsAlmost(om2, 0.0);
		}

		friend std::ostream& operator<<(std::ostream& s, const OrientationMatrix& anOrientationMatrix)
		{
			s << "[ ";

			for (std::size_t m = 0; m < anOrientationMatrix.size1(); m++)
			{
				s << "[ ";

				for (std::size_t n = 0; n < anOrientationMatrix.size2(); n++)
				{
					if (n == 0)
					{
						s << anOrientationMatrix(m, n);
					}
					else
					{
						s << ", " << anOrientationMatrix(m, n);
					}
				}

				s << " ]";
			}

			s << " ]";
			return s;
		}
	};


	/*! base for 2D and 3D VoxelIndex; Therefore required beside VoxelGridID
	*/
	using GridIndexType = unsigned int;

	/*! @class VoxelGridIndex3D
		@brief 3D voxel grid index in a discret geometry (matrix/image).
        @details analogous to DICOM where ImagePositionPatient gives the position of the center of the first coordinate (0/0/0)
	*/
	class VoxelGridIndex3D: public boost::numeric::ublas::vector<GridIndexType>
	{
	public:
		VoxelGridIndex3D() : boost::numeric::ublas::vector<GridIndexType>(3,0) {}
		explicit VoxelGridIndex3D(const GridIndexType value) : boost::numeric::ublas::vector<GridIndexType>(3,
			        value) {}
		VoxelGridIndex3D(const GridIndexType xValue, const GridIndexType yValue, const GridIndexType zValue)
			: boost::numeric::ublas::vector<GridIndexType>(3, xValue)
		{
			(*this)(1) = yValue;
			(*this)(2) = zValue;
		}

		const GridIndexType x() const
		{
			return (*this)(0);
		}
		const GridIndexType y() const
		{
			return (*this)(1);
		}
		const GridIndexType z() const
		{
			return (*this)(2);
		}

		const std::string toString() const
		{
      std::string s = std::to_string(x()) + " " + std::to_string(y()) + " " + std::to_string(z());
      return s;
		}

		VoxelGridIndex3D& operator=(const Index3D& ui)
		{
			(*this)(0) = ui(0);
			(*this)(1) = ui(1);
			(*this)(2) = ui(2);
			return (*this);
		}

		friend bool operator==(const VoxelGridIndex3D& gi1, const VoxelGridIndex3D& gi2)
		{
			if (gi1.size() != gi2.size())
			{
				return false;
			}

			for (size_t i = 0; i < gi1.size(); i++)
			{
				if (gi1(i) != gi2(i))
				{
					return false;
				}
			}

			return true;
		}

		friend std::ostream& operator<<(std::ostream& s, const VoxelGridIndex3D& aVector)
		{
			s << "[ " << aVector(0) << ", " << aVector(1) << ", " << aVector(2) << " ]";
			return s;
		}
	};


	/*! @class VoxelGridIndex3D
		@brief 2D voxel grid index.
	*/
	class VoxelGridIndex2D: public boost::numeric::ublas::vector<GridIndexType>
	{
	public:
		VoxelGridIndex2D() : boost::numeric::ublas::vector<GridIndexType>(2,0) {}
		explicit VoxelGridIndex2D(const GridIndexType value) : boost::numeric::ublas::vector<GridIndexType>(2,
			        value) {}
		VoxelGridIndex2D(const GridIndexType xValue, const GridIndexType yValue)
			: boost::numeric::ublas::vector<GridIndexType>(2, xValue)
		{
			(*this)(1) = yValue;
		}

		const GridIndexType x() const
		{
			return (*this)(0);
		}
		const GridIndexType y() const
		{
			return (*this)(1);
		}

		const std::string toString() const
		{
      std::string s = std::to_string(x()) + " " + std::to_string(y());
      return s;
		}

		friend bool operator==(const VoxelGridIndex2D& gi1, const VoxelGridIndex2D& gi2)
		{
			if (gi1.size() != gi2.size())
			{
				return false;
			}

			for (size_t i = 0; i < gi1.size(); i++)
			{
				if (gi1(i) != gi2(i))
				{
					return false;
				}
			}

			return true;
		}

		friend std::ostream& operator<<(std::ostream& s, const VoxelGridIndex2D& aVector)
		{
			s << "[ " << aVector(0) << ", " << aVector(1) << " ]";
			return s;
		}
	};

	using GridSizeType = long;

	using VoxelGridID = int; //starts from 0 and is continuously counting all positions on the grid
	using VoxelGridDimensionType = unsigned int;

	typedef double FractionType, DVHVoxelNumber;

	typedef double DoseCalcType, DoseTypeGy, GenericValueType, DoseVoxelVolumeType, VolumeType,
	        GridVolumeType, PercentType,
	        VoxelNumberType, BEDType,
	        LQEDType;

	using IDType = std::string;

	using StructureLabel = std::string;

	struct DVHRole
	{
		enum Type
		{
			TargetVolume = 1,
			HealthyTissue = 2,
			WholeVolume = 4,
			UserDefined = 128
		} Type;
	};

	struct DVHType
	{
		enum Type
		{
			Differential = 1,
			Cumulative = 2
		} Type;
	};

  using FileNameType = std::string;

	using PolygonType = std::vector<WorldCoordinate3D>;

	using PolygonSequenceType = std::vector<PolygonType>;

	using IndexValueType = double;

	using DoseStatisticType = double;

	using DICOMRTFileNameString = std::string;

	using Uint16 = unsigned short;

	typedef std::string XMLString, StatisticsString;


}//end: namespace rttb

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
