
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
* GCC 7.3

Other compilers may work as well, but are not tested.

#### Linking Static/Dynamic library support

Can be changed with advanced option `BUILD_SHARED_LIBS`

:warning: building RTToolbox as dynamic library under Windows and as static library under Linux is an experimental feature.

#### Third party libraries

* [boost](http://www.boost.org ), version 1.64.0 or higher
* [DCMTK](http://dicom.offis.de/dcmtk.php.en ) 3.6.5 or higher
* [ITK](https://itk.org ), version 4.4 or higher (*optional*)
  * for DoseInterpolation support with ITK transformation or ITK File IO support
* [MatchPoint](http://mitk.org/download/thirdparty/MatchPoint_rev1610.tar.gz ), version 0.12 or higher (*optional*)
  * for DoseInterpolation support with MatchPoint registration objects

:information_source: To make sure everything runs smoothly, please make sure that all libraries and the RTToolbox are either compiled with `/MD` or `/MT` flags.

##### Boost

In case you work with Windows, we recommend using the [pre-build versions of boost](https://sourceforge.net/projects/boost/files/boost-binaries/).

If you want to build the library yourself, consider the following:

Build (using the same compiler options as RTToolbox, usually `STATIC LINKING` and `x64` architecture). 
The following components are needed: 

* `filesystem`, 
* `system` and
* `program_options` 
  * if you plan to build the apps (*optional*)

:information_source: eventually, it might be needed to add the CMake variable `BOOST_LIBRARYDIR` and set it to the respective library path of boost.

For Windows:

To build Boost open a command prompt, change to your boost source directory and copy following command(s):
Debug:
`b2 -j12 --with-filesystem --with-system --with-thread --with-program_options --with-date_time --with-atomic --with-chrono toolset=<your toolset identifier> address-model=64 variant=debug threading=multi link=shared define=_BIND_TO_CURRENT_VCLIBS_VERSION`
Release:
`b2 -j12 --with-filesystem --with-system --with-thread --with-program_options --with-date_time --with-atomic --with-chrono toolset=<your toolset identifier> address-model=64 variant=release threading=multi link=shared`

Set the <your toolset identifier> in the commands above accordingly.
If you don´t require `program_options` delete `--with-program_options` from the command before executing it.
##### DCMTK

For Windows:
To compile DCMTK with `/MD` flags (standard for all other libs), you need to set DCMTK_COMPILE_WIN32_MULTITHREADED_DLL to "ON".

`BUILD_APPS` can be switched off.
Then build DCMTK.

For Linux:
install required dependencies (Ubuntu 18.04 and newer): `sudo apt-get install libpng-dev libtiff5-dev libxml2-dev libjpeg8-dev zlib1g-dev libwrap0-dev libssl-dev`
install required dependencies (Ubuntu 17.10 and older): `sudo apt-get install libpng12-dev libtiff5-dev libxml2-dev libjpeg8-dev zlib1g-dev libwrap0-dev libssl-dev`
Enable `BUILD_SHARED_LIBS`. `BUILD_APPS` can be switched off.

##### ITK

Build ITK with default options.
:warning: ensure that compiler enables C++11 features by setting `CMAKE_CXX_STANDARD=11` (default for supported compilers)

:warning: if you use ITK 5, turn on `RTTB_ITK5_SUPPORT`

:warning: Only use one ITK version consistently throughout all libraries and RTToolbox! Otherwise, linker errors will occur.

##### MatchPoint

Configure MatchPoint. Please disable `BUILD_TESTING` before building it.
:warning: ensure that compiler enables C++11 features by setting `CMAKE_CXX_STANDARD=11` (default for supported compilers)

:warning: Only use one ITK version consistently throughout all libraries and RTToolbox! Otherwise, linker errors will occur.

:warning: MatchPoint currently does not support ITK 5

### Building RT-Toolbox

* Configure with CMake
* Set `BOOST_INCLUDE_DIR` to the main boost directory. Eventually set `BOOST_LIBRARYDIR` to the respective path (e.g. `<boost_directory>/lib64-msvc-14.1\` for Visual Studio 2017 and 64-bit)
* Select all packages you like to build (Parameters `BUILD_*` ; e.g. `BUILD_IO_Dicom`). 
  * `BUILD_IO_Dicom`: Reading and writing of DICOM-RT files
  * `BUILD_IO_HELAX`: Reading of Helax DICOM files
  * `BUILD_IO_ITK`: Generic reading/writing with ITK
  * `BUILD_Interpolation`: Dose Interpolation
  * `BUILD_InterpolationMatchPointTransformation`: Dose Interpolation with Match Point registration support.
  * `BUILD_Masks`: Voxelization support
  * `BUILD_Models`: Calculation of dosimetrical models like TCP, NTCP etc.
  * `BUILD_Apps`: To build the RTTB command line apps (five available)
    * `BioModelCalc`: calculate the radiobiological effect based on dose
	* `DoseAcc`: Do dose accumulation
	* `DoseMap`: Do dose mapping
	* `DoseTool`: Compute Dose statistics and DVH
	* `VoxelizerTool`: Voxelize an RTSTRUCT file
	
Some modules of RT-Toolbox are mandatory (e.g. `RTTBCore`) and build automatically.
		
:information_source: enabling `BUILD_All_Modules` builds all modules (except Apps and Testing modules).

:information_source: if you build RTTB with VS dynamic, you must ensure that code that uses RTTB DLLs uses the same STL


Set `DCMTK_DIR` to your dcmtk binary file directory and `DCMTK_SOURCE_DIR` to your dcmtk source directory.

If you want to build RT-Toolbox with ITK and/or MatchPoint set your `ITK_DIR` to your itk binary file directory and/or `MatchPoint_DIR` to your binary matchpoint directory.

All directory entries left empty do not require a manual input.

Finally, Generate the compilation files for your environment and built it.

### Examples 

Some examples can be found in ´testing/examples´:

* `RTBioModelExampleTest`: Computation of Biological model indices (TCP/NTCP) from a given DVH
* `RTDoseStatisticsDicomTest`: Computation of dose statistics (max dose/mean dose/min dose/Dx/Vx) based on dose data for a specified structure
* `RTDVHTest`: Computation of statistics (max value/mean value/min value/Dx/Vx) based on a DVH

Other examples include:

* `DVHCalculatorTest` (`testing/core`): Computation of a DVH from dose and structure
* `VoxelizationValidationTest` (`testing/validation`): Computation of a voxelization
* `ITKDoseAccessorConverterTest`: (`testing/io/itk`): Saving image RTToolbox image data as an ITK file
* `DoseIndex tests`: (`testing/indices`): Computation of different dose indices (e.g. Conformation Number, Conformal Index, Conformity index)

## Running the tests

[CTest](https://cmake.org/Wiki/CMake/Testing_With_CTest) is used as testing framework. See their documentation for general testing questions.

:information_source: The used testing library Litmus is build automatically. 

:warning: currently, you have access to testing data only with ssh. That means that a [phabricator](https://phabricator.mitk.org/) account and access to `RTTB-data` repository is mandatory. Please contact rttb(at)dkfz.de for further information.

Enabling testing is done as follows:

* Enable `BUILD_TESTING`
* Configure with CMake
* Enable tests of interest
* Generate CMake configuration
* Build RT-Toolbox
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
