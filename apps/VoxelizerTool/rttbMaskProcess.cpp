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
// @version $Revision: 1377 $ (last changed revision)
// @date    $Date: 2016-06-07 10:26:43 +0200 (Di, 07 Jun 2016) $ (last change date)
// @author  $Author: strubel $ (last changed by)
*/
#include "rttbMaskProcess.h"

#include <boost/make_shared.hpp>

#include "rttbBoostMaskAccessor.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizerTool
		{
			MaskProcess::MaskProcess(StructureSetPointer rtStructureSet, DoseAccessorPointer doseAccessor,
			                         bool legacy, bool allowSelfIntersection) : _rtStructureSet(rtStructureSet),
				_doseAccessor(doseAccessor),
				_legacyVoxelization(legacy), _allowSelfIntersection(allowSelfIntersection)
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
						                  (_rtStructureSet->getStructure(indexOfStructure), _doseAccessor->getGeometricInfo(),
						                   !_allowSelfIntersection);
					}

					maskAccessorPtr->updateMask();
				}

				return maskAccessorPtr;

			}
		}
	}
}