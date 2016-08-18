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

#include <string>
#include <vector>
#include <exception>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "rttbIntegration.h"
#include "rttbNTCPLKBModel.h"
#include "rttbDvhBasedModels.h"
#include "rttbInvalidParameterException.h"
#include "rttbExceptionMacros.h"


namespace rttb
{

	namespace models
	{
		NTCPLKBModel::NTCPLKBModel() : NTCPModel(), _m(0), _a(0){
			_name = "NTCPLKBModel";
		}

		NTCPLKBModel::NTCPLKBModel(DVHPointer aDvh, BioModelParamType aD50, BioModelParamType aM,
		                           BioModelParamType aA):
								   NTCPModel(aDvh, aD50), _m(aM), _a(aA) {
									   _name = "NTCPLKBModel";
								   }

		void NTCPLKBModel::setA(const BioModelParamType aA)
		{
			_a = aA;
		}

		const BioModelParamType NTCPLKBModel::getA()
		{
			return _a;
		}

		void NTCPLKBModel::setM(const BioModelParamType aM)
		{
			_m = aM;
		}

		const BioModelParamType NTCPLKBModel::getM()
		{
			return _m;
		}

		void NTCPLKBModel::setParameterVector(const ParamVectorType& aParameterVector)
		{
			if (aParameterVector.size() != 3)
			{
				throw core::InvalidParameterException("Parameter invalid: aParameterVector.size must be 3! ");
			}
			else
			{
				_d50 = aParameterVector.at(0);
				_m = aParameterVector.at(1);
				_a = aParameterVector.at(2);
			}
		}

		void NTCPLKBModel::setParameterByID(const int aParamId, const BioModelParamType aValue)
		{
			if (aParamId == 0)
			{
				_d50 = aValue;
			}
			else if (aParamId == 1)
			{
				_m = aValue;
			}
			else if (aParamId == 2)
			{
				_a = aValue;
			}
			else
			{
				throw core::InvalidParameterException("Parameter invalid: aParamID must be 0(for d50) or 1(for m) or 2(for a)! ");
			}
		}

		const int NTCPLKBModel::getParameterID(const std::string& aParamName) const
		{
			if (aParamName == "d50")
			{
				return 0;
			}
			else if (aParamName == "m")
			{
				return 1;
			}
			else if (aParamName == "a")
			{
				return 2;
			}
			else
			{
				rttbExceptionMacro(core::InvalidParameterException,
				                   << "Parameter name " << aParamName << " invalid: it should be d50 or m or a!");

			}
		}

		std::map<std::string, double> NTCPLKBModel::getParameterMap(){
			parameterMap["d50"] = getD50();
			parameterMap["m"] = getM();
			parameterMap["a"] = getA();
			return parameterMap;
		}

		std::string NTCPLKBModel::getModelType() const{
			return _name;
		}

		BioModelValueType NTCPLKBModel::calcModel(const double doseFactor)
		{
			if (_a == 0)
			{
				throw core::InvalidParameterException("_a must not be zero");
			}

			if (_m == 0)
			{
				throw core::InvalidParameterException("_m must not be zero");
			}

			core::DVH variantDVH = core::DVH(_dvh->getDataDifferential(),
			                                 (DoseTypeGy)(_dvh->getDeltaD() * doseFactor),
			                                 _dvh->getDeltaV(), "temporary", "temporary");

			boost::shared_ptr<core::DVH> spDVH = boost::make_shared<core::DVH>(variantDVH);
			double eud = getEUD(spDVH, this->_a);
			//_m must not be zero
			double t = (eud - this->_d50) / (this->_m * this->_d50);
			double value = 1 / pow(2 * M_PI, 0.5);

			double result = integrateLKB(t);

			if (result != -100)
			{
				value *= result;

				return value;
			}
			else
			{
				return false;
			}
		}

	}//end namespace models
}//end namespace rttb
