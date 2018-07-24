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

#include "DummyModel.h"

namespace rttb
{
	namespace models
	{
		DummyModel::DummyModel(DVHPointer aDvh): BioModel(aDvh)
		{
			_calcCount = 0;
			_setParam1Count = 0;
			_setParam2Count = 0;
			_setParam3Count = 0;
			_name = "DummyModel";
		}

		BioModelValueType DummyModel::calcModel(const double doseFactor)
		{
			_calcCount++;
			_value = doseFactor;
			return _value;
		}

		void DummyModel::setParameterVector(const ParamVectorType& aParameterVector)
		{
			if (aParameterVector.size() != 3)
			{
				std::cerr << "aParameterVector.size must be 3! Nothing will be done." << std::endl;
			}
			else
			{
				_param1 = aParameterVector.at(0);
				_setParam1Count++;
				_param2 = aParameterVector.at(1);
				_setParam2Count++;
				_param3 = aParameterVector.at(2);
				_setParam3Count++;
			}
		}

		void DummyModel::setParameterByID(const int aParamId, const BioModelParamType aValue)
		{
			if (aParamId == 0)
			{
				_param1 = aValue;
				_setParam1Count++;
			}
			else if (aParamId == 1)
			{
				_param2 = aValue;
				_setParam2Count++;
			}
			else if (aParamId == 2)
			{
				_param3 = aValue;
				_setParam3Count++;
			}
			else
			{
				std::cerr <<
				          "aParamID must be 0(DummyParam1) or 1(DummyParam2) or 2(DummyParam3)! Nothing will be done." <<
				          std::endl;
			}

		}

		const int DummyModel::getParameterID(const std::string& aParamName) const
		{
			if (aParamName == "DummyParam1")
			{
				return 0;
			}
			else if (aParamName == "DummyParam2")
			{
				return 1;
			}
			else if (aParamName == "DummyParam3")
			{
				return 2;
			}
			else
			{
				std::cerr << "There is no parameter with the name " << aParamName << "!" << std::endl;
				return -1;
			}
		}

		void DummyModel::resetCounters()
		{
			_calcCount = 0;
			_setParam1Count = 0;
			_setParam2Count = 0;
			_setParam3Count = 0;
		}
		std::map<std::string, double> DummyModel::getParameterMap() const {
			return parameterMap;
		}

		void DummyModel::fillParameterMap(){
			parameterMap["Dummy"] = 2;
		}

		std::string DummyModel::getModelType() const{
			return _name;
		}
	}
}