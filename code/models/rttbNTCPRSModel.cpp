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

#include <exception>

#define _USE_MATH_DEFINES
#include <math.h>

#include "rttbNTCPRSModel.h"
#include "rttbInvalidParameterException.h"
#include "rttbExceptionMacros.h"

namespace rttb
{

	namespace models
	{
		NTCPRSModel::NTCPRSModel(): NTCPModel(), _gamma(0), _s(0) {}

		NTCPRSModel::NTCPRSModel(DVHPointer aDvh, BioModelParamType aD50, BioModelParamType aGamma, BioModelParamType aS):
			NTCPModel(aDvh, aD50), _gamma(aGamma), _s(aS) {}

		void NTCPRSModel::setGamma(const BioModelParamType aGamma)
		{
			_gamma = aGamma;
		}

		const BioModelParamType NTCPRSModel::getGamma()
		{
			return _gamma;
		}

		void NTCPRSModel::setS(const BioModelParamType aS)
		{
			_s = aS;
		}

		const BioModelParamType NTCPRSModel::getS()
		{
			return _s;
		}

		void NTCPRSModel::setParameterVector(const ParamVectorType& aParameterVector)
		{
			if (aParameterVector.size() != 3)
			{
				throw core::InvalidParameterException("Parameter invalid: aParameterVector.size must be 3! ");
			}
			else
			{
				_d50 = aParameterVector.at(0);
				_gamma = aParameterVector.at(1);
				_s = aParameterVector.at(2);
			}
		}

		void NTCPRSModel::setParameterByID(const int aParamId, const BioModelParamType aValue)
		{
			if (aParamId == 0)
			{
				_d50 = aValue;
			}
			else if (aParamId == 1)
			{
				_gamma = aValue;
			}
			else if (aParamId == 2)
			{
				_s = aValue;
			}
			else
			{
				throw core::InvalidParameterException("Parameter invalid: aParamID must be 0(for d50) or 1(for gamma) or 2(for s)! ");
			}

		}

		const int NTCPRSModel::getParameterID(const std::string& aParamName) const
		{
			if (aParamName == "d50")
			{
				return 0;
			}
			else if (aParamName == "gamma")
			{
				return 1;
			}
			else if (aParamName == "s")
			{
				return 2;
			}
			else
			{
				rttbExceptionMacro(core::InvalidParameterException,
				                   << "Parameter name " << aParamName << " invalid: it should be d50 or gamma or s!");
			}
		}

		const double NTCPRSModel::poissonModel(const double dose)
		{
			//_d50 must not be zero
			return pow(2, -exp(M_E * this->_gamma * (1 - dose / this->_d50)));
		}

		BioModelValueType NTCPRSModel::calcModel(double doseFactor)
		{
			if (_d50 == 0)
			{
				throw core::InvalidParameterException("d50 must not be zero");
			}

			if (_s == 0)
			{
				throw core::InvalidParameterException("s must not be zero");
			}

			std::deque<DoseCalcType> dataDifferential = this->_dvh->getDataDifferential();
			double ntcp = 1;

			for (GridIndexType i = 0; i < dataDifferential.size(); i++)
			{
				double pd = pow(this->poissonModel(i * this->_dvh->getDeltaD() * doseFactor), this->_s);

				double vi = dataDifferential[i] / this->_dvh->getNumberOfVoxels();
				ntcp *= pow((1 - pd), vi);
			}

			//_s must not be zero
			return (BioModelValueType)(pow((1 - ntcp), 1 / this->_s));
		}

	}//end namespace models
}//end namespace rttb

