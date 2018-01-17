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
#ifndef __INTEGRATION_H
#define __INTEGRATION_H

#include "rttbBaseTypeModels.h"

namespace rttb
{
	namespace models
	{

		using integrationType = double;
		/*! @class LkbModelFunctor
			@brief A FunctorType: calculate the transformed LKB-Model
            @details LBK Model is calculated using the transformation \f$x = b - \frac{1-t}{t}\f$. 
		\f$\int_{-\infty}^{b} dx\f$, \f$f(x) = \int_0^1 dt \frac{f(b - \frac{(1-t)}{t})}{t^2}\f$
		*/
		class LkbModelFunctor
		{
		public:
			/*!b: upper bound of the lkb integration*/
			double b;

			/*! calculate the transformed LKB-Model using the transformation \f$x = b - \frac{1-t}{t}.\f$
			  */
			double calculate(double x) const;
		};

		/*! @class tcpModelFunctor
			@brief A FunctorType: calculate the transformed TCP-LQ-Model
            @details LBK Model is calculated using the transformation \f$x = a + \frac{1-t}{t}\f$.
		\f$\int_{a}^{+\infty} dx f(x) =\int_0^1 dt \frac{f(a + \frac{(1-t)}{t}}{t^2}\f$
		*/
		class tcpModelFunctor
		{
		public:

			//TCP parameters including alpha mean, alpha variance, rho, volume vector and bed vector
			TcpParams params;

			/*! a: lower bound of the tcp integration*/
			double a;

			/*! calculate the transformed TCP-LQ-Model using the transformation \f$x = a + \frac{1-t}{t}\f$.
			  */
			double calculate(double x) const;
		};

		/*! @brief Function to be integrated for TCP LQ model.
		@param x: variable of the TCP LQ model
		@param tcp_params: TCP parameters including alpha mean, alpha variance, rho, volume vector and bed vector
		@return Return the function value
		*/
		double tcpFunction(double x, const TcpParams& tcp_params);


		/*! @brief Compute integration step for \f$f(x) = exp(-\frac{t^2}{2})\f$.
		@param x: variable of the lkb function
		@return Return the function value
		*/
		double lkbFunction(double x);


		/*! @brief Calculate LKB Integration over \f$(-\infty,b)\f$. The integral is mapped onto the semi-open interval \f$(0,1]\f$ using the transformation \f$x = b - \frac{1-t}{t}\f$
		@param b: upper bound of the lkb integration
		*/
		double integrateLKB(double b);

		/*! @brief Calculate TCP integration over \f$(a, \infty)\f$. The integral is mapped onto the semi-open interval \f$(0,1]\f$ using the transformation \f$x = a + \frac{1-t}{t}\f$
		@param a: lower bound of the tcp integration
		@param params: TCP parameters including alpha mean, alpha variance, rho, volume vector and bed vector
		*/
		double integrateTCP(double a, const TcpParams& params);


		/* @brief This function returns the nth stage of refinement of the extended trapezoidal rule.
		  @param BMfunction: function to be integrated, for example a LkbModelFunctor or a tcpModelFunctor
		  @param a: lower bound of the integral
		  @param b: upper bound of the integral
		  @param stepNum: the nth stage
		  @param result: the current result
		*/
		template <typename FunctorType>
		integrationType trapzd(const FunctorType& BMfunction, integrationType a, integrationType b,
		                       int stepNum);

		/*! @brief Iterative integration routine
		@param BMfunction: function to be integrated, for example a LkbModelFunctor or a tcpModelFunctor
		@param a: lower bound of the integral
		@param b: upper bound of the integral
		@exception throw InvalidParameterException if integral calculation failed.
		*/
		template <typename FunctorType>
		integrationType iterativeIntegration(const FunctorType& BMfunction, integrationType a,
		                                     integrationType b);


	}
}
#endif