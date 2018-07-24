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

#include <cmath>
#include <vector>

#include "rttbIntegration.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace models
	{

		double tcpModelFunctor::calculate(double x) const
		{
			if (x == 0)
			{
				x = 1e-30;
			}

			return tcpFunction(a + (1 - x) / x, params) / (x * x);
		}

		double LkbModelFunctor::calculate(double x) const
		{
			if (x == 0)
			{
				x = 1e-30;
			}

			return lkbFunction(b - (1 - x) / x) / (x * x);
		}


		double tcpFunction(double x, const TcpParams& tcp_params)
		{
			double alphaVariance = tcp_params.alphaVariance;

			if (alphaVariance == 0)
			{
				alphaVariance = 1e-30;
			}

			double f = exp(-pow((x - tcp_params.alphaMean) / alphaVariance, 2) / 2);

			for (size_t i = 0; i < tcp_params.volumeVector.size(); ++i)
			{
				double tmp, tmp1, tmp2, tmp3;
				tmp1 = exp(-x * tcp_params.bedVector.at(i));
				tmp2 = -(tcp_params.rho) * tcp_params.volumeVector.at(i);
				tmp3 = tmp2 * tmp1;
				tmp  = exp(tmp3);

				if (tmp != 1)
				{
					f = f * tmp;
				}
			}

			return f;
		}


		double integrateTCP(double a, const TcpParams& params)
		{
			double aNew = 1e-30;
			double bNew = 1.0;

			tcpModelFunctor BMFunction;
			BMFunction.params = params;
			BMFunction.a = a;

			return iterativeIntegration<tcpModelFunctor>(BMFunction, aNew, bNew);
		}

		double lkbFunction(double x)
		{

			double tmp = -pow(x, 2) / 2;

			double step = exp(tmp);

			return step;
		}


		double integrateLKB(double b)
		{

			double aNew = 1e-30;
			double bNew = 1.0;

			LkbModelFunctor BMFunction;
			BMFunction.b = b;

			return iterativeIntegration<LkbModelFunctor>(BMFunction, aNew, bNew);

		}

		//returns the nth stage of refinement of the extended trapezoidal rule
		template <typename FunctorType>
		integrationType trapzd(const FunctorType& BMfunction, integrationType a, integrationType b,
		                       int stepNum)
		{
			static integrationType result;

			if (stepNum == 1)
			{
				result = 0.5 * (b - a) * (BMfunction.calculate(a) + BMfunction.calculate(b));
			}
			else
			{
				integrationType x, tnm, sum, del;
				int it, j;

				for (it = 1, j = 1; j < stepNum - 1; j++)
				{
					it <<= 1;
				}

				tnm = it;
				del = (b - a) / tnm;
				x = a + 0.5 * del;

				for (sum = 0.0, j = 0; j < it; j++, x += del)
				{
					sum += BMfunction.calculate(x);
				}

				result = 0.5 * (result + (b - a) * sum / tnm);

			}

			return result;

		}

		template <typename FunctorType>
		integrationType iterativeIntegration(const FunctorType& BMfunction, integrationType a,
		                                     integrationType b)
		{
			integrationType ost = 0.0;
			integrationType os = 0.0;
			unsigned int maxSteps = 50;
			double eps = 1e-6;

			unsigned int i = 1;

			for (; i <= maxSteps; ++i)
			{
				integrationType st = trapzd(BMfunction, a, b, i);
				integrationType s = (4.0 * st - ost) / 3.0;

				if (i > 5)
				{
					if (fabs(s - os) < eps * fabs(os) || (s == 0.0 && os == 0.0))
					{
						return s;
					}
				}

				os = s;
				ost = st;
			}

			//too many iterations, this should never be reachable!
			throw rttb::core::InvalidParameterException("Integral calculation failed: too many iterations! ");

		}


	}
}
