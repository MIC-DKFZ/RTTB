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

#include <sstream>

#include "rttbMaskRectStructTester.h"
#include "rttbGeometricInfo.h"

namespace rttb
{

	namespace testing
	{

		MaskRectStructTester::MaskRectStructTester(MaskAccessorPointer aMaskAccessor, GridIndexType z)
		{
			_maskAccessor = aMaskAccessor;
			_referenceList = _maskAccessor->getRelevantVoxelVector();

			_differentSlice = false;
			_outsideStructure = false;
			_wrongRVF = false;
			_conversionFailed = false;

			_zIndex = z;
		}

		void MaskRectStructTester::setAccessor(const MaskAccessorPointer aMaskAccessor)
		{
			_maskAccessor = aMaskAccessor;
			_referenceList = _maskAccessor->getRelevantVoxelVector();
		}
		void MaskRectStructTester::setZ(const GridIndexType z)
		{
			_zIndex = z;
		}

		lit::StringType MaskRectStructTester::getTestDescription(void) const
		{
			return "Check if generated MaskVoxelList corresponds to the given rectangular structure.";
		};

		bool MaskRectStructTester::doCheck(void) const
		{
			_pResults->onTestStart(getCurrentTestLabel());

			_differentSlice = false;
			_outsideStructure = false;
			_wrongRVF = false;
			_conversionFailed = false;

			VoxelGridIndex3D indexOTB;
			MaskVoxelList::iterator iterR;
			VoxelGridID index = 0;

			for (iterR = _referenceList->begin(); iterR != _referenceList->end(); ++iterR, ++index)
			{
				if (!(_maskAccessor->getGeometricInfo().convert(iterR->getVoxelGridID(), indexOTB)))
				{
					_conversionFailed = true;
					_failedListIndex = index;
					break;
				}

				if (indexOTB.z() != _zIndex)
				{
					_differentSlice = true;
					_failedListIndex = index;
					_wrongVal = indexOTB.z();
					break;
				}
				//corners are [2,1],[5,1],[5,5],[2,5], relevant voxels need to lie inside
				else if ((indexOTB.x() < 2) || (indexOTB.x() > 5))
				{
					_outsideStructure = true;
					_failedListIndex = index;
					break;
				}
				else if ((indexOTB.y() < 1) || (indexOTB.y() > 5))
				{
					_outsideStructure = true;
					_failedListIndex = index;
					break;
				}
				//check corners
				else if ((indexOTB.x() == 2 && indexOTB.y() == 1) || (indexOTB.x() == 2 && indexOTB.y() == 5) ||
				         (indexOTB.x() == 5 && indexOTB.y() == 1) || (indexOTB.x() == 5 && indexOTB.y() == 5))
				{
					if (0.25 != iterR->getRelevantVolumeFraction())
					{
						_wrongRVF = true;
						_failedListIndex = index;
						_wrongVal = iterR->getRelevantVolumeFraction();
						break;
					}
				}
				else if ((indexOTB.x() == 2) || (indexOTB.y() == 5) || (indexOTB.y() == 1) || (indexOTB.x() == 5))
				{
					if (0.5 != iterR->getRelevantVolumeFraction())
					{
						_wrongRVF = true;
						_failedListIndex = index;
						_wrongVal = iterR->getRelevantVolumeFraction();
						break;
					}
				}
				//check sides
				else  //should be inside
				{
					if (1 != iterR->getRelevantVolumeFraction())
					{
						_wrongRVF = true;
						_failedListIndex = index;
						_wrongVal = iterR->getRelevantVolumeFraction();
						break;
					}
				}
			}//end for(iterR = _referenceList->begin(); iterR != _referenceLi...

			if (_conversionFailed || _differentSlice || _outsideStructure || _wrongRVF)
			{
				return false;
			}

			return true;
		}

		void MaskRectStructTester::handleSuccess(void) const
		{
			std::ostringstream stream;
			stream << "The Voxelization is inside the given rectangle" << std::endl;

			_pResults->onTestSuccess(getCurrentTestLabel(), stream.str());
		}

		void MaskRectStructTester::handleFailure(void) const
		{
			std::ostringstream stream;
			stream << "Voxelization was not correct" << std::endl;

			if (_conversionFailed)
			{
				stream << std::endl << "The conversion failed at position" << _failedListIndex << std::endl;
			}

			if (_differentSlice)
			{
				stream << std::endl << "The voxelisation was not in plane " << _zIndex << " at position " <<
				       _failedListIndex << " was in " << _wrongVal << std::endl;
			}

			if (_outsideStructure)
			{
				stream << std::endl << "The voxelisation was outside the rectangle at position " << _failedListIndex
				       << std::endl;
			}

			if (_wrongRVF)
			{
				stream << std::endl << "The voxelisation had the wrong relevant volume fraction at position " <<
				       _failedListIndex << " was in " << _wrongVal <<  std::endl;
			}

			_pResults->onTestFailure(getCurrentTestLabel(), stream.str());
		}

	}
}