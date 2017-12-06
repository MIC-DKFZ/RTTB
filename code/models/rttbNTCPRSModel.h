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

#ifndef __NTCP_RS_MODEL_H
#define __NTCP_RS_MODEL_H

#include <string>

#include "rttbNTCPModel.h"
#include "rttbBaseTypeModels.h"


namespace rttb
{

	namespace models
	{

		/*! @class NTCPRSModel
			@brief This class represents a NTCP(Normal Tissue Complication Probability) relative seriality model (Kaellman 1992)
			@see NTCPModel
		*/
		class NTCPRSModel: public NTCPModel
		{
		public:
			typedef NTCPModel::ParamVectorType ParamVectorType;
			typedef NTCPModel::DVHPointer DVHPointer;

		private:
			/*! _gamma The normalised dose-response gradient, values between 1.7 and 2.0 are typical for human tumours.
			(Kaellman 1992)
			*/
			BioModelParamType _gamma{0};

			/*! _s The relative seriality factor, e.g. s=3.4 for the esophagus (highly serial structure) and s=0.0061
			for the lung(highly parallel structure). Must not be zero on model evaluation.
			*/
			BioModelParamType _s{0};

			const double poissonModel(const double dose);

		protected:
			/*! @brief Calculate the model value
				@param doseFactor scaling factor for the dose. The model calculation will use the dvh with each di=old di*doseFactor.
				@throw <InvalidParameterException>  if either _s or _d50 is zero for the model calculation.
			*/
			BioModelValueType calcModel(const double doseFactor = 1) override;

		public:
			NTCPRSModel();

			/*!@brief Constructor initializing all member variables with given parameters.
			*/
			NTCPRSModel(DVHPointer aDvh, BioModelParamType aD50, BioModelParamType aGamma,
			            BioModelParamType aS);

			void setGamma(const BioModelParamType aGamma);

			const BioModelParamType getGamma();

			void setS(const BioModelParamType aS);

			const BioModelParamType getS();

			/*! @brief Set parameter with ID. "d50":0,"gamma":1,"s":2
			@exception InvalidParameterException Thrown if aParamId is not 0 or 1 or 2.
			*/
			void setParameterByID(const int aParamId, const BioModelParamType aValue) override;

			/*! @brief Set parameter vector, where index of vector is the parameter Id.
				"d50":0,"gamma":1,"s":2
			@exception InvalidParameterException Thrown if aParamterVector.size()!=3.
			*/
			void setParameterVector(const ParamVectorType& aParameterVector) override;

			/*! @brief Get parameter ID. "d50":0,"gamma":1,"s":2
				@return 0 for "d50", 1 for "gamma", 2 for "s"
			@exception InvalidParameterException Thrown if aParamName is not d50 or gamma or s.
			*/
			const int getParameterID(const std::string& aParamName) const override;

			std::map<std::string, double> getParameterMap() const override;

			void fillParameterMap() override;

			std::string getModelType() const override;
		};

	}
}


#endif
