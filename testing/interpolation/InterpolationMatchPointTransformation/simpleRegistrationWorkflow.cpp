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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

#undef MAP_SEAL_ALGORITHMS

#include "simpleRegistrationWorkflow.h"
#include "registrationHelper.h"

simpleRegistrationWorkflow::simpleRegistrationWorkflow(std::string targetFilename,
        std::string movingFilename,
        bool isDirectory) : _targetFilename(targetFilename), _movingFilename(movingFilename)
{
	setImageFileNames(_targetFilename, _movingFilename, isDirectory, globals);
	loadData(globals);
	_spAlgorithmEuler = nullptr;
}

vnl_vector<double> simpleRegistrationWorkflow::getRegistrationParameters(
    Registration3D3DPointer reg)
{
	typedef map::core::PreCachedRegistrationKernel<3, 3> PreCachedRegistrationKernel3D3D;

	const PreCachedRegistrationKernel3D3D* pModelBasedDirectKernel3D3D =
	    dynamic_cast<const PreCachedRegistrationKernel3D3D*>(&(reg->getDirectMapping()));

	if (pModelBasedDirectKernel3D3D)
	{
		PreCachedRegistrationKernel3D3D::TransformType::ParametersType params =
		    pModelBasedDirectKernel3D3D->getTransformModel()->GetParameters();

		return params;
	}
	else
	{
		return vnl_vector<double>();
	}
}

void simpleRegistrationWorkflow::initializeAndPerformRegistration()
{
	_spAlgorithmEuler = AlgorithmTypeEuler::New();
	_spAlgorithmEuler->setProperty("PreinitTransform", map::core::MetaProperty<bool>::New(true));
	_spAlgorithmEuler->setMovingImage(globals.spMovingImage);
	_spAlgorithmEuler->setTargetImage(globals.spTargetImage);
	AlgorithmTypeEuler::RegistrationType::Pointer spRegistration;

	try
	{
		spRegistration = _spAlgorithmEuler->getRegistration();
	}
	catch (const map::core::ExceptionObject& e)
	{
		std::cerr << "caught an MatchPoint exception:\n";
		e.Print(std::cerr);
		std::cerr << "\n";
	}
	catch (const itk::ExceptionObject& e)
	{
		std::cerr << "caught an ITK exception:\n";
		std::cerr << e.GetFile() << ":" << e.GetLine() << ":\n"
		          << e.GetDescription() << "\n";
	}
	catch (const std::exception& e)
	{
		std::cerr << "caught an exception:\n";
		std::cerr << e.what() << "\n";
	}
	catch (...)
	{
		std::cerr << "caught an unknown exception!!!\n";
	}
}

map::core::Registration<3, 3>::Pointer simpleRegistrationWorkflow::getRegistration()
{
	if (_spAlgorithmEuler.IsNull())
	{
		initializeAndPerformRegistration();
	}

	return _spAlgorithmEuler->getRegistration();
};

const itk::Image<ImageType::PixelType, 3>* simpleRegistrationWorkflow::getTargetImage()
{
	if (_spAlgorithmEuler.IsNull())
	{
		initializeAndPerformRegistration();
	}

	return _spAlgorithmEuler->getTargetImage();
};
