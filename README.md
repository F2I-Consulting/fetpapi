
# Prepare your build environment
 - Create a folder called fesapiEnv.
 - In this folder create the three following folders
	 - build
	 - fetpapi (Git clone this repository into this folder "fetpapi". You should then have a path fesapiEnv/fetpapi/src)
	 - dependencies 
  - The following compilers are known to work (used in CI)
    - gcc from version 4.8
	- visual studio from version 2017
# Prepare the dependencies
Download (build and install if necessary) third party libraries:
- BOOST : All versions from version 1.66 should be ok but you may experience some [min/max build issues](https://github.com/boostorg/beast/issues/1980) using version 1.72 or 1.73.
- AVRO : https://avro.apache.org/releases.html#Download (starting from version 1.9.0, build it with the above boost library)
- (OPTIONALLY) OpenSSL : version 1.1 is known to work.
- (OPTIONALLY) [FESAPI](https://github.com/F2I-Consulting/fesapi/releases) : All versions from version 2.3.0.0 should be ok.

We advise you to install these third party libraries respectively into
- fesapiEnv/dependencies/boost-particularVersion
- fesapiEnv/dependencies/avro-particularVersion
# Configure the build
FETPAPI uses cmake as its build tool. A 3.12 version or later of cmake is required https://cmake.org/download/. We also recommend using cmake-gui (already included in the bin folder of the binary releases of cmake) which provides a graphical user interface on top of cmake. If you want to use cmake in command line, you would find example in [Azure Pipeline file](./azure-pipelines.yml). Follow the below instructions :

- yourPath/fesapiEnv/fetpapi defines where is the source code folder
- yourPath/fesapiEnv/build/theNameYouWant defines where to build the binaries
- Click on "Configure" button and select your favorite compiler : it will raise several errors.
- give real path and files to the following cmake variables:
	- BOOST
		- Boost_INCLUDE_DIR : the directory where you can find the directory named "boost" which contain all BOOST headers
	- AVRO
		- AVRO_INCLUDE_DIR : where the "avro" directory containing all AVRO headers is located
		- (ONLY FOR WINDOWS) AVRO_LIBRARY_DEBUG : Optional, only used by Visual studio Debug configuration, the Avro debug library you want to link to.
		- AVRO_LIBRARY_RELEASE : the AVRO system library you want to link to.
- Click again on "Configure" button. You should no more have errors so you can now click on "Generate" button.
- You can now build your solution with your favorite compiler (and linker) using the generated solution in yourPath/fesapiEnv/build/theNameYouWant
- OPTIONALLY, you can also set the variables WITH_DOTNET_WRAPPING, WITH_PYTHON_WRAPPING to true if you want to also generate wrappers on top of FETPAPI for these two other programming languages. Don't forget to click again on "Configure" button once you changed the value of these two variables.
	- You will then have to also provide the path to the SWIG (version 3 as a mininum version) executable http://swig.org/download.html in the SWIG_EXECUTABLE variable (and click again on "Configure" button)
	- you will find the wrappers in fetpapi/cs/src (fetpapi/cs also contains a VS2015 project for the wrappers) or fetpapi/python/src
- OPTIONALLY, for SSL support, please enable the WITH_ETP_SSL variable and set the following variables :
	- OPENSSL_INCLUDE_DIR : the OpenSSL include directory
	- LIB_EAY_RELEASE : the OpenSSL crypto library you want to link with.
	- SSL_EAY_RELEASE : the OpenSSL ssl library you want to link with. 
- OPTIONALLY, for FESAPI support (see [here](https://github.com/F2I-Consulting/fesapi) for documentation on how to build fesapi), please enable the WITH_FESAPI variable and set the following variables :
	- FESAPI_INCLUDE_DIR : the directory where the FESAPI headers are located (generally the include subdirectory of the fesapi installation directory). 
	- (ONLY FOR WINDOWS) FESAPI_LIBRARY_DEBUG : Optional, only used by Visual studio Debug configuration, the FESAPI debug library you want to link to.
	- FESAPI_LIBRARY_RELEASE : the FESAPI library you want to link to.

Remark : you can choose where FETPAPI will be installed (using "make install" on Linux or by generating the "INSTALL" project on Visual Studio) by setting the cmake variable called CMAKE_INSTALL_PREFIX
# How to start
As a first way to start, we advise people to look at client ([here](https://github.com/F2I-Consulting/fetpapiClient)) and server ([here](https://github.com/F2I-Consulting/fetpapiServer)) examples.
# Credits
This software was developed with :
- the date library from [Howard Hinnant](https://github.com/HowardHinnant/date)

This software was tested with :
- [Azure Pipelines](https://azure.microsoft.com/en-us/services/devops/pipelines/)
