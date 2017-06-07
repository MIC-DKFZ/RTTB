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

#ifndef __DOSE_TO_VOLUME_MEASURE_H
#define __DOSE_TO_VOLUME_MEASURE_H

#include "rttbMeasureInterface.h"

namespace rttb
{

	namespace algorithms
	{
		class RTTBAlgorithms_EXPORT DoseToVolumeMeasure : public MeasureInterface {

		public:
			typedef std::map<DoseTypeGy, VolumeType> DoseToVolumeFunctionType;

		private:
			complexStatistics name;
			DoseTypeGy _referenceDose;
			DoseToVolumeFunctionType values;

		public:
			DoseToVolumeMeasure(complexStatistics name, DoseToVolumeFunctionType values = std::map<DoseTypeGy, VolumeType>(), DoseTypeGy referenceDose = -1);
			
			void setReferenceDose(DoseTypeGy referenceDose);
			void insertValue(std::pair<DoseTypeGy, VolumeType> value);

			/*! @brief Gets the volume irradiated with a dose >= x, depending on the complexStatistics name.
				@return Return absolute volume in absolute cm^3.
				@exception NoDataException if the Vx values have not been set (i.e. the vector is empty)
				@excon NoDataException if the requested Dose is not in the vector
			*/
			VolumeType getValue(DoseTypeGy xVolumeAbsolute) const;
			VolumeType getValue(DoseTypeGy xVolumeAbsolute, bool findNearestValue, DoseTypeGy& nearestXDose) const;
			VolumeType getValueRelative(DoseTypeGy xDoseRelative) const;
			VolumeType getValueRelative(DoseTypeGy xDoseRelative, bool findNearestValue, DoseTypeGy& nearestXDose) const;
			DoseToVolumeFunctionType getAllValues() const;

			friend bool operator==(const DoseToVolumeMeasure& volumeToDoseMesure, const DoseToVolumeMeasure& otherVolumeToDoseMesure);
		};
	}
}


#endif
