// -----------------------------------------------------------------------
// MatchPoint - DKFZ translational registration framework
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See mapCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/MatchPoint/copyright.html
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
// Subversion HeadURL: $HeadURL: https://svn/sbr/Sources/SBR-Projects/MatchPoint/trunk/Examples/Algorithms/mapDemoHelloWorldRegistration1.cpp $
*/

#undef MAP_SEAL_ALGORITHMS

#include "registrationAlgorithm.h"
#include "mapDemoHelloWorldRegistration1Helper.h"

registrationAlgorithm::registrationAlgorithm(std::string targetFilename, std::string movingFilename, bool isDirectory, bool writeResultsFile)
{
	_targetFilename = targetFilename;
	_movingFilename = movingFilename;
	AppGlobals globals;
	int result = EXIT_FAILURE;

	_saveResults = writeResultsFile;

	setImageFileNames(_targetFilename, _movingFilename, "result.dcm", isDirectory, globals);

	//load image and landmark files
	std::cout << "Load images and landmarks..." << std::endl;

	loadData(globals);

	std::cout << "Establish registration algorithm..." << std::endl;

	_spAlgorithmEuler = AlgorithmTypeEuler::New();
	//to ensure that the Geometric centers of both images are matched
	//all other parameters are default
	_spAlgorithmEuler->setProperty("PreinitTransform",map::core::MetaProperty<bool>::New(true));

	//Finally we set moving and target image for that should be
	//used by the image based registration algorithm
	_spAlgorithmEuler->setMovingImage(globals.spMovingImage);
	_spAlgorithmEuler->setTargetImage(globals.spTargetImage);

	//The algorithm is set up and ready to run...
	std::cout << "Starting to determine the registration..." << std::endl;
	AlgorithmTypeEuler::RegistrationType::Pointer spRegistration;
	try {
		spRegistration = _spAlgorithmEuler->getRegistration();
	}
	catch (const map::core::ExceptionObject& e)
	{
		std::cerr << "caught an MatchPoint exception:\n";
		e.Print(std::cerr);
		std::cerr << "\n";
		result = -1;
	}
	catch (const itk::ExceptionObject& e)
	{
		std::cerr << "caught an ITK exception:\n";
		std::cerr << e.GetFile() << ":" << e.GetLine() << ":\n"
			<< e.GetDescription() << "\n";
		result = -1;
	}
	catch (const std::exception& e)
	{
		std::cerr << "caught an exception:\n";
		std::cerr << e.what() << "\n";
		result = -1;
	}
	catch (...)
	{
		std::cerr << "caught an unknown exception!!!\n";
		result = -1;
	}

	std::cout << std::endl << "Registration determined..." << std::endl;
	//Thats all. Now we have a registration that map the moving image into the target image
	//space. But the algorithm only delivers the registration as a mapping function between
	//moving and target image space. So the moving image must be mapped...

	if (_saveResults){
		//define registration task
		std::cout << "Define registration task..." << std::endl;

		ImageMappingTaskType::Pointer spImageTask = ImageMappingTaskType::New();
		spImageTask->setInputImage(globals.spMovingImage);

		spImageTask->setRegistration(spRegistration);
		spImageTask->setResultImageDescriptor(map::core::createFieldRepresentation(*(_spAlgorithmEuler->getTargetImage().GetPointer())));	

		//Process the task
		std::cout << "Process registration task..." << std::endl;
		spImageTask->execute();

		globals.spResultImage = spImageTask->getResultImage();

		saveResults(globals);
	}

}

vnl_vector<double> registrationAlgorithm::getRegistrationParameters(Registration3D3DPointer reg){
	typedef map::core::ModelBasedRegistrationKernel<3, 3> ModelBasedRegistrationKernel3D3D;

	const ModelBasedRegistrationKernel3D3D* pModelBasedDirectKernel3D3D = dynamic_cast<const ModelBasedRegistrationKernel3D3D*>(&(reg->getDirectMapping()));

	if (pModelBasedDirectKernel3D3D)
	{
		ModelBasedRegistrationKernel3D3D::ParametersType params =
			pModelBasedDirectKernel3D3D->getTransformModel()->getTransform()->GetParameters();

		return params;
	}
	else 
		return vnl_vector<double>();
}
