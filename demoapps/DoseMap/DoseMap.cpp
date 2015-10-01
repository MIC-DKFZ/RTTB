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

#include "DoseMapApplicationData.h"
#include "DoseMapHelper.h"

#include "mapDummyRegistrationAlgorithm.h"

rttb::apps::doseMap::ApplicationData appData;

/**
 Main function of dose mapper.
 @retval 0 normal program execution
 @retval 1 showed help or version (flag was set).
 @retval 2 not enough required input files.
 @retval 3 Argument parsing error
 @retval 4 Error loading input dose file
 @retval 5 Error loading reference dose file
 @retval 6 Error loading registration
 @retval 9 Error while mapping or storing result.
*/
int main(int argc, char** argv)
{
  int result = 0;

  std::cout << "DoseMap - RTTB demo app for simple dose mapping." << std::endl;

  switch (rttb::apps::doseMap::ParseArgumentsForAppData(argc, argv, appData))
  {
    case 1:
    {
      //showed version or help info. Done.
      return 1;
    }

    case 2:
    {
      std::cerr << "Missing Parameters. Use one of the following flags for more information:" <<
                std::endl;
      std::cerr << "-? or --help" << std::endl;
      return 2;
    }

    case 3:
    {
      //wrong option usage.
      return 3;
    }
  }

  if (appData._fileCount < 2)
  {
    std::cerr << "Missing Parameters. Use one of the following flags for more information:" <<
              std::endl;
    std::cerr << "-? or --help" << std::endl;
    return 1;
  }

  std::cout << std::endl << "*******************************************" << std::endl;
  std::cout << "Input dose file:        " << appData._inputDoseFileName << std::endl;

  if (!(appData._regFileName.empty()))
  {
    std::cout << "Registration file: " << appData._regFileName << std::endl;
  }

  if (!(appData._refDoseFileName.empty()))
  {
    std::cout << "Reference dose file:        " << appData._refDoseFileName << std::endl;
  }

  try
  {
    appData._inputDose = rttb::apps::doseMap::loadDose(appData._inputDoseFileName,
                         appData._inputDoseLoadStyle);
  }
  catch (::itk::ExceptionObject& e)
  {
    std::cerr << "Error!!!" << std::endl;
    std::cerr << e << std::endl;
    return 4;
  }
  catch (std::exception& e)
  {
    std::cerr << "Error!!!" << std::endl;
    std::cerr << e.what() << std::endl;
    return 4;
  }
  catch (...)
  {
    std::cerr << "Error!!! unknown error while reading input image." << std::endl;
    return 4;
  }

  if (!(appData._refDoseFileName.empty()))
  {
    try
    {
      appData._refDose = rttb::apps::doseMap::loadDose(appData._refDoseFileName,
                         appData._refDoseLoadStyle);
    }
    catch (::itk::ExceptionObject& e)
    {
      std::cerr << "Error!!!" << std::endl;
      std::cerr << e << std::endl;
      return 5;
    }
    catch (std::exception& e)
    {
      std::cerr << "Error!!!" << std::endl;
      std::cerr << e.what() << std::endl;
      return 5;
    }
    catch (...)
    {
      std::cerr << "Error!!! unknown error while reading reference image." << std::endl;
      return 5;
    }
  }
  else
  {
    appData._refDose = appData._inputDose;
  }

  if (!(appData._regFileName.empty()))
  {
    try
    {
      appData._spReg = rttb::apps::doseMap::loadRegistration(appData._regFileName);
    }
    catch (::itk::ExceptionObject& e)
    {
      std::cerr << "Error!!!" << std::endl;
      std::cerr << e << std::endl;
      return 6;
    }
    catch (std::exception& e)
    {
      std::cerr << "Error!!!" << std::endl;
      std::cerr << e.what() << std::endl;
      return 6;
    }
    catch (...)
    {
      std::cerr << "Error!!! unknown error while reading registration file." << std::endl;
      return 6;
    }
  }
  else
  {
    //generate dummy identity registration
    typedef map::algorithm::DummyRegistrationAlgorithm<3>	DummyRegType;
    DummyRegType::Pointer regAlg = DummyRegType::New();

    appData._spReg = regAlg->getRegistration();
  }

  try
  {
    rttb::apps::doseMap::processData(appData);
  }
  catch (::itk::ExceptionObject& e)
  {
    std::cerr << "Error!!!" << std::endl;
    std::cerr << e << std::endl;
    return 9;
  }
  catch (std::exception& e)
  {
    std::cerr << "Error!!!" << std::endl;
    std::cerr << e.what() << std::endl;
    return 9;
  }
  catch (...)
  {
    std::cerr << "Error!!! unknown error while mapping and writing image." << std::endl;
    return 9;
  }

  std::cout << std::endl;

  return result;
}
