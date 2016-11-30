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
// @version $Revision $ (last changed revision)
// @date $Date $ (last change date)
// @author $Author $ (last changed by)
*/
#include <sstream>

#include "rttbGeometricInfo.h"
#include "rttbDoseAccessorTester.h"

namespace rttb
{

	namespace testing
	{

		DoseAccessorTester::DoseAccessorTester(DoseAccessorPointer aReferenceDose,
		                                       DoseAccessorPointer aCompareDose) :
			_referenceDose(aReferenceDose), _compareDose(aCompareDose), _geometryIsSimilar(false),
			_sameGridSize(false), _conversionFailed(false),
			_numDifference(0)
		{}

		void DoseAccessorTester::setReferenceDose(const DoseAccessorPointer aReferenceDose)
		{
			_referenceDose = aReferenceDose;
		}
		void DoseAccessorTester::setCompareDose(const DoseAccessorPointer aCompareDose)
		{
			_compareDose = aCompareDose;
		}

		lit::StringType DoseAccessorTester::getTestDescription(void) const
		{
			return "Compare two DoseAccessors and determine if the contained doses are equal.";
		};

		bool DoseAccessorTester::doCheck(void) const
		{
			_pResults->onTestStart(getCurrentTestLabel());
			_geometryIsSimilar = (_referenceDose->getGeometricInfo() == _compareDose->getGeometricInfo());

			if (!_geometryIsSimilar)
			{
				return false;
			}

			_sameGridSize = (_referenceDose->getGridSize() == _compareDose->getGridSize());

			if (!_sameGridSize)
			{
				return false;
			}

			_numDifference = 0;
			_maxDifference = 0;
			VoxelGridIndex3D id3D;

			for (VoxelGridID id = 0; id < _referenceDose->getGridSize(); id++)
			{
				_compareDose->getGeometricInfo().convert(id, id3D);

				if (!_compareDose->getGeometricInfo().validIndex(id3D))
				{
					_conversionFailed = true;
					_failedID = id;
					return false;
				}

				if ((_referenceDose-> getValueAt(id) != _referenceDose-> getValueAt(id3D))
				    || (_compareDose->getValueAt(id) != _compareDose-> getValueAt(id3D)))
				{
					_conversionFailed = true;
					_failedID = id;
					return false;
				}

				if ((_referenceDose->getValueAt(id) != _compareDose->getValueAt(id))
				    || (_referenceDose->getValueAt(id3D) != _compareDose->getValueAt(id3D)))
				{
					double tmpDifference = std::abs(_referenceDose->getValueAt(id) - _compareDose->getValueAt(id));

					if (tmpDifference > _maxDifference)
					{
						_maxDifference = tmpDifference;
					}

					_numDifference++;
				}
			}//end for(VoxelGridID id = 0; id < _referenceDose->getGridSize(); id++)

			return (_numDifference == 0);
		}

		void DoseAccessorTester::handleSuccess(void) const
		{
			std::ostringstream stream;
			stream << "Both doses are equal" << std::endl;

			_pResults->onTestSuccess(getCurrentTestLabel(), stream.str());
		}

		void DoseAccessorTester::handleFailure(void) const
		{
			std::ostringstream stream;
			stream << "Doses are different" << std::endl;

			if (_geometryIsSimilar)
			{
				if (_sameGridSize)
				{
					stream << std::endl << "Error voxel count:   " << _numDifference << std::endl;
					stream << std::endl << "Maximum difference:  " << _maxDifference << std::endl;

					if (_conversionFailed)
					{
						stream << std::endl << "Index conversion failed in:  " << _failedID << std::endl;
					}
				}
				else
				{
					stream << "Doses have different grid sizes" << std::endl;
					stream << "Reference dose contains " << _referenceDose->getGridSize() <<
					       " voxels and comparison dose " <<
					       _compareDose->getGridSize() << std::endl;
				}
			}
			else
			{
				stream << "Doses have different geometry" << std::endl;
			}

			_pResults->onTestFailure(getCurrentTestLabel(), stream.str());
		}

	}
}