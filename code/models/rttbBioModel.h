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

#ifndef __BIO_MODEL_H
#define __BIO_MODEL_H

#include "rttbBaseType.h"
#include "rttbDVH.h"
#include "rttbBaseTypeModels.h"

#include "RTTBModelsExports.h"

#include <map>

namespace rttb
{

	namespace models
	{

		/*! @class BioModel
			@brief This is the interface class for biological models
		*/
		class RTTBModels_EXPORT BioModel
		{
		public:
			typedef std::vector<BioModelParamType> ParamVectorType;
			typedef core::DVH::DVHPointer DVHPointer;

		protected:
			DVHPointer _dvh;

			BioModelValueType _value;

			/*! @brief Calculate the model value
				@param doseFactor scaling factor for the dose. The model calculation will use the dvh with each di=old
				di*doseFactor.
			*/
			virtual BioModelValueType calcModel(const double doseFactor = 1) = 0;

			/* Map of all parameters
			*/
			std::map<std::string, double> parameterMap;

			std::string _name;


		public:
			BioModel(): _value(0) {};
			BioModel(DVHPointer aDvh): _dvh(aDvh), _value(0) {};

			/*! @brief Start the calculation. If any parameter changed, init() should be called again and return =true
				before getValue() is called!
				@return Return true if successful
			*/
			bool init(const double doseFactor = 1);

			/*! @param aDVH must be a DVH calculated by a cumulative dose distribution, not a fraction DVH!
			*/
			void setDVH(const DVHPointer aDVH);

			const DVHPointer getDVH() const;

			/*! @brief Set parameter vector, where index of vector is the parameter ID.
			*/
			virtual void setParameterVector(const ParamVectorType& aParameterVector) = 0;

			virtual void setParameterByID(const int aParamId, const BioModelParamType aValue) = 0;


			/*! @brief Get parameter by ID.
				@return Return -1 if ID is not found.
			*/
			virtual const int getParameterID(const std::string& aParamName) const = 0;

			virtual std::map<std::string, double> getParameterMap() const = 0;
			virtual void fillParameterMap() = 0 ;

			virtual std::string getModelType() const = 0;

			/*! @brief Get the value of biological model
				@pre init() must be called and =true!
			*/
			const BioModelValueType getValue() const;
		};

	}//end namespace models
}//end namespace rttb

#endif
