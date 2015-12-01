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

#include <map>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "rttbBioModelCurve.h"
#include "rttbDvhBasedModels.h"

namespace rttb
{
	namespace models
	{

		CurveDataType getCurveDoseVSBioModel(BioModel& aModel, double normalisationDose, int aBin,
		                                     double minDose, double maxDose)
		{
			CurveDataType _curveData;

			double minFactor = 0, maxFactor = 0;

			minFactor = minDose / normalisationDose;
			maxFactor = maxDose / normalisationDose;

			double lastValue = 0;

			for (int i = 0; i < aBin; i++)
			{
				double factor = minFactor + i * (maxFactor - minFactor) / (aBin - 1);

				if (lastValue < 1)
				{
					aModel.init(factor);
					BioModelValueType value = aModel.getValue();
					lastValue = value;
					_curveData.insert(CurvePointType(factor * normalisationDose, value));
				}
				else
				{
					_curveData.insert(CurvePointType(factor * normalisationDose, 1));
				}
			}

			return _curveData;
		}

		CurveDataType getCurveEUDVSBioModel(NTCPLKBModel& aModel, DoseCalcType maxFactor,
		                                    DoseCalcType minFactor, int aBin)
		{
			CurveDataType _curveData;
			BioModel::DVHPointer _dvh = aModel.getDVH();

			for (int i = 0; i < aBin; i++)
			{
				DoseCalcType factor = minFactor + i * (maxFactor - minFactor) / (aBin - 1);

				core::DVH variantDVH = core::DVH(_dvh->getDataDifferential(), _dvh->getDeltaD() * factor,
				                                 _dvh->getDeltaV(),
				                                 "temporary", "temporary");

				boost::shared_ptr<core::DVH> spDVH = boost::make_shared<core::DVH>(variantDVH);
				double eud = getEUD(spDVH, aModel.getA());

				aModel.init(factor);
				BioModelValueType value = aModel.getValue();
				_curveData.insert(CurvePointType(eud, value));
			}

			return _curveData;
		}
	}
}