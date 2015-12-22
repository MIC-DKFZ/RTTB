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

		typedef double integrationType;
		/*! @class LkbModelFunctor
			@brief A FunctorType: calculate the transformed LKB-Model using the transformation x = b - (1-t)/t.
		\int_{-\infty}^{b} dx f(x) = \int_0^1 dt f(b - (1-t)/t)/t^2
		*/
		class LkbModelFunctor
		{
		public:
			/*!b: upper bound of the lkb integration*/
			double b;

			/*! calculate the transformed LKB-Model using the transformation x = b - (1-t)/t..
			  */
			double calculate(double x) const;
		};

		/*! @class tcpModelFunctor
			@brief .A FunctorType: calculate the transformed TCP-LQ-Model using the transformation x = a + (1-t)/t.
		\int_{a}^{+\infty} dx f(x) =\int_0^1 dt f(a + (1-t)/t)/t^2
		*/
		class tcpModelFunctor
		{
		public:

			//TCP parameters including alpha mean, alpha variance, rho, volume vector and bed vector
			TcpParams params;

			/*! a: lower bound of the tcp integration*/
			double a;

			/*! calculate the transformed TCP-LQ-Model using the transformation x = a + (1-t)/t.
			  */
			double calculate(double x) const;
		};

		/*! Function to be integrated for TCP LQ model.
		@param x: variable of the TCP LQ model
		@param params: TCP parameters including alpha mean, alpha variance, rho, volume vector and bed vector
		@return Return the function value
		*/
		double tcpFunction(double x, const TcpParams& tcp_params);


		/*! Compute integration step for f(x) = exp(-pow(t,2)/2).
		@param x: variable of the lkb function
		@return Return the function value
		*/
		double lkbFunction(double x);


		/*! Calculate LKB Integration over(-infinity,b). The integral is mapped onto the semi-open interval (0,1] using the transformation x = b - (1-t)/t
		@param b: upper bound of the lkb integration
		*/
		double integrateLKB(double b);

		/*! Calculate TCP integration over (a, infinity). The integral is mapped onto the semi-open interval (0,1] using the transformation x = a + (1-t)/t
		@param a: lower bound of the tcp integration
		@param params: TCP parameters including alpha mean, alpha variance, rho, volume vector and bed vector
		*/
		double integrateTCP(double a, const TcpParams& params);


		/*This function returns the nth stage of refinement of the extended trapezoidal rule.
		  @param BMfunction: function to be integrated, for example a LkbModelFunctor or a tcpModelFunctor
		  @param a: lower bound of the integral
		  @param b: upper bound of the integral
		  @param stepNum: the nth stage
		  @param result: the current result
		*/
		template <typename FunctorType>
		integrationType trapzd(const FunctorType& BMfunction, integrationType a, integrationType b,
		                       int stepNum);

		/*! Iterative integration routine
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