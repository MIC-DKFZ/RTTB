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
#ifndef __ROSU_MAPPABLE_DOSE_ACCESSOR_H
#define __ROSU_MAPPABLE_DOSE_ACCESSOR_H

#include <boost/shared_ptr.hpp>

#include "rttbBaseType.h"
#include "rttbInterpolationBase.h"
#include "rttbMappableDoseAccessorInterface.h"

namespace rttb
{
	namespace interpolation
	{
        class TransformationInterface;
		/*! @class RosuMappableDoseAccessor
		@brief Class for dose mapping based on interpolation described in the Rosu2005 paper
		@details implementation of the following paper: Rosu, M., Chetty, I. J., Balter, J. M., Kessler, M. L., McShan, D. L., & Ten Haken, R. K. (2005). Dose reconstruction in deforming lung anatomy: Dose grid size effects and clinical implications. Medical Physics, 32(8), 2487.
		@ingroup interpolation
		*/
		class RosuMappableDoseAccessor: public MappableDoseAccessorInterface
		{
		private:
			InterpolationBase::Pointer _spInterpolation;

		public:
			typedef boost::shared_ptr<RosuMappableDoseAccessor> Pointer;
            typedef boost::shared_ptr<TransformationInterface> TransformationPointer;

			/*! @brief Constructor. Just hands values over to base class constructor.
				@note no interpolation as parameter since linear interpolation is fixed.
				@sa MappableDoseAccessorBase
			*/
			RosuMappableDoseAccessor(const core::GeometricInfo& geoInfoTargetImage,
			                         const DoseAccessorPointer doseMovingImage,
                                     const TransformationPointer aTransformation,
			                         bool acceptPadding = true,
			                         DoseTypeGy defaultOutsideValue = 0.0);

			/*! @brief Virtual destructor.
			*/
			~RosuMappableDoseAccessor() override {};

			GenericValueType getValueAt(const VoxelGridID aID) const override;

			/*! @brief Returns the dose for a given voxel grid index. The computation of the octant around the voxel is done and the interpolation is performed.
				@details Boundary treatment: if more than 6 subvoxels are outside: return _defaultOutsideValue. Otherwise: ignore the outside values.
				@return the dose or if (isOutside==true && _acceptPadding==true) then _defaultValue
				@exception core::MappingOutsideOfImageException if the point is mapped outside and if _acceptPadding==false, possibly returning _defaultValue)
			*/
			GenericValueType getValueAt(const VoxelGridIndex3D& aIndex) const override;

		private:
			/*! @brief returns the octant coordinates around a coordinate.
				@details i.e. coordinate is the center of a virtual voxel. Then, each side is divided into equal parts. The centers of the new subvoxels are then returned.
				@return a vector of the octant coordinates.
			*/
			std::vector<WorldCoordinate3D> getOctants(const WorldCoordinate3D& aCoordinate) const;
		};
	}
}

#endif
