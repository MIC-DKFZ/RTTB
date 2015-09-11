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
#ifndef __DUMMY_DOSE_ACCESSOR_H
#define __DUMMY_DOSE_ACCESSOR_H

#include <vector>

#include "rttbAccessorWithGeoInfoBase.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"


namespace rttb
{
	namespace testing
	{

		/*! @class DummyDoseAccessor
			@brief A dummy DoseAccessor for testing filled with random dose values between 0 and 100.
			The default grid size is [11,10,5]
		*/
		class DummyDoseAccessor: public core::AccessorWithGeoInfoBase
		{

		private:
			/*! vector of dose data(absolute Gy dose/doseGridScaling)*/
			std::vector<DoseTypeGy> doseData;

			IDType _doseUID;


		public:
			~DummyDoseAccessor();

			/*! @brief A dummy DoseAccessor for testing filled with random dose values between 0 and 100.
				    The default grid size is [11,10,5]
			    */
			DummyDoseAccessor();

			/*! @brief Constructor.
				Initialisation of dose with a given vector.
			*/
			DummyDoseAccessor(const std::vector<DoseTypeGy>& aDoseVector, const core::GeometricInfo& geoInfo);

			const std::vector<DoseTypeGy>* getDoseVector() const
			{
				return &doseData;
			};

			GenericValueType getValueAt(const VoxelGridID aID) const;

			GenericValueType getValueAt(const VoxelGridIndex3D& aIndex) const;

			const IDType getUID() const
			{
				return _doseUID;
			};
		};
	}
}

#endif
