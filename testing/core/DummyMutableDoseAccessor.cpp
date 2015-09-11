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

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "DummyMutableDoseAccessor.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbIndexOutOfBoundsException.h"

namespace rttb
{
	namespace testing
	{

		DummyMutableDoseAccessor::~DummyMutableDoseAccessor() {}

		DummyMutableDoseAccessor::DummyMutableDoseAccessor()
		{
			boost::uuids::uuid id;
			boost::uuids::random_generator generator;
			id = generator();
			std::stringstream ss;
			ss << id;
			_doseUID = "DummyMutableDoseAccessor_" + ss.str();

			SpacingVectorType3D aVector(2.5);
			_geoInfo.setSpacing(aVector);
			WorldCoordinate3D anOtherVector(-25, -2, 35);
			_geoInfo.setImagePositionPatient(anOtherVector);
			_geoInfo.setNumRows(11);
			_geoInfo.setNumColumns(10);
			_geoInfo.setNumSlices(5);

			OrientationMatrix unit = OrientationMatrix();
			_geoInfo.setOrientationMatrix(unit);

			for (int i = 0; i < _geoInfo.getNumberOfVoxels(); i++)
			{
				doseData.push_back((double(rand()) / RAND_MAX) * 1000);
			}
		}

		DummyMutableDoseAccessor::DummyMutableDoseAccessor(const std::vector<DoseTypeGy>& aDoseVector,
		        const core::GeometricInfo& geoInfo)
		{
			boost::uuids::uuid id;
			boost::uuids::random_generator generator;
			id = generator();
			std::stringstream ss;
			ss << id;
			_doseUID = "DummyMutableDoseAccessor_" + ss.str();

			doseData = aDoseVector;
			_geoInfo = geoInfo;
		}

		const core::GeometricInfo&
		DummyMutableDoseAccessor::
		getGeometricInfo() const
		{
			return _geoInfo;
		}

		DoseTypeGy DummyMutableDoseAccessor::getValueAt(const VoxelGridID aID) const
		{
			if (!_geoInfo.validID(aID))
			{
				throw core::IndexOutOfBoundsException("Not a valid Position!");
			}

			return doseData.at(aID);
		}

		DoseTypeGy DummyMutableDoseAccessor::getValueAt(const VoxelGridIndex3D& aIndex) const
		{
			VoxelGridID gridID = 0;
			_geoInfo.convert(aIndex, gridID);
			return getValueAt(gridID);
		}

		void DummyMutableDoseAccessor::setDoseAt(const VoxelGridID aID, DoseTypeGy value)
		{
			if (!_geoInfo.validID(aID))
			{
				throw core::IndexOutOfBoundsException("Not a valid Position!");
			}

			doseData.at(aID) = value;
		}

		void DummyMutableDoseAccessor::setDoseAt(const VoxelGridIndex3D& aIndex, DoseTypeGy value)
		{
			VoxelGridID gridID = 0;
			_geoInfo.convert(aIndex, gridID);
			setDoseAt(gridID, value);
		}

	}//end namespace testing
}//end namespace rttb