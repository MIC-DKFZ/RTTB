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


#ifndef __DOSE_MAP_APPLICATION_DATA_H
#define __DOSE_MAP_APPLICATION_DATA_H

#include "mapRegistration.h"

#include "rttbBaseType.h"
#include "rttbDoseAccessorInterface.h"

namespace rttb
{
  namespace apps
  {
    namespace doseMap
    {

      class ApplicationData
      {
      public:
        typedef map::core::Registration<3, 3> RegistrationType;

        /**Vector of arguments used to specify the loading style (always the first argument)
        * and, if needed, additional arguments for the specified loading style (e.g. location of the
        * Virtuos plan file for the Virtuos IO style).
        */
        typedef std::vector<std::string> LoadingStyleArgType;
        /** Loaded Dose.*/
        core::DoseAccessorInterface::DoseAccessorPointer _inputDose;
        std::string  _inputDoseFileName;
        LoadingStyleArgType  _inputDoseLoadStyle;
        core::DoseAccessorInterface::DoseAccessorPointer _refDose;
        std::string  _refDoseFileName;
        LoadingStyleArgType  _refDoseLoadStyle;
        RegistrationType::Pointer _spReg;
        std::string  _regFileName;

        std::string  _outputFileName;

        std::string  _interpolatorName;

        bool _showVersion;
        bool _showHelp;

        int _fileCount;

        void Reset();

        ApplicationData();
      };

      /** Parse the application argument passed when starting the application.
      * If no error or special request occurred the return is 0. Otherwise the return values
      * have the following meaning: \n
      * 0: Normal parsing.\n
      * 1: showed help or version (flag was set).\n
      * 2: not enough required input files.\n
      * 3: Parsing error.\n
      * @param argc Number of parameter arguments
      * @param argv Pointer to the passed arguments
      * @return Result code of the parsing (see above).**/
      unsigned int ParseArgumentsForAppData(int argc, char** argv, ApplicationData& appData);

    }
  }
}
#endif
