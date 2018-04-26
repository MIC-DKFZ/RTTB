# RTToolbox

RTToolbox is a software library to support quantitative analysis of treatment outcome for radiotherapy.

The RTToolbox was designed following object-oriented design (OOD) principles and was implemented in the language C++. 

Features include:

* import of radiotherapy data (e.g. dose distributions and structure sets) from DICOM-RT format and other standard image processing formats
* DVH calculation
* Dose statistic calculation
* arithmetic operations on dose distributions
* structure relationship analyses (e.g. fully-contained, partially-contained)
* Calculation of dose comparison indices such as Conformity Index (CI), Homogeneity Index (HI) and Conformation Number (CN)
* Calculation of biological models including TCP, NTCP, EUD and BED

Also, the RTToolbox provides apps e.g. for DVH/Dose Statistic calculation or Dose accumulation that provides a convenient access of RT scenarios without computer-science knowledge.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

#### Build system

* [CMake](https://cmake.org), version 3.1 or higher

#### Compiler

* Visual Studio 2013
* Visual Studio 2015
* Visual Studio 2017
* GCC 5.4

Other compiler may work as well, but are not tested.

#### Linking Static/Dynamic library support

Can be changed with advanced option `BUILD_SHARED_LIBS`

:warning: building RTToolbox as dynamic library under Windows and as static library under Linux is an experimental feature.

#### Third party libraries

* [boost](http://www.boost.org), version 1.64.0 or higher
* [DCMTK](http://dicom.offis.de/dcmtk.php.en), with RT support - 3.6.1_20121102 or newer
* [ITK](https://itk.org), version 4.4 or higher (*optional*)
  * for DoseInterpolation support with ITK transformation or ITK File IO support
* [MatchPoint](http://mitk.org/download/thirdparty/MatchPoint_rev1610.tar.gz), version 0.12 or higher (*optional*)
  * for DoseInterpolation support with MatchPoint registration objects

:information_source: To make sure everything runs smoothly, please make sure that all libraries and the RTToolbox are either compiled with `/MD` or `/MT` flags.

##### Boost

Build (using the same compiler options as RTToolbox, usually `STATIC LINKING` and `x64` architecture). 
The following components are needed: 

* `filesystem`, 
* `system`, 
* `thread` and 
* `program_options` 
  * if you plan to build the apps (*optional*)

:information_source: eventually, it might be needed to add the CMake variable `BOOST_LIBRARY_dir` and set it to the respective library.

##### DCMTK

For Windows:

To compile DCMTK with `/MD` flags (standard for all other libs), you need to patch the CMAKE options of DCMTK (`PathToDCMTK\CMake\dcmtkPrepare.cmake`), either by replacing `"/MT"` with `"/MD"` or by explicitly replacing lines 135 to 171 with the following lines:

```
IF(DCMTK_OVERWRITE_WIN32_COMPILER_FLAGS AND NOT BUILD_SHARED_LIBS)

  
	# settings for Microsoft Visual Studio
  
	IF(CMAKE_GENERATOR MATCHES "Visual Studio .*")
    
		# get Visual Studio Version
    
		STRING(REGEX REPLACE "Visual Studio ([0-9]+).*" "\\1" VS_VERSION "${CMAKE_GENERATOR}")
    
		# these settings never change even for C or C++
    
		SET(CMAKE_C_FLAGS_DEBUG "/MDd /Z7 /Od")
    
		SET(CMAKE_C_FLAGS_RELEASE "/DNDEBUG /MD /O2")
    
		SET(CMAKE_C_FLAGS_MINSIZEREL "/DNDEBUG /MD /O2")
    
		SET(CMAKE_C_FLAGS_RELWITHDEBINFO "/DNDEBUG /MDd /Z7 /Od")

    
		SET(CMAKE_CXX_FLAGS_DEBUG "/MDd /Z7 /Od")
    
		SET(CMAKE_CXX_FLAGS_RELEASE "/DNDEBUG /MD /O2")
    
		SET(CMAKE_CXX_FLAGS_MINSIZEREL "/DNDEBUG /MD /O2")
    
		SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/DNDEBUG /MDd /Z7 /Od")
    
		# specific settings for the various Visual Studio versions
    
		IF(VS_VERSION EQUAL 6)
      
			SET(CMAKE_C_FLAGS "/nologo /W3 /GX /Gy /YX")
      
			SET(CMAKE_CXX_FLAGS "/nologo /W3 /GX /Gy /YX /Zm500") # /Zm500 increments heap size which is needed on some system to compile templates in dcmimgle
    
		ENDIF(VS_VERSION EQUAL 6)
    
		IF(VS_VERSION EQUAL 7)
      
			SET(CMAKE_C_FLAGS "/nologo /W3 /Gy")
      
			SET(CMAKE_CXX_FLAGS "/nologo /W3 /Gy")
    
		ENDIF(VS_VERSION EQUAL 7)
    
		IF(VS_VERSION GREATER 7)
      
			SET(CMAKE_C_FLAGS "/nologo /W3 /Gy /EHsc")
      
			SET(CMAKE_CXX_FLAGS "/nologo /W3 /Gy /EHsc")
    
		ENDIF(VS_VERSION GREATER 7)
  
	ENDIF(CMAKE_GENERATOR MATCHES "Visual Studio .*")


ENDIF(DCMTK_OVERWRITE_WIN32_COMPILER_FLAGS AND NOT BUILD_SHARED_LIBS)
```

Then build DCMTK. `BUILD_APPS` can be switched off.

##### ITK

Build ITK with default options.
:warning: ensure that compiler enables C++11 features by setting `CMAKE_CXX_STANDARD=11` (default for supported compilers)

:warning: Only use one ITK version consistently throughout all libraries and RTToolbox! Otherwise, linker errors will occur.

##### MatchPoint

Build MatchPoint with default options.
:warning: ensure that compiler enables C++11 features by setting `CMAKE_CXX_STANDARD=11` (default for supported compilers)

:warning: Only use one ITK version consistently throughout all libraries and RTToolbox! Otherwise, linker errors will occur.


### Building RT-Toolbox

* Configure with CMake
* Set `BOOST_INCLUDE_DIR` and `BOOST_DIR` to the main boost directory (where `boost_build.jam` is located). Eventually, you have to set `BOOST_LIBRARYDIR`
* Select all packages you like to build (Parameters `BUILD_*` ; e.g. `BUILD_IO_Dicom`). 
  * `BUILD_IO_Dicom`: Reading and writing of DICOM-RT files
  * `BUILD_IO_HELAX`: Reading of Helax DICOM files
  * `BUILD_IO_ITK`: Generic reading/writing with ITK
  * `BUILD_Interpolation`: Dose Interpolation
  * `BUILD_InterpolationMatchPointTransformation`: Dose Interpolation with Match Point registration support.
  * `BUILD_Masks`: Voxelization support
  * `BUILD_Masks_Legacy`: Legacy voxelization support. 
  * `BUILD_Models`: Calculation of dosimetrical models like TCP, NTCP etc.
  * `BUILD_Apps`: To build the RTTB command line apps (five available)
    * `BioModelCalc`: calculate the radiobiological effect based on dose
	* `DoseAcc`: Do dose accumulation
	* `DoseMap`: Do dose mapping
	* `DoseTool`: Compute Dose statistics and DVH
	* `VoxelizerTool`: Voxelize an RTSTRUCT file
	
Some modules of RT-Toolbox are mandatory (e.g. `RTTBCore`) and build automatically.
		
:information_source: enabling `BUILD_All_Modules` builds all modules (except Apps, Testing modules and Masks_Legacy).

Set the third party library paths.

Finally, Generate the compilation files for your environment and built it.

### Examples 

Some examples can be found in ´testing/examples´:

* `RTBioModelExampleTest`: Computation of Biological model indices (TCP/NTCP) from a given DVH
* `RTDoseIndexTest`: Computation of dose indices (Conformation Number, Conformal Index, Conformity index) from a given DVH
* `RTDoseStatisticsDicomTest`: Computation of dose statistics (max dose/mean dose/min dose/Dx/Vx) based on dose data for a specified structure
* `RTDVHTest`: Computation of statistics (max value/mean value/min value/Dx/Vx) based on a DVH

Other examples include:

* `DVHCalculatorTest` (`testing/core`): Computation of a DVH from dose and structure
* `VoxelizationValidationTest` (`testing/validation`): Computation of a voxelization
* `ITKDoseAccessorConverterTest`: (`testing/io/itk`): Saving image RTToolbox image data as an ITK file

## Running the tests

[CTest](https://cmake.org/Wiki/CMake/Testing_With_CTest) is used as testing framework. See their documentation for general testing questions.

:information_source: The used testing library Litmus is build automatically. 

:warning: currently, you have access to testing data only with ssh. That means that a [phabricator](https://phabricator.mitk.org/) account and access to `RTTB-data` repository is mandatory. Please contact rttb(at)dkfz.de for further information.

Enabling testing is done as follows:

* Enable `BUILD_TESTING`
* Configure with CMake
* Enable tests of interest
* Generate CMake configuration
* Run tests (build `RUN_TESTS` project or call `ctest` in commandline) to ensure that everything is correct.

:information_source: `BUILD_Tester_All` builds all test modules.

## Contributing

Please add a github issue and send a pull request if you want to contribute.

## Versioning

We use the Ubuntu Release versioning scheme. v2017.02 was released in February 2017. We aim at releasing stable versions once a year. For the versions available, see the [tags on this repository](https://github.com/MIC-DKFZ/RTTB/tags). 

## Authors

See the list of [contributors](https://github.com/MIC-DKFZ/RTTB/contributors) who participated in this project.

## License

This project is licensed under the BSD License - see the [LICENSE](LICENSE) file for details

## Contact

Software Development for Integrated Diagnostics and Therapy (SIDT),
German Cancer Research Center (DKFZ), Heidelberg, Germany.

Web:    https://www.dkfz-heidelberg.de/en/mic/research/SIDT/sidt_projects.html
E-mail: rttb(at)dkfz.de

## Acknowledgments

* **Billie Thompson** - *Template of the readme* - [PurpleBooth](https://github.com/PurpleBooth)
