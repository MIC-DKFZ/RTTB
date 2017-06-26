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

#ifndef __VOLUME_TO_DOSE_MEASURE_COLLECTION_H
#define __VOLUME_TO_DOSE_MEASURE_COLLECTION_H

#include "rttbMeasureCollection.h"

namespace rttb
{

	namespace algorithms
	{
		/*! @class VolumeToDoseMeasureCollection
		@brief This class handels the access to the VolumeToDoseMeasureCollection elements for a specific complex statistic
		@note _volume has to be set to use getValueRelative() otherwise an exception is thrown
		*/
		class RTTBAlgorithms_EXPORT VolumeToDoseMeasureCollection : public MeasureCollection {

		public:
			typedef std::map<VolumeType, DoseTypeGy> VolumeToDoseFunctionType;

		private:
			VolumeType _volume;
			complexStatistics _name;
			VolumeToDoseFunctionType _values;

		public:
			VolumeToDoseMeasureCollection(complexStatistics name, VolumeType volume = -1);

			/*! @brief This has to be set >=0 to use getValueRelative()
			*/
			void setVolume(VolumeType volume);

			void insertValue(VolumeType volume, DoseTypeGy dose);

			/*! @brief Gets the x of the current volume, depending on the complexStatistics name.
				@return Return dose value in Gy.
				@exception InvalidDoseException if the vector values is empty
				@exception DataNotAvailableException if _volume is not set
			*/
			DoseTypeGy getValue(VolumeType xVolumeAbsolute) const;
			DoseTypeGy getValue(VolumeType xVolumeAbsolute, bool findNearestValue, VolumeType& nearestXDose) const;
			DoseTypeGy getValueRelative(VolumeType xDoseRelative) const;
			DoseTypeGy getValueRelative(VolumeType xDoseRelative, bool findNearestValue, VolumeType& nearestXDose) const;
			VolumeToDoseFunctionType getAllValues() const;

			friend bool operator==(const VolumeToDoseMeasureCollection& volumeToDoseMesureCollection, const VolumeToDoseMeasureCollection& otherVolumeToDoseMesureCollection);
		};
	}
}


#endif
