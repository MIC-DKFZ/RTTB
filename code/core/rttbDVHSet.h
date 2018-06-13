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

#ifndef __DVH_SET_H
#define __DVH_SET_H

#include <map>
#include <vector>
#include <ostream>

#include "rttbCommon.h"
#include "rttbBaseType.h"
#include "rttbDVH.h"

#include "RTTBCoreExports.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{
	namespace core
	{

		/*! @class DVHSet
			@brief This is a class representing a RT DVHSet including Target Volume and Organ at Risk.
			@details A DVHSet consists of three subsets: one for the target volume (_dvhTVSet), one for healthy tissue (_dvhHTSet),
			and one for the whole volume (_dvhWVSet).
		*/
		class RTTBCore_EXPORT DVHSet
		{
		public:
      rttbClassMacroNoParent(DVHSet)
			using DVHSetType = std::vector<DVH>;
			using IndexType = DVHSetType::size_type;

		private:
			IDType _structureSetID;
			IDType _doseID;
			DVHSetType _dvhTVSet;
			DVHSetType _dvhHTSet;
			DVHSetType _dvhWVSet;

		public:
			DVHSet(IDType aStructureSetID = "", IDType aDoseID = "");
			DVHSet(DVHSetType aDVHTVSet, DVHSetType aDVHHTSet, IDType aStructureSetID = "",
			       IDType aDoseID = "");
			DVHSet(DVHSetType aDVHTVSet, DVHSetType aDVHHTSet, DVHSetType aDVHWVSet,
			       IDType aStructureSetID = "",
			       IDType aDoseID = "");

			/*! @brief Get the size of the DVHSet, that is the sum of the numbers of DVHs in all sub-sets.
			*/
			std::size_t size() const;

			void setStrSetID(IDType aStrSetID);
			void setDoseID(IDType aDoseID);

			IDType getStrSetID() const;
			IDType getDoseID() const;

			/*! @brief Get the DVH according to the structure ID
				@return Return nullptr if not found
			*/
			DVH* getDVH(IDType aStructureID);

			/*! @brief Insert a DVH object.
				@brief param aDVHType "TV" for target volume or "HT" for healthy tissue or "WV" for whole volume
			@exception InvalidParameterException Thrown if no valid DVHRole was given.
			*/
			void insert(DVH& aDvh, DVHRole aDVHRole);

			/*! @brief Get DVH subset for target volume
			*/
			const DVHSetType& getTargetVolumeSet() const
			{
				return _dvhTVSet;
			};

			/*! @brief Get DVH subset for healthy tissue
			*/
			const DVHSetType& getHealthyTissueSet() const
			{
				return _dvhHTSet;
			};

			/*! @brief Get DVH subset for whole volume
			*/
			const DVHSetType& getWholeVolumeSet() const
			{
				return _dvhWVSet;
			};


			/*! @brief Get the whole volume irradiated to >= aDoseAbsolute
			*/
			VolumeType getWholeVolume(DoseTypeGy aDoseAbsolute) const;

			/*! @brief Get the healthy tissue volume irradiated to >= aDoseAbsolute
				@return Return -1 if DVH of _dvhHTSet init() failed
			*/
			VolumeType getHealthyTissueVolume(DoseTypeGy aDoseAbsolute) const;

			/*! @brief Get the target volume irradiated to >= aDoseAbsolute
				@return Return -1 if DVH of _dvhTVSet init() failed
			*/
			VolumeType getTargetVolume(DoseTypeGy aDoseAbsolute) const;

			/*! DVHSets are considered equal if they have the same structureSet, dose and voxelization ID
				and the number of DVHs are equal.
			*/
			bool friend operator==(const DVHSet& aDVHSet, const DVHSet& otherDVHSet);

			friend std::ostream& operator<<(std::ostream& s, const DVHSet& aDVHSet);

			friend std::ostream& operator<<(std::ostream& s, const DVHSetType& aDVHSet);
		};

		bool operator==(const DVHSet& aDVHSet, const DVHSet& otherDVHSet);

		std::ostream& operator<<(std::ostream& s, const DVHSet& aDVHSet);

		std::ostream& operator<<(std::ostream& s, const DVHSet::DVHSetType& aDVHSet);

	}
}

#endif
