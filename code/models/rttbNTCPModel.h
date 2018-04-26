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

#ifndef __NTCP_MODEL_H
#define __NTCP_MODEL_H


#include "rttbBioModel.h"
#include "rttbBaseTypeModels.h"



namespace rttb
{
	namespace models
	{

		/*! @class NTCPModel
			@brief This is the interface class for NTCP(Normal Tissue Complication Probability) models
		*/
		class NTCPModel: public BioModel
		{
		public:
			using ParamVectorType = BioModel::ParamVectorType;

		protected:
			BioModelParamType _d50{0};

		public:
			NTCPModel(): BioModel() {}
			NTCPModel(const BioModelParamType aD50): BioModel(), _d50(aD50) {}
			NTCPModel(core::DVH::Pointer aDvh, const BioModelParamType aD50): BioModel(aDvh), _d50(aD50) {}

			const BioModelParamType getD50()
			{
				return _d50;
			}

			void setD50(const BioModelParamType aD50)
			{
				_d50 = aD50;
			}
		};

	}//end namespace models
}//end namespace rttb

#endif
