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

#ifndef __MODEL_CURVE_H
#define __MODEL_CURVE_H

#include "rttbBioModel.h"
#include "rttbBaseType.h"
#include "rttbNTCPLKBModel.h"

namespace rttb
{
	namespace models
	{
		//map of dose value and model value
		typedef std::map<double, double> CurveDataType;
		//pair of dose value and model value
		typedef std::pair<double, BioModelValueType> CurvePointType;

		/*! @brief Get the curve TCP/NTCP Value vs normalisationDose, normalisationDose variant between minDose and
			maxDose.
			@param bin the size of the map
			@param minDose min value for x axis
			@param maxDose max value for x axis
			@param normalisationDose prescribed dose of the current _dvh or mean/maximum.
		*/
		CurveDataType getCurveDoseVSBioModel(BioModel& aModel, double normalisationDose, int aBin = 201,
		                                     double minDose = 0.1, double maxDose = 150);

		/*! @brief Get the curve NTCP Value vs EUD, dvh variant between minFactor*deltaD and maxFactor*deltaD.
			@param bin the size of the map
			@param minFactor min factor for dvh deltaD
			@param maxFactor max factor for dvh deltaD
		*/
		CurveDataType getCurveEUDVSBioModel(NTCPLKBModel& aModel, DoseCalcType maxFactor = 10,
		                                    DoseCalcType minFactor = 0.1, int aBin = 201);
	}
}
#endif