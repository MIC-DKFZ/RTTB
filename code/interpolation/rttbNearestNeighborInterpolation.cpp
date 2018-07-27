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

#include "rttbNearestNeighborInterpolation.h"

#include <boost/make_shared.hpp>

namespace rttb
{
	namespace interpolation
	{
		DoseTypeGy NearestNeighborInterpolation::getValue(const WorldCoordinate3D& aWorldCoordinate) const
		{
			//proper initialization of target and values (although target is irrelevant in nearest neighbor case)
			std::array<double, 3> target = {{0.0, 0.0, 0.0}};
      auto values = boost::make_shared<DoseTypeGy[]>(8);
			getNeighborhoodVoxelValues(aWorldCoordinate, 0, target, values);
			return values[0];
		}

	}
}
