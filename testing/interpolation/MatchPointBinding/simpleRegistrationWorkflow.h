// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html [^]
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision: 741 $ (last changed revision)
// @date    $Date: 2014-09-16 16:34:22 +0200 (Di, 16 Sep 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/
#ifndef __SIMPLE_REGISTRATION_WORKFLOW_H
#define __SIMPLE_REGISTRATION_WORKFLOW_H

#include <boost/shared_ptr.hpp>

#include "registrationHelper.h"

#include "mapImageMappingTask.h"
#include "mapITKEuler3DMattesMIRegistrationAlgorithmTemplate.h"
#include "mapExceptionObject.h"

/*! @class simpleRegistrationWorkflow
	@brief implements a concrete registration algorithm of MatchPoint
*/
class simpleRegistrationWorkflow
{
public:
	typedef map::core::Registration<3, 3> Registration3D3DType;
	typedef Registration3D3DType::Pointer Registration3D3DPointer;
	typedef map::algorithm::boxed::ITKEuler3DMattesMIRegistrationAlgorithm<ImageType>
	AlgorithmTypeEuler;
private:
	std::string _targetFilename;
	std::string _movingFilename;
	std::string _targetDirectory;
	std::string _movingDirectory;
	AppGlobals globals;
	AlgorithmTypeEuler::Pointer _spAlgorithmEuler;

public:
	/*! @brief Constructor
	*/
	simpleRegistrationWorkflow(std::string targetFilename, std::string movingFilename,
	                           bool isDirectory = false);
	map::core::Registration<3, 3>::Pointer getRegistration();
	const itk::Image<ImageType::PixelType, 3>* getTargetImage();
	vnl_vector<double> getRegistrationParameters(Registration3D3DPointer reg);

protected:
	void initializeAndPerformRegistration();
};

#endif