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

#ifndef __INTERPOLATION_BASE_H
#define __INTERPOLATION_BASE_H

#include <boost/shared_ptr.hpp>
#include <array>

#include "rttbAccessorInterface.h"

#include "RTTBInterpolationExports.h"

namespace rttb
{
	namespace interpolation
	{

		/*! @class InterpolationBase
			@brief Base class for interpolation.
			@ingroup interpolation
		*/
		class RTTBInterpolation_EXPORT InterpolationBase
		{
		public:
			using Pointer = boost::shared_ptr<InterpolationBase>;

			/*! @brief Constructor
			*/
			InterpolationBase() = default;

			/*! @brief Virtual destructor of base class
			*/
			virtual ~InterpolationBase() = default;

			/*! @brief Sets the AccessorPointer
				@pre originalData initialized
				@exception core::NullPointerException if originalData==nullptr
			*/
			void setAccessorPointer(core::AccessorInterface::ConstPointer originalData);

			/*! @brief Returns the interpolated value for the given world coordinate
			*/
			virtual DoseTypeGy getValue(const WorldCoordinate3D& aWorldCoordinate) const = 0;

		protected:
      rttb::core::AccessorInterface::ConstPointer _spOriginalData;
			/*! @brief determines voxels in a certain neighborhood of a physical based coordinate and converts in a standard cube with corner points [0 0 0], [1 0 0], [0 1 0], [1 1 0], [0 0 1], [1 0 1], [0 1 1], [1 1 1].
				@param aWorldCoordinate the coordinate where to start
				@param neighborhood voxel around coordinate (currently only 0 and 8 implemented)
				@param target coordinates inside the standard cube with values [0 1] in each dimension.
				@param values dose values at all corner points of the standard cube. Is of type boost:shared_ptr[neighborhood]
				@pre target and values have to be correctly initialized (e.g. std::array<double, 3> target = {0.0, 0.0, 0.0}; boost::shared_ptr<DoseTypeGy> values(new DoseTypeGy[8]()); where 8 is neighborhood)
				@exception core::InvalidParameterException if neighborhood =! 0 && !=8
				@exception core::MappingOutsideOfImageException if initial mapping of aWorldCoordinate is outside image
				@exception core::NullPointerException if dose is nullptr
			*/
			void getNeighborhoodVoxelValues(const WorldCoordinate3D& aWorldCoordinate,
			                                unsigned int neighborhood, std::array<double, 3>& target,
			                                boost::shared_ptr<DoseTypeGy[]> values) const;

			/*! @brief returns the nearest inside voxel value
				@pre the voxelGridIndex is outside the image and voxelGridIndex>image.size() for all dimensions. Also voxelGridIndex[]>=0 for all dimensions
				@note used for virtually expanding the image by one voxel as edge handling
			*/
			DoseTypeGy getNearestInsideVoxelValue(const VoxelGridIndex3D& currentVoxelIndex) const;
		};

	}
}

#endif
