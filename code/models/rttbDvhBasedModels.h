#include <map>

#include "rttbDVH.h"
#include "rttbBaseType.h"

namespace rttb{
	namespace models{
		typedef core::DVH::DataDifferentialType DataDifferentialType;
		typedef core::DVH::DVHPointer DVHPointer;

		/*! @brief Get Equivalent Uniform Dose (EUD)  
			@pre dvh data differential is not empty, 
			@pre aA is not zero, 
			@return Return calculated EUD value, 
      @exception InvalidParameterException Thrown if parameters were not set correctly.
		*/
		DoseStatisticType getEUD(const DVHPointer dvh, const DoseCalcType aA);

		/*! @brief Calculate Biological Effective/Equivalent Dose (BED) of dvh 
			@param relativeVolume default false-> the corresponding bed value is the voxel number of the dose bin; 
			if true-> the corresponding bed value is the relative volume % between 0 and 1, 
			(the voxel number of this dose bin)/(number of voxels)
			@pre dvh data differential is not empty
			@pre alpha_beta > 0
			@pre numberOfFractions > 0
			@return map of BED values
      @exception InvalidParameterException Thrown if parameters were not set correctly.
		*/
		std::map<rttb::DoseTypeGy,rttb::DoseCalcType> calcBEDDVH(const DVHPointer dvh, const int numberOfFractions, 
			const DoseCalcType alpha_beta, const bool relativeVolume=false);

		/*! @brief Calculate Linear-quadratic equivalent dose for 2-Gy (LQED2) of dvh  
			@param relativeVolume default false-> the corresponding LQED2 value is the voxel number of the dose bin; 
			if true-> the corresponding LQED2 value is the relative volume % between 0 and 1, 
			(the voxel number of this dose bin)/(number of voxels)
			@pre dvh data differential is not empty
			@pre alpha_beta > 0
			@pre numberOfFractions > 0
			@return map of LQED2 values
      @exception InvalidParameterException Thrown if parameters were not set correctly.
		*/
		std::map<rttb::DoseTypeGy,rttb::DoseCalcType> calcLQED2DVH(const DVHPointer dvh, const int numberOfFractions, 
			const DoseCalcType alpha_beta, const bool relativeVolume=false);
		}
	}