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

#include "rttbLinearInterpolation.h"

#include <boost/make_shared.hpp>

namespace rttb
{
	namespace interpolation
	{

		DoseTypeGy LinearInterpolation::trilinear(std::array<double, 3> target,
		        boost::shared_ptr<DoseTypeGy[]> values) const
		{
			//4 linear interpolation in x direction
			DoseTypeGy c_00 = values[0] * (1.0 - target[0]) + values[1] * target[0];
			DoseTypeGy c_10 = values[2] * (1.0 - target[0]) + values[3] * target[0];
			DoseTypeGy c_01 = values[4] * (1.0 - target[0]) + values[5] * target[0];
			DoseTypeGy c_11 = values[6] * (1.0 - target[0]) + values[7] * target[0];

			//combine result in y direction
			DoseTypeGy c_0 = c_00 * (1.0 - target[1]) + c_10 * target[1];
			DoseTypeGy c_1 = c_01 * (1.0 - target[1]) + c_11 * target[1];

			//finally incorporate z direction
			return (c_0 * (1.0 - target[2]) + c_1 * target[2]);
		}

		DoseTypeGy LinearInterpolation::getValue(const WorldCoordinate3D& aWorldCoordinate) const
		{
			//proper initialization of target and values
			std::array<double, 3> target = {{0.0, 0.0, 0.0}};
      auto values = boost::make_shared<DoseTypeGy[]>(8);
			getNeighborhoodVoxelValues(aWorldCoordinate, 8, target, values);

			return trilinear(target, values);
		}

	}
}
