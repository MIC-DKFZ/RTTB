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

#ifndef __DOSE_TO_VOLUME_MEASURE_COLLECTION_H
#define __DOSE_TO_VOLUME_MEASURE_COLLECTION_H

#include "rttbMeasureCollection.h"

namespace rttb
{

	namespace algorithms
	{
		/*! @class DoseToVolumeMeasureCollection
		@brief This class handels the access to the DoseToVolumeMeasureCollection elements for a specific complex statistic
		@note _referenceDose has to be set to use getValueRelative() otherwise an exception is thrown
		*/
		class RTTBAlgorithms_EXPORT DoseToVolumeMeasureCollection : public MeasureCollection {

		public:
			typedef std::map<DoseTypeGy, VolumeType> DoseToVolumeFunctionType;

		private:
			complexStatistics _name;
			DoseTypeGy _referenceDose;
			DoseToVolumeFunctionType _values;

		public:
			DoseToVolumeMeasureCollection(complexStatistics name, DoseTypeGy referenceDose = -1);
			
			/*! @brief This has to be set >=0 to use getValueRelative()
			*/
			void setReferenceDose(DoseTypeGy referenceDose);

			void insertValue(DoseTypeGy dose, VolumeType volume);

			/*! @brief Gets the volume irradiated with a dose >= x, depending on the complexStatistics name.
				@return Return absolute volume in absolute cm^3.
				@exception InvalidDoseException if the vector values is empty or _referenceDose is -1
				@exception NoDataException if the requested Dose is not in the vector
			*/
			VolumeType getValue(DoseTypeGy xVolumeAbsolute) const;
			VolumeType getValue(DoseTypeGy xVolumeAbsolute, bool findNearestValue, DoseTypeGy& nearestXDose) const;
			VolumeType getValueRelative(DoseTypeGy xDoseRelative) const;
			VolumeType getValueRelative(DoseTypeGy xDoseRelative, bool findNearestValue, DoseTypeGy& nearestXDose) const;
			DoseToVolumeFunctionType getAllValues() const;

			friend bool operator==(const DoseToVolumeMeasureCollection& volumeToDoseMesureCollection, const DoseToVolumeMeasureCollection& otherVolumeToDoseMesureCollection);
		};
	}
}


#endif
