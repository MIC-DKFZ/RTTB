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

#define _USE_MATH_DEFINES
#include <cmath>

#include <exception>
#include <map>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "rttbTCPLQModel.h"
#include "rttbDvhBasedModels.h"
#include "rttbIntegration.h"
#include "rttbInvalidParameterException.h"
#include "rttbExceptionMacros.h"


namespace rttb
{

	namespace models
	{

		TCPLQModel::TCPLQModel(): TCPModel() {
			_name = "TCPLQModel";
			fillParameterMap();
		}

		TCPLQModel::TCPLQModel(DVHPointer aDVH, BioModelParamType aAlphaMean, BioModelParamType aBeta,
		                       BioModelParamType aRho,
		                       int aNumberOfFractions): TCPModel(aDVH, aNumberOfFractions), _alphaMean(aAlphaMean),
			_alphaVariance(0),
			_alpha_beta(aAlphaMean / aBeta), _rho(aRho)  {
			_name = "TCPLQModel";
			fillParameterMap();
		}


		TCPLQModel::TCPLQModel(DVHPointer aDVH, BioModelParamType aRho, int aNumberOfFractions,
		                       BioModelParamType aAlpha_Beta,
		                       BioModelParamType aAlphaMean, BioModelParamType aAlphaVariance): TCPModel(aDVH, aNumberOfFractions),
			_alphaMean(aAlphaMean),
			_alphaVariance(aAlphaVariance), _alpha_beta(aAlpha_Beta), _rho(aRho) {
			fillParameterMap();
			_name = "TCPLQModel";
		}

		void TCPLQModel::setParameters(const BioModelParamType aAlphaMean,
		                               const BioModelParamType aAlpha_Beta,
		                               const BioModelParamType aRho, const BioModelParamType aAlphaVariance)
		{
			_alphaMean = aAlphaMean;
			_alphaVariance = aAlphaVariance;
			_alpha_beta = aAlpha_Beta;
			_rho = aRho;

			//reset _value, because parameters have changed.
			_value = 0;
		}

		void TCPLQModel::setAlpha(const BioModelParamType aAlphaMean,
		                          const BioModelParamType aAlphaVariance)
		{
			_alphaVariance = aAlphaVariance;
			_alphaMean = aAlphaMean;
		}

		void TCPLQModel::setAlphaBeta(const BioModelParamType aAlpha_Beta)
		{
			_alpha_beta = aAlpha_Beta;
		}

		void TCPLQModel::setRho(const BioModelParamType aRho)
		{
			_rho = aRho;
		}

		const BioModelParamType TCPLQModel::getAlphaBeta()
		{
			return _alpha_beta;
		}

		const BioModelParamType TCPLQModel::getAlphaMean()
		{
			return _alphaMean;
		}

		const BioModelParamType TCPLQModel::getAlphaVariance()
		{
			return _alphaVariance;
		}

		const BioModelParamType TCPLQModel::getRho()
		{
			return _rho;
		}

		long double TCPLQModel::calcTCPi(BioModelParamType aRho, BioModelParamType aAlphaMean, double vj,
		                                 double bedj)
		{
			return exp(-aRho * vj * exp(-aAlphaMean * bedj));
		}

		long double TCPLQModel::calcTCP(std::map<rttb::DoseTypeGy, rttb::DoseCalcType> aBEDDVH,
		                                BioModelParamType aRho,
		                                BioModelParamType aAlphaMean, double aDeltaV)
		{
			std::map<rttb::DoseTypeGy, rttb::DoseCalcType>::iterator it;
			long double tcp = 1;

			for (it = aBEDDVH.begin(); it != aBEDDVH.end(); ++it)
			{
				long double tcpi = this->calcTCPi(aRho, aAlphaMean, (*it).second * aDeltaV, (*it).first);
				tcp = tcp * tcpi;
			}

			return tcp;
		}

		long double TCPLQModel::calcTCPAlphaNormalDistribution(
		    std::map<rttb::DoseTypeGy, rttb::DoseCalcType> aBEDDVH,
		    BioModelParamType aRho, BioModelParamType aAlphaMean,
		    BioModelParamType aAlphaVariance, double aDeltaV)
		{

			std::map<rttb::DoseTypeGy, rttb::DoseCalcType>::iterator it;
			std::vector<DoseCalcType> volumeV2;
			std::vector<DoseTypeGy> bedV2;
			int i = 0;

			for (it = aBEDDVH.begin(); it != aBEDDVH.end(); ++it)
			{
				volumeV2.push_back((*it).second * aDeltaV);
				bedV2.push_back((*it).first);
				i++;
			}

			struct TcpParams params = {aAlphaMean, aAlphaVariance, aRho, volumeV2, bedV2};

			double result = integrateTCP(0, params);

			if (result == -100)
			{
				std::cerr << "Integration error!\n";
				return -1;
			}
			else
			{
				long double tcp = 1 / (pow(2 * M_PI, 0.5) * _alphaVariance) * result;

				return tcp;
			}
		}

		BioModelValueType TCPLQModel::calcModel(const double doseFactor)
		{
			core::DVH variantDVH = core::DVH(_dvh->getDataDifferential(),
			                                 (DoseTypeGy)(_dvh->getDeltaD() * doseFactor),
			                                 _dvh->getDeltaV(), "temporary", "temporary");
			boost::shared_ptr<core::DVH> spDVH = boost::make_shared<core::DVH>(variantDVH);

			BioModelValueType value = 0;

			if (_alphaVariance == 0)
			{
				if (_alphaMean <= 0 || _alpha_beta <= 0 || _rho <= 0)
				{
					throw core::InvalidParameterException("Parameter invalid: alpha, alpha/beta, rho and number of fractions must >0!");
				}

				if (_numberOfFractions <= 1)
				{
					throw core::InvalidParameterException("Parameter invalid: numberOfFractions must be >1! The dvh should be an accumulated-dvh of all fractions, not a single fraction-dvh!");
				}

				std::map<rttb::DoseTypeGy, rttb::DoseCalcType> dataBED = calcBEDDVH(spDVH, _numberOfFractions,
				        _alpha_beta);

				value = (BioModelValueType)this->calcTCP(dataBED, _rho, _alphaMean, variantDVH.getDeltaV());
				return value;
			}

			//if alpha normal distribution
			else
			{
				if (this->_alpha_beta <= 0 || this->_alphaMean <= 0 || this->_alphaVariance < 0 || _rho <= 0)
				{
					throw core::InvalidParameterException("Parameter invalid: alpha/beta, alphaMean, rho and number of fractions must >0!");
				}

				if (_numberOfFractions <= 1)
				{
					throw core::InvalidParameterException("Parameter invalid: numberOfFractions must be >1! The dvh should be an accumulated-dvh of all fractions, not a single fraction-dvh!");
				}

				std::map<rttb::DoseTypeGy, rttb::DoseCalcType> dataBED = calcBEDDVH(spDVH, _numberOfFractions,
				        _alpha_beta);
				value = (BioModelValueType)(this->calcTCPAlphaNormalDistribution(dataBED, _rho, _alphaMean,
				                            _alphaVariance,
				                            variantDVH.getDeltaV()));
				return value;
			}
		}

		void TCPLQModel::setParameterVector(const ParamVectorType& aParameterVector)
		{
			if (aParameterVector.size() != 4)
			{
				throw core::InvalidParameterException("Parameter invalid: aParameterVector.size must be 4! ");
			}
			else
			{
				_alphaMean = aParameterVector.at(0);
				_alphaVariance = aParameterVector.at(1);
				_alpha_beta = aParameterVector.at(2);
				_rho = aParameterVector.at(3);
			}
		}

		void TCPLQModel::setParameterByID(const int aParamId, const BioModelParamType aValue)
		{
			if (aParamId == 0)
			{
				_alphaMean = aValue;
			}
			else if (aParamId == 1)
			{
				_alphaVariance = aValue;
			}
			else if (aParamId == 2)
			{
				_alpha_beta = aValue;
			}
			else if (aParamId == 3)
			{
				_rho = aValue;
			}
			else
			{
				throw core::InvalidParameterException("Parameter invalid: aParamID must be 0(alphaMean) or 1(alphaVariance) or 2(alpha_beta) or 3(rho)! ");
			}

		}

		const int TCPLQModel::getParameterID(const std::string& aParamName) const
		{
			if (aParamName == "alphaMean")
			{
				return 0;
			}
			else if (aParamName == "alphaVariance")
			{
				return 1;
			}
			else if (aParamName == "alpha_beta")
			{
				return 2;
			}
			else if (aParamName == "rho")
			{
				return 3;
			}
			else
			{
				rttbExceptionMacro(core::InvalidParameterException,
				                   << "Parameter name " << aParamName <<
				                   " invalid: it should be alphaMean or alphaVariance or alpha_beta or rho!");

			}
		}

		std::map<std::string, double> TCPLQModel::getParameterMap() const{
			return parameterMap;
		}

		void TCPLQModel::fillParameterMap(){
			parameterMap["numberOfFraction"] = getNumberOfFractions();
			parameterMap["alphaMean"] = getAlphaMean();
			parameterMap["alphaVariance"] = getAlphaVariance();
			parameterMap["alpha_beta"] = getAlphaBeta();
			parameterMap["rho"] = getRho();
		}

		std::string TCPLQModel::getModelType() const{
			return _name;
		}

	}//end namespace models
}//end namespace rttb


