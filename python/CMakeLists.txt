# Included in ../CmakeLists.txt if Python build is selected.

find_package(SWIG 3.0 REQUIRED)

set(SWIG_LINKED_TO_RELEASE ON CACHE BOOL "Is your SWIG generated library linked to the release or debug version of FesapiCpp ?")

message("Generating SWIG Python files...")
file (GLOB TO_DELETE ${CMAKE_SOURCE_DIR}/python/src/*.pyd)
if (TO_DELETE)
	file (REMOVE ${TO_DELETE})
endif (TO_DELETE)
file (REMOVE ${CMAKE_SOURCE_DIR}/python/src/fesapi.py)
file (REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/python/src/__pycache__)

# The name of the library is different on Windows because it includes the version
if (WIN32)
	if (SWIG_LINKED_TO_RELEASE)
		set (ASSEMBLY_NAME ${LIBRARY_NAME}${CMAKE_RELEASE_POSTFIX}.${Fesapi_VERSION})
	else (SWIG_LINKED_TO_RELEASE)
		set (ASSEMBLY_NAME ${LIBRARY_NAME}${CMAKE_DEBUG_POSTFIX}.${Fesapi_VERSION})
	endif (SWIG_LINKED_TO_RELEASE)
else (WIN32)
	if (SWIG_LINKED_TO_RELEASE)
		set (ASSEMBLY_NAME ${LIBRARY_NAME}${CMAKE_RELEASE_POSTFIX})
	else (SWIG_LINKED_TO_RELEASE)
		set (ASSEMBLY_NAME ${LIBRARY_NAME}${CMAKE_DEBUG_POSTFIX})
	endif (SWIG_LINKED_TO_RELEASE)
endif (WIN32)
get_filename_component(AVRO_LIBRARY_DIR_RELEASE ${AVRO_LIBRARY_RELEASE} DIRECTORY)
get_filename_component(AVRO_LIBRARY_RELEASE_WLE ${AVRO_LIBRARY_RELEASE} NAME_WLE)
configure_file(${CMAKE_SOURCE_DIR}/cmake/setup_etp.py.in ${CMAKE_SOURCE_DIR}/swig/setup.py) # Overwrite if different

# SWIG execution
if (WIN32)
	set ( EXECUTE_COMMAND "${SWIG_EXECUTABLE} -v -c++ -python -builtin -py3 -o ${CMAKE_SOURCE_DIR}/swig/swigGeneratedPythonWrapper.cpp -outdir ${CMAKE_SOURCE_DIR}/python/src ${CMAKE_SOURCE_DIR}/swig/swigEtp1_2Include.i")
	execute_process(COMMAND powershell ${EXECUTE_COMMAND})
elseif (UNIX)
	set ( EXECUTE_COMMAND ${SWIG_EXECUTABLE} -v -c++ -python -builtin -py3 -o ${CMAKE_SOURCE_DIR}/swig/swigGeneratedPythonWrapper.cpp -outdir ${CMAKE_SOURCE_DIR}/python/src ${CMAKE_SOURCE_DIR}/swig/swigEtp1_2Include.i)
	execute_process(COMMAND ${EXECUTE_COMMAND})
endif (WIN32)
message("SWIG Python files have been generated.")