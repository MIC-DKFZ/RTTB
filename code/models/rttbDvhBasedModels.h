#include <map>

#include "rttbDVH.h"
#include "rttbBaseType.h"

namespace rttb
{
	namespace models
	{
		using DataDifferentialType = core::DVH::DataDifferentialType;
		using DVHPointer = core::DVH::DVHPointer;
		typedef std::map<BEDType, VoxelNumberType> BEDDVHType;
		typedef std::map<LQEDType, VoxelNumberType> LQEDDVHType;

		/*! @brief Get Equivalent Uniform Dose (EUD)
		@pre dvh data differential is not empty,
		@pre aA is not zero,
		@return Return calculated EUD value,
		@exception InvalidParameterException Thrown if parameters were not set correctly.
		*/
		DoseStatisticType getEUD(const DVHPointer dvh, const DoseCalcType aA);

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
		BEDDVHType calcBEDDVH(const DVHPointer dvh, const int numberOfFractions,
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
		LQEDDVHType calcLQED2DVH(const DVHPointer dvh, const int numberOfFractions,
		                         const DoseCalcType alpha_beta, const bool relativeVolume = false);
	}
}