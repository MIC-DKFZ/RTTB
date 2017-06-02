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
// @version $Revision: 1674 $ (last changed revision)
// @date    $Date: 2017-01-27 10:34:46 +0100 (Fr, 27 Jan 2017) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#ifndef __VOLUME_TO_DOSE_MEASURE_H
#define __VOLUME_TO_DOSE_MEASURE_H

#include <vector>
#include <map>

#include "rttbBaseType.h"

#include "RTTBAlgorithmsExports.h"

#include "rttbDoseStatistics.h"

namespace rttb
{

	namespace algorithms
	{
		class RTTBAlgorithms_EXPORT VolumeToDoseMeasure {

		public:
			typedef std::map<VolumeType, DoseTypeGy> VolumeToDoseFunctionType;

		private:
			std::string name;
			VolumeToDoseFunctionType values;
			VolumeType _volume;

		public:
			VolumeToDoseMeasure(std::string name, VolumeToDoseFunctionType values, VolumeType volume);
			std::string getName() const;
			void insertValue(std::pair<VolumeType, DoseTypeGy> value);
			DoseTypeGy getValue(VolumeType xVolumeAbsolute);
			DoseTypeGy getValue(VolumeType xVolumeAbsolute, bool findNearestValue, VolumeType& nearestXDose);
			DoseTypeGy getValueRelative(VolumeType xDoseRelative);
			DoseTypeGy getValueRelative(VolumeType xDoseRelative, bool findNearestValue, VolumeType& nearestXDose);
			VolumeToDoseFunctionType getAllValues() const;
			friend bool operator==(const VolumeToDoseMeasure& volumeToDoseMesure, const VolumeToDoseMeasure& otherVolumeToDoseMesure);

		private:
			double getSpecificValue(double key, bool findNearestValueInstead, double& storedKey) const;
			std::map<double, double>::const_iterator findNearestKeyInMap(const std::map<double, double>& aMap, double key) const;
		};
	}
}


#endif
