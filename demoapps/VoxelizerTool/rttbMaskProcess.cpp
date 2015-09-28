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
// @version $Revision: 5603 $ (last changed revision)
// @date    $Date: 2015-08-24 15:17:30 +0200 (Mo, 24 Aug 2015) $ (last change date)
// @author  $Author: strubel $ (last changed by)
*/
#include "rttbMaskProcess.h"

#include <boost/make_shared.hpp>

#include "rttbBoostMaskAccessor.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizer
		{
			MaskProcess::MaskProcess(StructureSetPointer rtStructureSet, DoseAccessorPointer doseAccessor,
			                         bool legacy) : _rtStructureSet(rtStructureSet), _doseAccessor(doseAccessor),
				_legacyVoxelization(legacy)
			{
			}

			MaskProcess::MaskAccessorPointer MaskProcess::createMask(unsigned int indexOfStructure) const
			{
				MaskAccessorPointer maskAccessorPtr;

				if (_doseAccessor != NULL && _rtStructureSet != NULL)
				{
					if (_legacyVoxelization)
					{
						maskAccessorPtr = boost::make_shared<rttb::masks::legacy::OTBMaskAccessor>
						                  (_rtStructureSet->getStructure(indexOfStructure), _doseAccessor->getGeometricInfo());
					}
					else
					{
						maskAccessorPtr = boost::make_shared<rttb::masks::boost::BoostMaskAccessor>
						                  (_rtStructureSet->getStructure(indexOfStructure), _doseAccessor->getGeometricInfo());
					}

					maskAccessorPtr->updateMask();
				}

				return maskAccessorPtr;

			}
		}
	}
}