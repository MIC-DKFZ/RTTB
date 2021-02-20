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

#ifndef __TCP_MODEL_H
#define __TCP_MODEL_H

#include "rttbBioModel.h"

#include "RTTBModelsExports.h"

namespace rttb
{

	namespace models
	{


		/*! @class TCPModel
			@brief This is the interface class for TCP(Tumor Control Probability) models
		*/
		class RTTBModels_EXPORT TCPModel: public BioModel
		{
		public:
			using ParamVectorType = BioModel::ParamVectorType;

		protected:
			int _numberOfFractions{0};

		public:

			TCPModel(): BioModel() {};

      explicit TCPModel(int aNum): BioModel(), _numberOfFractions(aNum) {};

			TCPModel(core::DVH::Pointer aDvh, int aNum): BioModel(aDvh), _numberOfFractions(aNum) {};

			void setNumberOfFractions(const int aNumberOfFractions);

			const int getNumberOfFractions();

		};

	}//end namespace models
}//end namespace rttb

#endif
