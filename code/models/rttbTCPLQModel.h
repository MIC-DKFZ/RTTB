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
#ifndef __TCP_LQ_MODEL_H
#define __TCP_LQ_MODEL_H

#include <string>
#include <vector>
#include <map>

#include "rttbBaseType.h"
#include "rttbTCPModel.h"
#include "rttbBaseTypeModels.h"


namespace rttb
{

	namespace models
	{

		/*! @class TCPLQModel
			@brief This class represents a TCP(Tumor Control Probability) LQ model (Nahum and Sanchez-Nieto 2001,
			Hall and Giaccia 2006)
			@see TCPModel
		*/

		class TCPLQModel: public TCPModel
			{
			public:
				typedef TCPModel::ParamVectorType ParamVectorType;
				typedef TCPModel::DVHPointer DVHPointer;

			private:
				/*! @brief Calculate intermediate tcp using alpha constant. This is a helper function for calcTCP()
					@see calcTCP
				*/
				long double calcTCPi(BioModelParamType aRho, BioModelParamType aAlphaMean, double vj, double bedj);

			/*! @brief Calculate tcp using alpha constant.
				*/
			long double calcTCP(std::map<rttb::DoseTypeGy, rttb::DoseCalcType> aBEDDVH, BioModelParamType aRho,
					BioModelParamType aAlphaMean, double aDeltaV);

			/*! @brief Calculate tcp using a normal distribution for alpha.
				*/
			long double calcTCPAlphaNormalDistribution(std::map<rttb::DoseTypeGy, rttb::DoseCalcType> aBEDDVH,
			        BioModelParamType aRho, BioModelParamType aAlphaMean,
					BioModelParamType aAlphaVariance, double aDeltaV);

		protected:
				BioModelParamType _alphaMean;

				BioModelParamType _alphaVariance;

				BioModelParamType _alpha_beta;

				/*! Roh is the initial clonogenic cell density
				*/
				BioModelParamType _rho;


				/*! @brief Calculate the model value
				@param doseFactor scaling factor for prescribed dose. The model calculation will use the dvh
					with each di=old di*doseFactor.
          @pre _alphaMean >0
          @pre _alphaVariance >= 0
          @pre _alpha_beta > 0
          @pre _rho > 0
			@pre _numberOfFractions > 1
          @exception InvalidParameterException Thrown if parameters were not set correctly.
				*/
			BioModelValueType calcModel(const double doseFactor = 1);

			public:
				TCPLQModel();

				/*! @brief Constructor initializes member variables with given parameters.
			@pre aAlphaMean >0
			@pre aBeta > 0
			@pre aRho > 0
			@pre aNumberOfFractions > 1
				*/
			TCPLQModel(DVHPointer aDVH, BioModelParamType aAlphaMean, BioModelParamType aBeta, BioModelParamType aRho,
					int aNumberOfFractions);

				/*! @brief Constructor for alpha distribution initializes member variables with given parameters.
			@pre aAlphaMean >0
			@pre aAlphaVariance >0
			@pre aAlpha_Beta > 0
			@pre aRho > 0
			@pre aNumberOfFractions > 1
				*/
			TCPLQModel(DVHPointer aDVH, BioModelParamType aRho, int aNumberOfFractions, BioModelParamType aAlpha_Beta,
					BioModelParamType aAlphaMean, BioModelParamType aAlphaVariance);

				const BioModelParamType getRho();

				void setRho(const BioModelParamType aRho);

				const BioModelParamType getAlphaMean();

				const BioModelParamType getAlphaVariance();

			/*! @brief The distribution of the parameter alpha, which is characteristic for a population of cells,
					is described by the its mean and variance. If alpha is constant the variance is 0.
				@param aAlphaVariance  The variance of alpha can be given, the default value is 0 resulting in constant
					alpha.
				*/
			void setAlpha(const BioModelParamType aAlphaMean, const BioModelParamType aAlphaVariance = 0);

				const BioModelParamType getAlpahBeta();

				void setAlphaBeta(const BioModelParamType aAlpha_Beta);

				/*! @brief Set parameters for the TCP model. _value will be reset to 0.
				@param aAlpha_Beta alpha/beta constant .
				@param aAlphaMean mean of alpha distribution.
				@param aAlphaVariance variance of alpha distribution.
				*/
			void setParameters(const BioModelParamType aAlphaMean, const BioModelParamType aAlpha_Beta,
			                   const BioModelParamType aRho, const BioModelParamType aAlphaVariance = 0);

				/*! @brief Set parameter with ID. "alphaMean":0,"alphaVariance":1,"alpha_beta":2, "rho":3
        @exception InvalidParameterException Thrown if aParamId is not 0 or 1 or 2 or 3.
				*/
				virtual void setParameterByID(const int aParamId, const BioModelParamType aValue);

			/*! @brief Set parameter vector, where index of vector is the parameter id.
					"alphaMean":0,"alphaVariance":1,"alpha_beta":2, "rho":3
          @exception InvalidParameterException Thrown if aParamterVector.size()!=4.
				*/
			virtual void setParameterVector(const ParamVectorType& aParameterVector);

				/*! @brief Get parameter id. "alphaMean":0,"alphaVariance":1,"alpha_beta":2, "rho":3
        @return 0 for "alphaMean", 1 for "alphaVariance", 2 for "alpha_beta", 3 for "rho"
        @exception InvalidParameterException Thrown if aParamName is not alphaMean or alphaVariance or alpha_beta or rho.
				*/
			virtual const int getParameterID(const std::string& aParamName) const;
			};

		}//end algorithms
}//end rttb


#endif
