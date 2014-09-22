#ifndef __MYREGISTRATION_H
#define __MYREGISTRATION_H

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#include "mapDemoHelloWorldRegistration1Helper.h"
#include "mapImageMappingTask.h"
#include "mapITKEuler3DMattesMIRegistrationAlgorithmTemplate.h"
#include "mapExceptionObject.h"

/*! @class registrationAlgorithm
	@brief implements a concrete registration algorithm of MatchPoint
*/
class registrationAlgorithm
{
private:
	std::string _targetFilename;
	std::string _movingFilename;
	std::string _targetDirectory;
	std::string _movingDirectory;

	bool _saveResults;

public:
	typedef map::core::Registration<3, 3> Registration3D3DType;
	typedef Registration3D3DType::Pointer Registration3D3DPointer;
	typedef map::algorithm::boxed::ITKEuler3DMattesMIRegistrationAlgorithm<ImageType>
	AlgorithmTypeEuler;
	typedef map::core::ImageMappingTask<AlgorithmTypeEuler::RegistrationType, ImageType, ImageType>
	ImageMappingTaskType;

	/*! @brief Constructor
	*/
	registrationAlgorithm(std::string targetFilename, std::string movingFilename,
	                      bool isDirectory = false, bool saveResult = false);
	map::core::Registration<3, 3>::Pointer getRegistration()
	{
		return _spAlgorithmEuler->getRegistration();
	};
	const itk::Image<ImageType::PixelType, 3>* getTargetImage()
	{
		return _spAlgorithmEuler->getTargetImage();
	};
	vnl_vector<double> getRegistrationParameters(Registration3D3DPointer reg);

private:
	AlgorithmTypeEuler::Pointer _spAlgorithmEuler;
};

#endif