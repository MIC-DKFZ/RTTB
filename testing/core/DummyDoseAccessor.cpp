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

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "DummyDoseAccessor.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbIndexOutOfBoundsException.h"

namespace rttb
{
	namespace testing
	{

		DummyDoseAccessor::~DummyDoseAccessor() {}

		DummyDoseAccessor::DummyDoseAccessor()
		{
			boost::uuids::uuid id;
			boost::uuids::random_generator generator;
			id = generator();
			std::stringstream ss;
			ss << id;
			_doseUID = "DummyDoseAccessor_" + ss.str();

      assembleGeometricInfo();

			for (int i = 0; i < _geoInfo.getNumberOfVoxels(); i++)
			{
				doseData.push_back((double(rand()) / RAND_MAX) * 1000);
			}
		}

		DummyDoseAccessor::DummyDoseAccessor(const std::vector<DoseTypeGy>& aDoseVector,
		                                     const core::GeometricInfo& geoInfo)
		{
			boost::uuids::uuid id;
			boost::uuids::random_generator generator;
			id = generator();
			std::stringstream ss;
			ss << id;
			_doseUID = "DummyDoseAccessor_" + ss.str();

			doseData = aDoseVector;
			_geoInfo = geoInfo;
		}

    void DummyDoseAccessor::assembleGeometricInfo() {
      SpacingVectorType3D aVector(2.5);
      _geoInfo.setSpacing(aVector);
      WorldCoordinate3D anOtherVector(-25, -2, 35);
      _geoInfo.setImagePositionPatient(anOtherVector);
      _geoInfo.setNumRows(11);
      _geoInfo.setNumColumns(10);
      _geoInfo.setNumSlices(10);

      OrientationMatrix unit = OrientationMatrix();
      _geoInfo.setOrientationMatrix(unit);
    }

		GenericValueType DummyDoseAccessor::getValueAt(const VoxelGridID aID) const
		{
			if (!_geoInfo.validID(aID))
			{
				throw core::IndexOutOfBoundsException("Not a valid Position!");
			}

			return doseData.at(aID);
		}

		GenericValueType DummyDoseAccessor::getValueAt(const VoxelGridIndex3D& aIndex) const
		{
			VoxelGridID gridID = 0;
			_geoInfo.convert(aIndex, gridID);
			return getValueAt(gridID);
		}

	}//end namespace testing
}//end namespace rttb