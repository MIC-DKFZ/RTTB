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
#ifndef __NTCP_LKB_MODEL_H
#define __NTCP_LKB_MODEL_H

#include <string>
#include <vector>

#include "rttbBaseType.h"
#include "rttbNTCPModel.h"
#include "rttbBaseTypeModels.h"



namespace rttb{

	namespace models{

		/*! @class NTCPLKBModel
		@brief This class represents a NTCP(Normal Tissue Complication Probability) LKB model 
		(Lyman 1985, Kutcher and Burman 1989) 
		@see NTCPModel
		*/
		class NTCPLKBModel: public NTCPModel
			{
			public:
				typedef NTCPModel::ParamVectorType ParamVectorType;
				typedef NTCPModel::DVHPointer DVHPointer;

			private: 
				/*! The steepness of the dose-response curve. Must not be zero on model evaluation. 
				*/
				BioModelParamType _m;

				/*! Tumor or normal tissue-specific parameter that describes the dose–volume effect, 
				e.g. -10 for prostate (Wu 2002). Must not be zero on model evaluation, because EUD calculation will fail.
				*/
				BioModelParamType _a;

			protected:
				/*! @brief Calculate the model value
				*  @param doseFactor: scaling factor for the dose. The model calculation will use the dvh with each di=old di*doseFactor.
				*  @throw <InvalidParameterException> if either _a or _m is zero for the model calculation
				*/
				BioModelValueType calcModel(const double doseFactor=1); 

			public: 
				NTCPLKBModel();

				NTCPLKBModel(DVHPointer aDvh, BioModelParamType aD50, BioModelParamType aM, BioModelParamType aA);

				void setM(const BioModelParamType aM);

				const BioModelParamType getM();

				void setA(const BioModelParamType aA);

				const BioModelParamType getA();

				/*! @brief Set parameter with ID. "d50":0,"m":1,"a":2
        @exception InvalidParameterException Thrown if aParamId is not 0 or 1 or 2.
				*/
				virtual void setParameterByID(const int aParamId, const BioModelParamType aValue);

				/*! @brief Set parameter vector, where index of vector is the parameter ID. "d50":0,"m":1,"a":2
        @exception InvalidParameterException Thrown if aParamterVector.size()!=3.
				*/
				virtual void setParameterVector(const ParamVectorType aParameterVector);

				/*! @brief Get parameter ID. "d50":0,"m":1,"a":2
					@return 0 for "d50", 1 for "m", 2 for "a"
          @exception InvalidParameterException Thrown if aParamName is not d50 or m or a.
				*/
				virtual const int getParameterID(const std::string aParamName) const;
			};

		}
	}


#endif
