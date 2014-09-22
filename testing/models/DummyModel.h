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
#ifndef __DUMMY_MODEL_H
#define __DUMMY_MODEL_H


#include "rttbBaseType.h"
#include "rttbBaseTypeModels.h"
#include "rttbBioModel.h"

namespace rttb{

	namespace models{

		/*! @class DummyModel
		@brief generates a dummy model object used for unit tests
		*/
		class DummyModel: public BioModel{
		private:
			BioModelParamType _param1;
			BioModelParamType _param2;
			BioModelParamType _param3;

			int _calcCount;
			int _setParam1Count;
			int _setParam2Count;
			int _setParam3Count;

		protected:
			/*! @brief Calculate the model value
			@param doseFactor scaling factor for the dose. The model calculation will use the dvh with each di=old 
			di*doseFactor.
			*/
			BioModelValueType calcModel(const double doseFactor=1); 
		public:
			/*!@constructor initializes DVH pointer */
			DummyModel(DVHPointer aDvh);

			/*! @brief Set parameter vector, where index of vector is the parameter ID.
				The length of the vector has to be 3.
			*/
			void setParameterVector(const ParamVectorType aParameterVector);

			void setParameterByID(const int aParamId, const BioModelParamType aValue);

			/*! @brief Get parameter by ID. 
			@return Return -1 if ID is not found.
			*/
			const int getParameterID(std::string aParamName) const;

			/*! return counting values */
			const int getSetParam1Count() const {return _setParam1Count;};
			const int getSetParam2Count() const {return _setParam2Count;};
			const int getSetParam3Count() const {return _setParam3Count;};
			const int getCalcCount() const {return _calcCount;};

			void resetCounters();
		};
	}
}
#endif