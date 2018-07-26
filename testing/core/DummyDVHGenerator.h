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

#ifndef __DUMMY_DVH_GENERATOR_H
#define __DUMMY_DVH_GENERATOR_H

#include <vector>

#include "rttbDVH.h"
#include "rttbBaseType.h"

namespace rttb
{

	namespace testing
	{

		/*! @class  DummyDVHGenerator
			@brief generate DVHs for testing based on a randomly generated dose data vector.
		*/
		class DummyDVHGenerator
		{
		private:
			DoseTypeGy _binSize;
			DoseVoxelVolumeType _voxelVolume;
			DoseCalcType _value;

		public:
			~DummyDVHGenerator() {};

			DummyDVHGenerator();

			/*! generate a dummy DVH with bin size = 0.1, voxel volume  = 8 and 100 entries.
				The values of the 100 bins are randomly generated [0,1000].
			*/
			core::DVH generateDVH(IDType structureID, IDType doseID);

			/*! generate a dummy DVH with bin size = 0.1, voxel volume  = 8 and 100 entries.
				The values of the 100 bins all contain the given value.
			*/
			core::DVH generateDVH(IDType structureID, IDType doseID, DoseCalcType value);

			/*! generate a dummy DVH with bin size = 0.1, voxel volume  = 0.016 and 200 entries.
				The values of the 200 bins are random values inside the interval defined by minValue and maxValue.
			*/
			core::DVH generateDVH(IDType structureID, IDType doseID, DoseCalcType minValue,
			                      DoseCalcType maxValue);

		};
	}
}

#endif
