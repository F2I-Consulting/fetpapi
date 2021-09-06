# Included in ../CmakeLists.txt if csharp build is selected.

find_package(SWIG 3.0 REQUIRED)

set(SWIG_LINKED_TO_RELEASE ON CACHE BOOL "Is your SWIG generated library linked to the release or debug version of FesapiCpp ?")

message("Generating SWIG C# files...")

# Cleaning
execute_process(COMMAND powershell "Remove-Item ${CMAKE_SOURCE_DIR}/cs/src/* -recurse -exclude .gitignore")

# The name of the library is different on Windows because it includes the version
if (WIN32)
	if (SWIG_LINKED_TO_RELEASE)
		set (ASSEMBLY_NAME ${PROJECT_NAME}${CMAKE_RELEASE_POSTFIX}.${Fetpapi_VERSION})
	else (SWIG_LINKED_TO_RELEASE)
		set (ASSEMBLY_NAME ${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}.${Fetpapi_VERSION})
	endif (SWIG_LINKED_TO_RELEASE)
else (WIN32)
	if (SWIG_LINKED_TO_RELEASE)
		set (ASSEMBLY_NAME ${PROJECT_NAME}${CMAKE_RELEASE_POSTFIX})
	else (SWIG_LINKED_TO_RELEASE)
		set (ASSEMBLY_NAME ${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX})
	endif (SWIG_LINKED_TO_RELEASE)
endif (WIN32)

# SWIG execution
set (EXECUTE_COMMAND "${SWIG_EXECUTABLE}")
list (APPEND EXECUTE_COMMAND -v)
list (APPEND EXECUTE_COMMAND -c++)
list (APPEND EXECUTE_COMMAND -csharp)
list (APPEND EXECUTE_COMMAND -dllimport)
list (APPEND EXECUTE_COMMAND ${ASSEMBLY_NAME}.dll)
list (APPEND EXECUTE_COMMAND -namespace)
list (APPEND EXECUTE_COMMAND F2iConsulting.Fetpapi)
list (APPEND EXECUTE_COMMAND -o)
list (APPEND EXECUTE_COMMAND ${CMAKE_SOURCE_DIR}/swig/swigGeneratedCsWrapper.cpp)
list (APPEND EXECUTE_COMMAND -outdir)
list (APPEND EXECUTE_COMMAND ${CMAKE_SOURCE_DIR}/cs/src)
list (APPEND EXECUTE_COMMAND ${CMAKE_SOURCE_DIR}/swig/swigEtp1_2Include.i)
if (WIN32)
        execute_process(COMMAND powershell ${EXECUTE_COMMAND})
elseif (UNIX)
        execute_process(COMMAND ${EXECUTE_COMMAND})
endif (WIN32)
message("SWIG C# files have been generated.")

#Add the swig cpp file to the FesapiCpp
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/swig/swigGeneratedCsWrapper.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/swig/swigGeneratedCsWrapper.h)

# Create the Visual Studio project from a template file
set (CS_LIBRARY_NAME FetpapiCs)

#file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/cs/src" DOS_STYLE_SOURCE_DIR)
configure_file(${CMAKE_SOURCE_DIR}/cmake/fetpapiCs.csproj.template ${CMAKE_SOURCE_DIR}/cs/fetpapiCs.csproj) # Overwrite if different
configure_file(${CMAKE_SOURCE_DIR}/cmake/AssemblyInfo.cs ${CMAKE_SOURCE_DIR}/cs/Properties/AssemblyInfo.cs) # Overwrite if different
