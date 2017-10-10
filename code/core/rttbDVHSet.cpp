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

#include "rttbDVHSet.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace core
	{

		DVHSet::DVHSet(IDType aStructureSetID, IDType aDoseID) : _structureSetID(aStructureSetID),
			_doseID(aDoseID) {}

		DVHSet::DVHSet(DVHSetType aDVHTVSet, DVHSetType aDVHHTSet, IDType aStructureSetID, IDType aDoseID)
		{
			_dvhHTSet = aDVHHTSet;
			_dvhTVSet = aDVHTVSet;
			this->_structureSetID = aStructureSetID;
			this->_doseID = aDoseID;
		}

		DVHSet::DVHSet(DVHSetType aDVHTVSet, DVHSetType aDVHHTSet, DVHSetType aDVHWVSet,
		               IDType aStructureSetID, IDType aDoseID)
		{
			_dvhHTSet = aDVHHTSet;
			_dvhTVSet = aDVHTVSet;
			_dvhWVSet = aDVHWVSet;
			this->_structureSetID = aStructureSetID;
			this->_doseID = aDoseID;
		}

		std::size_t DVHSet::size() const
		{
			return _dvhHTSet.size() + _dvhTVSet.size() + _dvhWVSet.size();
		}

		void DVHSet::setStrSetID(IDType aStrSetID)
		{
			_structureSetID = aStrSetID;
		}
		void DVHSet::setDoseID(IDType aDoseID)
		{
			_doseID = aDoseID;
		}

		IDType DVHSet::getStrSetID() const
		{
			return _structureSetID;
		}
		IDType DVHSet::getDoseID() const
		{
			return _doseID;
		}

		DVH* DVHSet::getDVH(IDType structureID)
		{
			DVHSetType::iterator itTV = _dvhTVSet.begin();

			for (; itTV != _dvhTVSet.end(); ++itTV)
			{
				if ((*(itTV)).getStructureID() == structureID)
				{
					return &(*itTV);
				}
			}

			DVHSetType::iterator itHT = _dvhHTSet.begin();

			for (; itHT != _dvhHTSet.end(); ++itHT)
			{
				if ((*(itHT)).getStructureID() == structureID)
				{
					return &(*itHT);
				}
			}

			DVHSetType::iterator itWV = _dvhWVSet.begin();

			for (; itWV != _dvhWVSet.end(); ++itWV)
			{
				if ((*(itWV)).getStructureID() == structureID)
				{
					return &(*itWV);
				}
			}

			std::cout << "No DVH with the structure id: " << structureID << " was found!" << std::endl;
			return nullptr;
		}

		void DVHSet::insert(DVH& aDvh, DVHRole aDVHRole)
		{
			if (aDVHRole.Type == DVHRole::TargetVolume)
			{
				_dvhTVSet.push_back(aDvh);
			}
			else if (aDVHRole.Type == DVHRole::HealthyTissue)
			{
				_dvhHTSet.push_back(aDvh);
			}
			else if (aDVHRole.Type == DVHRole::WholeVolume)
			{
				_dvhWVSet.push_back(aDvh);
			}
			else
			{
				throw core::InvalidParameterException("aDVHType must be TV or HT or WV!");
			}
		}

		VolumeType DVHSet::getWholeVolume(DoseTypeGy aDoseAbsolute) const
		{
			VolumeType volume = this->getHealthyTissueVolume(aDoseAbsolute) + this->getTargetVolume(
			                        aDoseAbsolute);
			return volume;
		}

		VolumeType DVHSet::getHealthyTissueVolume(DoseTypeGy aDoseAbsolute) const
		{
			DVHSetType::const_iterator itHT = _dvhHTSet.begin();
			VolumeType volume = 0;

			while (itHT != _dvhHTSet.end())
			{
				VolumeType testVol = 0;
				DVH dvh = *(itHT);
				testVol = dvh.getVx(aDoseAbsolute);

				if (testVol >= 0)
				{
					volume += testVol;
				}

				++itHT;
			}

			return volume;

		}

		VolumeType DVHSet::getTargetVolume(DoseTypeGy aDoseAbsolute) const
		{
			DVHSetType::const_iterator itTV = _dvhTVSet.begin();
			VolumeType volume = 0;

			while (itTV != _dvhTVSet.end())
			{
				VolumeType testVol = 0;
				DVH dvh = *(itTV);
				testVol = dvh.getVx(aDoseAbsolute);

				if (testVol >= 0)
				{
					volume += testVol;
				}

				++itTV;
			}

			return volume;
		}

		bool operator==(const DVHSet& aDVHSet, const DVHSet& otherDVHSet)
		{
			if (aDVHSet.getStrSetID() != otherDVHSet.getStrSetID())
			{
				return false;
			}

			if (aDVHSet.getDoseID() != otherDVHSet.getDoseID())
			{
				return false;
			}

			if (aDVHSet.size() != otherDVHSet.size())
			{
				return false;
			}

			return true;
		}

		std::ostream& operator<<(std::ostream& s, const DVHSet& aDVHSet)
		{
			s << "[ " << aDVHSet.getStrSetID() << ", " << aDVHSet.getDoseID() << " ]";
			return s;
		}

		std::ostream& operator<<(std::ostream& s, const DVHSet::DVHSetType& aDVHSet)
		{
			s << "[ ";

			for (size_t i = 0; i < aDVHSet.size(); i++)
			{
				s << aDVHSet.at(i);
			}

			s << " ]";
			return s;
		}

	}
}

