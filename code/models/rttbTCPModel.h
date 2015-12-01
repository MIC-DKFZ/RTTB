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
#ifndef __TCP_MODEL_H
#define __TCP_MODEL_H


#include "rttbBaseType.h"
#include "rttbBioModel.h"
#include "rttbBaseTypeModels.h"

namespace rttb
{

	namespace models
	{


		/*! @class TCPModel
			@brief This is the interface class for TCP(Tumor Control Probability) models
		*/
		class TCPModel: public BioModel
		{
		public:
			typedef BioModel::ParamVectorType ParamVectorType;
			typedef BioModel::DVHPointer DVHPointer;

		protected:
			int _numberOfFractions;

		public:

			TCPModel(): BioModel(), _numberOfFractions(0) {};

			TCPModel(int aNum): BioModel(), _numberOfFractions(aNum) {};

			TCPModel(DVHPointer aDvh, int aNum): BioModel(aDvh), _numberOfFractions(aNum) {};

			void setNumberOfFractions(const int aNumberOfFractions);

			const int getNumberOfFractions();

		};

	}//end namespace models
}//end namespace rttb

#endif
