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

#ifndef __MODEL_SCATTER_H
#define __MODEL_SCATTER_H

#include "rttbBaseType.h"
#include "rttbBaseTypeModels.h"

#include "RTTBModelsExports.h"

namespace rttb
{
	namespace models
	{
        class BioModel;
		// maps dose to a pair of model value and probability
		typedef std::multimap<double , std::pair<double, double> > ScatterPlotType;
        using ParamVectorType = std::vector<BioModelParamType>;

		/*! @brief Get the points (TCP/NTCP Value, probability of the value) if 1 parameter vary from a normal-
		distribution with mean=aMean, variance=aVariance.
		@param aModel biological model for which the scatter plot will be generated
		@param aParamId ID of the parameter to be varied to generate the scatter plot
		@param aMean mean value for the distribution of the varied parameter
		@param aVariance variance of the varied parameter. The variance may not be exactly zero. If so, it is set to 1e-30
		to avoid numerical instability.
		@param aNormalisationDose prescribed dose of the current _dvh
		@param numberOfPoints the size of the map, number of points to be calculated
		@param aMinDose dose will be randomly selected from [aMinDose] (uniform distribution).
		They will define the minvalue for x axis
		@param aMaxDose dose will be randomly selected from [aMaxDose] (uniform distribution).
		They will define the max value for x axis
		@return Map of scattered values. If all parameters are valid, this map contains numberOfPoints valid scatter values.
		If aMaxDose<=aMinDose, the scatter plot cannot be generated. The map will therefore be empty.
		@warning This method is slow, do not use with too many points. Because the scatter plot map must contain numberOfPoints
		the scatter plot generation may run more often (producing invalid values). In tests the generation process runs on average
		approximately 20% more often.
		@exception InvalidParameterException Thrown if aNormalisationDose<=0 or aMinDose<=aMaxiDose
		*/
		RTTBModels_EXPORT ScatterPlotType getScatterPlotVary1Parameter(BioModel& aModel, int aParamId,
		        BioModelParamType aMean, BioModelParamType aVariance, DoseTypeGy aNormalisationDose,
		        int numberOfPoints = 100,
		        DoseTypeGy aMinDose = 0, DoseTypeGy aMaxDose = 150);

		/*! @brief Get the points (TCP/NTCP Value, probability of the value) if >=1 parameter vary from a normal-
		distribution with mean of parameter aParamIdVec.at(i)=aMeanVec.at(i), variance of parameter aParamIdVec.at(i)=
		aVarianceVec.at(i).
		@param aModel biological model for which the scatter plot will be generated
		@param aParamIdVec a vector containing the IDs of the parameters to be varied to generate the scatter plot
		@param aMeanVec a vector of mean values for the distribution of individually the varied parameters
		@param aVarianceVec a vector of variance values of the individually varied parameter. The variance may not be exactly zero for
		any parameter. If so, it is set to 1e-30 to avoid numerical instability.
		@param aNormalisationDose prescribed dose of the current _dvh
		@param numberOfPoints the size of the map, number of points to be calculated
		@param aMinDose dose will be randomly selected from [aMinDose] (uniform distribution).
		They will define the min value for x axis
		@param aMaxDose dose will be randomly selected from [aMaxDose] (uniform distribution).
		They will define the max value for x axis
		@throw InvalidParameterException is thrown if the parameter vectors do not have the same size.
		@return Map of scattered values. If all parameters are valid, this map contains numberOfPoints valid scatter values.
		If aMaxDose<=aMinDose, the scatter plot cannot be generated. The map will therefore be empty.
		@warning This method is very slow do not use with too many points. Because the scatter plot map must contain numberOfPoints
		the scatter plot generation may run more often (producing invalid values). In tests the generation process runs on average
		approximately 20% more often.
		@exception InvalidParameterException Thrown if aNormalisationDose<=0 or aMinDose<=aMaxiDose
		*/
		RTTBModels_EXPORT ScatterPlotType getScatterPlotVaryParameters(BioModel& aModel, std::vector<int> aParamIdVec,
		       ParamVectorType aMeanVec, ParamVectorType aVarianceVec,
		        DoseTypeGy aNormalisationDose, int numberOfPoints = 50, DoseTypeGy aMinDose = 0,
		        DoseTypeGy aMaxDose = 150);

		/*! Compute normal probability density function for zero mean at aValue with aVariance.
		*/
		RTTBModels_EXPORT double normal_pdf(double aValue, double aVariance);

	}
}

#endif
