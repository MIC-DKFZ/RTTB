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

#ifndef __LINEAR_INTERPOLATION_H
#define __LINEAR_INTERPOLATION_H

#include <array>

#include "rttbInterpolationBase.h"

#include "RTTBInterpolationExports.h"

namespace rttb
{

	namespace interpolation
	{

		/*! @class LinearInterpolation
			@brief Linear interpolation.
			@ingroup interpolation
		*/
        class RTTBInterpolation_EXPORT LinearInterpolation : public InterpolationBase
		{
		public:
			/*! @brief Constructor
			*/
			LinearInterpolation() = default;
			/*! @brief Returns the interpolated value
			*/
			DoseTypeGy getValue(const WorldCoordinate3D& aWorldCoordinate) const override;

		private:
			/*! @brief Trilinar interpolation
				@sa InterpolationBase for details about target and values
				@note Source: http://en.wikipedia.org/wiki/Trilinear_interpolation
			*/
			DoseTypeGy trilinear(std::array<double, 3> target, boost::shared_ptr<DoseTypeGy[]> values) const;
		};

	}
}

#endif
