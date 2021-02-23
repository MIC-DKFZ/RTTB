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

#include <map>

#include "rttbDVH.h"
#include "rttbBaseType.h"

#include "RTTBModelsExports.h"

namespace rttb
{
	namespace models
	{
		using DataDifferentialType = core::DVH::DataDifferentialType;
		typedef std::map<BEDType, VoxelNumberType> BEDDVHType;
		typedef std::map<LQEDType, VoxelNumberType> LQEDDVHType;

		/*! @brief Get Equivalent Uniform Dose (EUD)
		@pre dvh data differential is not empty,
		@pre aA is not zero,
		@return Return calculated EUD value,
		@exception InvalidParameterException Thrown if parameters were not set correctly.
		*/
		RTTBModels_EXPORT DoseStatisticType getEUD(core::DVH::ConstPointer dvh, const DoseCalcType aA);

		/*! @brief Calculate Biological Effective/Equivalent Dose (BED) of dvh
		@param relativeVolume default false-> the corresponding volume value is the voxel number of the dose bin;
		if true-> the corresponding volume value is the relative volume % between 0 and 1,
		(the voxel number of this dose bin)/(number of voxels)
		@pre dvh should be an accumulated dvh of all fractions, not a single fraction dvh
		@pre dvh data differential is not empty
		@pre alpha_beta > 0
		@pre numberOfFractions > 1
		@return Return map<double, double>: keys are BEDi in Gy, values are the volume of the dose bin
		@exception InvalidParameterException Thrown if parameters were not set correctly.
		*/
		RTTBModels_EXPORT BEDDVHType calcBEDDVH(core::DVH::ConstPointer dvh, const int numberOfFractions,
		                      const DoseCalcType alpha_beta, const bool relativeVolume = false);

		/*! @brief Calculate Linear-quadratic equivalent dose for 2-Gy (LQED2) of dvh
		@param relativeVolume default false-> the corresponding volume value is the voxel number of the dose bin;
		if true-> the corresponding volume value is the relative volume % between 0 and 1,
		(the voxel number of this dose bin)/(number of voxels)
		@pre dvh should be an accumulated dvh of all fractions, not a single fraction dvh
		@pre dvh data differential is not empty
		@pre alpha_beta > 0
		@pre numberOfFractions > 1
		@return Return map<double, double>: keys are LQED2 in Gy, values are the volume of the dose bin; return empty map if not initialized
		@exception InvalidParameterException Thrown if parameters were not set correctly.
		*/
		RTTBModels_EXPORT LQEDDVHType calcLQED2DVH(core::DVH::ConstPointer dvh, const int numberOfFractions,
		                         const DoseCalcType alpha_beta, const bool relativeVolume = false);
	}
}