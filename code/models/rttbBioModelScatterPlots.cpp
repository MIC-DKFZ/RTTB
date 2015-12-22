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

#include <ctime>

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>

#include "rttbBioModelScatterPlots.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace models
	{

		/* Initiate Random Number generator with current time */
		boost::random::mt19937 rng(static_cast<unsigned>(time(0)));
		/* Generate random number between 0 and 1 */
		boost::random::uniform_01<boost::mt19937> uniDist(rng);

		ScatterPlotType getScatterPlotVary1Parameter(BioModel& aModel, int aParamId,
		        BioModelParamType aMean, BioModelParamType aVariance, DoseTypeGy aNormalisationDose,
		        int numberOfPoints,
		        DoseTypeGy aMinDose, DoseTypeGy aMaxDose)
		{
			ScatterPlotType scatterPlotData;

			if (aVariance == 0)
			{
				//set to small positive value to avoid negative infinity!
				aVariance = 1e-30;
			}

			if (aMaxDose <= aMinDose)
			{
				throw core::InvalidParameterException("Parameter invalid: aMaxDose must be > aMinDose!");
			}

			if (aNormalisationDose <= 0)
			{
				throw core::InvalidParameterException("Parameter invalid: aNormalisationDose must be > 0!");
			}

			/* Choose Normal Distribution */
			boost::random::normal_distribution<double> gaussian_dist(0, aVariance);

			/* Create a Gaussian Random Number generator
			*  by binding with previously defined
			*  normal distribution object
			*/
			boost::random::variate_generator<boost::mt19937&, boost::normal_distribution<double> > generator(
			    rng, gaussian_dist);

			int i = 0;

			while (i < numberOfPoints)
			{
				double paramValue, probability;
				double randomValue = generator();
				paramValue = randomValue + aMean;
				probability = normal_pdf(randomValue, aVariance);

				if (probability > 1)
				{
					probability = 1;
				}

				//randomly select a dose between aMinDose and aMaxDose
				double dose = uniDist() * (aMaxDose - aMinDose) + aMinDose;

				if (probability > 0)
				{
					try
					{
						aModel.setParameterByID(aParamId, paramValue);
						aModel.init(dose / aNormalisationDose);
						double value = aModel.getValue();
						std::pair<double, double> modelProbPair = std::make_pair(value, probability);
						scatterPlotData.insert(std::pair<double, std::pair<double, double> >(dose, modelProbPair));
						i++;
					}
					catch (core::InvalidParameterException& /*e*/)
					{
						//repeat evaluation to guarantee the correct number of scatter values
						continue;
					}
				}
			}

			return scatterPlotData;
		}

		double normal_pdf(double aValue, double aVariance)
		{
			static const double inv_sqrt_2pi = 0.3989422804014327;
			double a = (aValue) / aVariance;

			return inv_sqrt_2pi / aVariance * std::exp(-0.5f * a * a);
		}

		ScatterPlotType getScatterPlotVaryParameters(BioModel& aModel,
		        std::vector<int> aParamIdVec, BioModel::ParamVectorType aMeanVec,
		        BioModel::ParamVectorType aVarianceVec,
		        DoseTypeGy aNormalisationDose, int numberOfPoints, DoseTypeGy aMinDose, DoseTypeGy aMaxDose)
		{

			ScatterPlotType scatterPlotData;

			if (aMaxDose <= aMinDose)
			{
				throw core::InvalidParameterException("Parameter invalid: aMaxDose must be > aMinDose!");
			}

			if (aNormalisationDose <= 0)
			{
				throw core::InvalidParameterException("Parameter invalid: aNormalisationDose must be > 0!");
			}

			//all input vectors need to have the same size
			if (((aVarianceVec.size() != aMeanVec.size()) || (aVarianceVec.size() != aParamIdVec.size())))
			{
				throw core::InvalidParameterException("Parameter vectors have different sizes!");
			}

			for (GridIndexType v = 0; v < aVarianceVec.size(); v++)
			{
				//set to small positive value to avoid negative infinity!
				if (aVarianceVec.at(v) == 0)
				{
					aVarianceVec.at(v) = 1e-30;
				}
			}

			double paramValue;


			// vary all parameters for each scattered point
			int i = 0;

			while (i < numberOfPoints)
			{
				double probability = 1;

				for (GridIndexType j = 0; j < aParamIdVec.size(); j++)
				{
					/* Choose Normal Distribution */
					boost::random::normal_distribution<double> gaussian_dist(0, aVarianceVec.at(j));

					/* Create a Gaussian Random Number generator
					*  by binding with previously defined
					*  normal distribution object
					*/
					boost::random::variate_generator<boost::mt19937&, boost::normal_distribution<double> > generator(
					    rng, gaussian_dist);

					double randomValue = generator();
					paramValue = randomValue + aMeanVec.at(j);

					if (aVarianceVec.at(j) != 0)
					{

						/* calculate combined probability */
						probability = probability * normal_pdf(randomValue, aVarianceVec.at(j));

					}
					else
					{
						throw core::InvalidParameterException("Parameter invalid: Variance should not be 0!");
					}

					aModel.setParameterByID(aParamIdVec.at(j), paramValue);
				}

				//randomly select a dose between aMinDose and aMaxDose
				double dose = uniDist() * (aMaxDose - aMinDose) + aMinDose;


				if (probability > 0)
				{
					try
					{
						aModel.init(dose / aNormalisationDose);
						double value = aModel.getValue();
						std::pair<double, double> modelProbPair = std::make_pair(value, probability);
						scatterPlotData.insert(std::pair<double, std::pair<double, double> >(dose, modelProbPair));
						i++;
					}
					catch (core::InvalidParameterException& /*e*/)
					{
						//repeat evaluation to guarantee the correct number of scatter values
						continue;
					}
				}
			}

			return scatterPlotData;
		}

	}//end namespace models
}//end namespace rttb
