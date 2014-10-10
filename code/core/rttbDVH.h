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
#ifndef __DVH_H
#define __DVH_H

#include <deque>
#include <ostream>

#include "rttbBaseType.h"
#include "rttbStructure.h"

namespace rttb{
	namespace core{

		class Structure;

		/*! @class DVH
		@brief This is a class representing a dose volume histogram (DVH)
		*/
		class DVH
			{
			public:
				typedef std::deque<DoseCalcType> DataDifferentialType;
				typedef boost::shared_ptr<DVH> DVHPointer;
			private: 
				IDType _structureID;
				IDType _doseID;
				IDType _voxelizationID;
				/*! @brief Differential dvh data index is the dose bin, value is the voxel number (sub voxel accuracy) 
					of the dose bin
				*/
				DataDifferentialType _dataDifferential;
				/*! @brief Differential dvh data relative to the total number of voxels
				*/
				DataDifferentialType _dataDifferentialRelative;
				/*! @brief Volume of a voxel in cm3 
				*/
				DoseVoxelVolumeType _deltaV;
				/*! @brief Absolute dose value of a dose-bin in Gy
				*/
				DoseTypeGy _deltaD;

				StructureLabel _label;

				DoseStatisticType _maximum;
				DoseStatisticType _minimum;
				DoseStatisticType _mean;
				DoseStatisticType _modal;
				DVHVoxelNumber _numberOfVoxels;
				DoseStatisticType _median;
				DoseStatisticType _stdDeviation;
				DoseStatisticType _variance;
				DataDifferentialType _dataCumulative;
				DataDifferentialType _dataCumulativeRelative;

				/*! @brief DVH initialisation 
					The DVH is initialized and all statistical values are calculated.
					@throw <InvalidParameterException> if _deltaV or _deltaD are zero
					@throw <InvalidParameterException> is _data differential is empty
				*/
				void init();


			public:
				~DVH();

				/*!
					@throw <InvalidParameterException> if _deltaV or _deltaD are zero
					@throw <InvalidParameterException> is _data differential is empty
				*/
				DVH(const DataDifferentialType& aDataDifferential, const DoseTypeGy& aDeltaD, const DoseVoxelVolumeType& aDeltaV, 
					IDType aStructureID, IDType aDoseID);

				/*!
					@throw <InvalidParameterException> if _deltaV or _deltaD are zero
					@throw <InvalidParameterException> is _data differential is empty
				*/
				DVH(const DataDifferentialType& aDataDifferential, DoseTypeGy aDeltaD, DoseVoxelVolumeType aDeltaV, 
					IDType aStructureID, IDType aDoseID, IDType aVoxelizationID);

				DVH(const DVH &copy);

				/*!
					@throw <InvalidParameterException> if _deltaV or _deltaD are zero
					@throw <InvalidParameterException> is _data differential is empty
				*/
				DVH &operator=(const DVH &copy);

				/*! equality operator
					DVHs are considered equal if they have the same structure, dose and voxelization ID
					and the same number of voxels.
				*/
				bool friend operator==(const DVH &aDVH, const DVH &otherDVH);

				friend std::ostream& operator<<(std::ostream& s, const DVH &aDVH);

				void setLabel(StructureLabel aLabel);
				StructureLabel getLabel() const;

				/*! @param relativeVolume default false-> Value is the voxel number of the dose bin; 
					if true-> value is the relative volume % between 0 and 1, 
					(the voxel number of this dose bin)/(number of voxels)
					@return Return differential data of the dvh (relative or abolute depending on the 
					input parameter). 
				*/
				DataDifferentialType getDataDifferential(bool relativeVolume=false) const;

				DoseVoxelVolumeType getDeltaV() const;
				DoseTypeGy getDeltaD() const;
				IDType getStructureID() const;
				IDType getDoseID() const;
				IDType getVoxelizationID() const;

				/*! @todo Should it be possible to set the DoseID externally? Should this be only possible in the 
					DVHCalculator and corresponding IO classes?
				*/
				void setDoseID(IDType aDoseID);
				/*! @todo Should it be possible to set the StructureID externally?
				*/
				void setStructureID(IDType aStrID);

				/*! @brief Calculate number of the voxels (with sub voxel accuracy)
					@return Return -1 if not initialized
				*/
				DVHVoxelNumber getNumberOfVoxels() const;

				/*! @brief Get the maximum dose in Gy from dvh
					@return Return the maximum dose in Gy (i+0.5)*deltaD, i-the maximal dose-bin with volume>0
					Return -1 if not initialized
				*/
				DoseStatisticType getMaximum() const;

				/*! @brief Get the minimum dose in Gy from dvh
					@return Return the minimum dose (i+0.5)*deltaD, i-the minimal dose-bin with volume>0
					Return -1 if not initialized
				*/
				DoseStatisticType getMinimum() const;

				DoseStatisticType getMean() const;
				DoseStatisticType getMedian() const;
				DoseStatisticType getModal() const;
				DoseStatisticType getStdDeviation() const;
				DoseStatisticType getVariance() const;

				/*! @brief Calculate the cumulative data of dvh 
					@param relativeVolume default false-> Value is the voxel number of the dose bin; 
					if true-> value is the relative volume % between 0 and 1, 
					(the voxel number of this dose bin)/(number of voxels)
					@return Return cumulative dvh value i-voxel number in dose-bin i
				*/
				DataDifferentialType calcCumulativeDVH(bool relativeVolume=false);

				/*! @brief Get Vx the volume irradiated to >= x  
					@return Return absolute Volume in absolute cm3
					Return -1 if not initialized 
				*/		
				VolumeType getVx(DoseTypeGy xDoseAbsolute);
				/*! @brief Get Dx the minimal dose delivered to x  
					@return Return absolute dose value in Gy
					Return -1 if not initialized
				*/	
				DoseTypeGy getDx(VolumeType xVolumeAbsolute);

				/*! @brief Calculate the absolute volume in cm3
					@param relativePercent 0~100, the percent of the whole volume
				*/
				VolumeType getAbsoluteVolume(int relativePercent);

			};
		}
}


#endif
