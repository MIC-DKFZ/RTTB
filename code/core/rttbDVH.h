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
#include <map>

#include "boost/shared_ptr.hpp"

#include "rttbBaseType.h"
#include "rttbCommon.h"

#include "RTTBCoreExports.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{
	namespace core
	{

		/*! @class DVH
		@brief This is a class representing a dose volume histogram (DVH)
		*/
		class RTTBCore_EXPORT DVH
		{
		public:
			using DataDifferentialType = std::deque<DoseCalcType>;
      rttbClassMacroNoParent(DVH);

		private:
			/*! @brief Differential dvh data index is the dose bin, value is the voxel number (sub voxel accuracy)
				of the dose bin
			*/
			DataDifferentialType _dataDifferential;
			
			/*! @brief Absolute dose value of a dose-bin in Gy
			*/
			DoseTypeGy _deltaD;

			/*! @brief Volume of a voxel in cm3
			*/
			DoseVoxelVolumeType _deltaV;

			IDType _structureID;
			IDType _doseID;
			IDType _voxelizationID;


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

			/*! @brief DVH initialization
				The DVH is initialized and all statistical values are calculated.
				@throw <InvalidParameterException> if _deltaV or _deltaD are zero
				@throw <InvalidParameterException> is _data differential is empty
			*/
			void init();

      /*! @brief Calculate the cumulative data of dvh
      */
      void calcCumulativeDVH();


		public:
			~DVH();

			/*!
				@throw <InvalidParameterException> if _deltaV or _deltaD are zero
				@throw <InvalidParameterException> is _data differential is empty
			*/
			DVH(const DataDifferentialType& aDataDifferential, const DoseTypeGy& aDeltaD,
			    const DoseVoxelVolumeType& aDeltaV,
			    const IDType& aStructureID, const IDType& aDoseID);

			/*!
				@throw <InvalidParameterException> if _deltaV or _deltaD are zero
				@throw <InvalidParameterException> is _data differential is empty
			*/
			DVH(const DataDifferentialType& aDataDifferential, DoseTypeGy aDeltaD, DoseVoxelVolumeType aDeltaV,
			    const IDType& aStructureID, const IDType& aDoseID, const IDType& aVoxelizationID);

			DVH(const DVH& copy);

			/*!
				@throw <InvalidParameterException> if _deltaV or _deltaD are zero
				@throw <InvalidParameterException> is _data differential is empty
			*/
			DVH& operator=(const DVH& copy);

			/*! equality operator
				DVHs are considered equal if the following are equal (let alone double inconsistencies):
        - structureID
        - doseID
        - voxelizationID
        - number of voxels
        - Histogram entries.
			*/
			bool friend operator==(const DVH& aDVH, const DVH& otherDVH);

			friend std::ostream& operator<<(std::ostream& s, const DVH& aDVH);

			void setLabel(StructureLabel aLabel);
			StructureLabel getLabel() const;

			/*! @param relativeVolume default false-> Value is the voxel number of the dose bin;
				if true-> value is the relative volume % between 0 and 1,
				(the voxel number of this dose bin)/(number of voxels)
				@return Return differential data of the dvh (relative or absolute depending on the
				input parameter).
			*/
			DataDifferentialType getDataDifferential() const;

      /*! @param relativeVolume default false-> Value is the voxel number of the dose bin;
      if true-> value is the relative volume % between 0 and 1,
      (the voxel number of this dose bin)/(number of voxels)
      @return Return cumulative data of the dvh
      */
      DataDifferentialType getDataCumulative() const;

			DoseVoxelVolumeType getDeltaV() const;
			DoseTypeGy getDeltaD() const;
			IDType getStructureID() const;
			IDType getDoseID() const;
			IDType getVoxelizationID() const;

			void setDoseID(IDType aDoseID);

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

			/*! @brief Get Vx the volume irradiated to >= x
				@return Return absolute Volume in absolute cm3
				Return -1 if not initialized
			*/
			VolumeType getVx(DoseTypeGy xDoseAbsolute) const;
			/*! @brief Get Dx the minimal dose delivered to x
				@return Return absolute dose value in Gy
				Return -1 if not initialized
			*/
			DoseTypeGy getDx(VolumeType xVolumeAbsolute) const;

			/*! @brief Calculate the absolute volume in cm3
				@param relativePercent 0~100, the percent of the whole volume
			*/
			VolumeType getAbsoluteVolume(int relativePercent) const;

			/*! @brief Convert absolute values relative to the total number of voxels
			*/
			DataDifferentialType convertAbsoluteToRelative(bool isCumulative = true) const;

			/*	@brief Multiplies each values with its Delta value. Values depend on DVHType.
				@param The DVHType that is being used DVHType::Cumulative or DVHType::Differential
			*/
			std::map <DoseTypeGy, PercentType> getNormalizedDVH(DVHType dvhType = { DVHType::Cumulative }) const;
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
