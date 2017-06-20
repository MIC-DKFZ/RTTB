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

#include "rttbNearestNeighborInterpolation.h"

#include <array>

namespace rttb
{
	namespace interpolation
	{
		DoseTypeGy NearestNeighborInterpolation::getValue(const WorldCoordinate3D& aWorldCoordinate) const
		{
			//proper initialization of target and values (although target is irrelevant in nearest neighbor case)
			std::array<double, 3> target = {0.0, 0.0, 0.0};
			boost::shared_ptr<DoseTypeGy[]> values(new DoseTypeGy[8]());
			getNeighborhoodVoxelValues(aWorldCoordinate, 0, target, values);
			return values[0];
		}

	}
}
