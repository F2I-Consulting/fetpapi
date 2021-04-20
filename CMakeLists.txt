cmake_minimum_required(VERSION 3.5)

project(Fetpapi)

set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "DEBUG_POSTFIX property is initialized when the target is created to the value of this variable except for executable targets")

# Allow to have all executables generated in the same binary directory. Otherwise there would be in a directory different for each add_subdirectory cmake commande.
set (FETPAPI_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# version mechanism
set (Fetpapi_VERSION_MAJOR 0)
set (Fetpapi_VERSION_MINOR 1)
set (Fetpapi_VERSION_PATCH 0)
set (Fetpapi_VERSION_TWEAK 0)
set (Fetpapi_VERSION ${Fetpapi_VERSION_MAJOR}.${Fetpapi_VERSION_MINOR}.${Fetpapi_VERSION_PATCH}.${Fetpapi_VERSION_TWEAK})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

IF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install CACHE PATH "${PROJECT_NAME} install prefix" FORCE)
ENDIF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# required to define CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_LIBDIR and CMAKE_INSTALL_INCLUDEDIR
include(GNUInstallDirs)

# ============================================================================
# Lib type (shared/static)
# ============================================================================

set (BUILD_SHARED_LIBS ON CACHE BOOL "Build shared lib")
if (BUILD_SHARED_LIBS)
	set (LIB_TYPE SHARED)
else (BUILD_SHARED_LIBS)
	set (LIB_TYPE STATIC)
endif (BUILD_SHARED_LIBS)

add_library (${PROJECT_NAME} ${LIB_TYPE} "") 

# Define the compile options according to the compiler
target_compile_options(${PROJECT_NAME}	PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/bigobj>
	$<$<CXX_COMPILER_ID:MSVC>:/MP>
	$<$<CXX_COMPILER_ID:MSVC>:/W4>
	$<$<CXX_COMPILER_ID:GNU>:-Wall>
	$<$<CXX_COMPILER_ID:GNU>:-Wextra>
	$<$<CXX_COMPILER_ID:GNU>:-Wcast-qual>
	$<$<CXX_COMPILER_ID:GNU>:-pedantic>
	$<$<CXX_COMPILER_ID:CLANG>:-Weverything>
)

if (WIN32)
	if (NOT BUILD_SHARED_LIBS)
		target_compile_definitions(${PROJECT_NAME} PRIVATE FETPAPI_STATIC)
	endif ()
endif (WIN32)

# ============================================================================
# some useful variables
# ============================================================================

set (WITH_FESAPI OFF CACHE BOOL "Build Fetpapi with Fesapi support.")
set (WITH_ETP_SSL OFF CACHE BOOL "Build Fesapi with ETP SSL support.")

# ============================================================================
# checking for required dependencies
# ============================================================================

find_package (Threads)

# avro DEPENDENCY
set (AVRO_INCLUDE_DIR AVRO_INCLUDE_DIR-NOTFOUND CACHE PATH "Path to the directory which contains the avro header files")
if (NOT IS_DIRECTORY ${AVRO_INCLUDE_DIR})
	message(SEND_ERROR "The Avro include dir (AVRO_INCLUDE_DIR variable) does not look to be a valid directory. Please modify it.")
endif ()
set (AVRO_LIBRARY_RELEASE AVRO_LIBRARY_RELEASE-NOTFOUND CACHE FILEPATH "Path to the file which contains the avro library release")
if (NOT EXISTS ${AVRO_LIBRARY_RELEASE})
	message(WARNING "The avro library (AVRO_LIBRARY_RELEASE variable) does not look to be a valid file. Please modify it.")
endif ()
set (AVRO_LIBRARY_DEBUG AVRO_LIBRARY_DEBUG-NOTFOUND CACHE FILEPATH "Path to the file which contains the AVRO library DEBUG")
if (NOT EXISTS ${AVRO_LIBRARY_DEBUG})
	message(WARNING "The AVRO library (AVRO_LIBRARY_DEBUG variable) does not look to be a valid file. Please modify it.")
endif ()

if (WITH_FESAPI)
	set (FESAPI_INCLUDE_DIR FESAPI_INCLUDE_DIR-NOTFOUND CACHE PATH "Path to the directory which contains the FESAPI header files")
	if (NOT IS_DIRECTORY ${FESAPI_INCLUDE_DIR})
		message(SEND_ERROR "The FESAPI include dir (FESAPI_INCLUDE_DIR variable) does not look to be a valid directory. Please modify it.")
	endif ()
	set (FESAPI_LIBRARY_RELEASE FESAPI_LIBRARY_RELEASE-NOTFOUND CACHE FILEPATH "Path to the file which contains the FESAPI library release")
	if (NOT EXISTS ${FESAPI_LIBRARY_RELEASE})
		message(WARNING "The FESAPI library (FESAPI_LIBRARY_RELEASE variable) does not look to be a valid file. Please modify it.")
	endif ()
	set (FESAPI_LIBRARY_DEBUG FESAPI_LIBRARY_DEBUG-NOTFOUND CACHE FILEPATH "Path to the file which contains the FESAPI library DEBUG")
	if (NOT EXISTS ${FESAPI_LIBRARY_DEBUG})
		message(WARNING "The FESAPI library (FESAPI_LIBRARY_DEBUG variable) does not look to be a valid file. Please modify it.")
	endif ()
	set (FESAPI_JAR FESAPI_JAR-NOTFOUND CACHE FILEPATH "Path to the jar of FESAPI")
	if (NOT EXISTS ${FESAPI_JAR})
		message(SEND_ERROR "The FESAPI library (FESAPI_JAR variable) does not look to be a valid file. Please modify it.")
	endif ()
endif (WITH_FESAPI)


# Boost DEPENDENCY
find_package(Boost 1.66.0 REQUIRED system)
if (WIN32 AND (Boost_VERSION_MAJOR EQUAL 1) AND (Boost_VERSION_MINOR LESS 74) AND (Boost_VERSION_MINOR GREATER 71))
		message(WARNING "You may experience min/max issue with this boost version : See https://github.com/boostorg/beast/issues/1980")
endif ()

target_compile_definitions(${PROJECT_NAME} PRIVATE BOOST_ALL_NO_LIB)

target_link_libraries (${PROJECT_NAME} PRIVATE ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
if (WIN32)
	target_link_libraries (${PROJECT_NAME} PRIVATE bcrypt.lib)
	
	if (EXISTS ${AVRO_LIBRARY_RELEASE} AND EXISTS ${AVRO_LIBRARY_DEBUG})
		target_link_libraries (${PROJECT_NAME} PRIVATE optimized ${AVRO_LIBRARY_RELEASE} debug ${AVRO_LIBRARY_DEBUG})
	elseif (EXISTS ${AVRO_LIBRARY_RELEASE})
		target_link_libraries (${PROJECT_NAME} PRIVATE ${AVRO_LIBRARY_RELEASE})
	elseif (EXISTS ${AVRO_LIBRARY_DEBUG})
		target_link_libraries (${PROJECT_NAME} PRIVATE ${AVRO_LIBRARY_DEBUG})
	endif ()
	
	set_target_properties(${PROJECT_NAME} PROPERTIES
		PDB_NAME ${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}.${Fetpapi_VERSION}
		LINK_FLAGS "/INCREMENTAL:NO /OPT:REF"
		RUNTIME_OUTPUT_DIRECTORY ${FETPAPI_BINARY_DIR}
		SUFFIX .${Fetpapi_VERSION}.dll
		IMPORT_SUFFIX .${Fetpapi_VERSION}.lib
		VERSION ${Fetpapi_VERSION_MAJOR}.${Fetpapi_VERSION_MINOR}
	)
else (WIN32)
	target_link_libraries (${PROJECT_NAME} PRIVATE ${AVRO_LIBRARY_RELEASE})
	
	set_target_properties(${PROJECT_NAME} PROPERTIES
		COMPILE_FLAGS "-fPIC"
		VERSION ${Fetpapi_VERSION}
		SOVERSION ${Fetpapi_VERSION_MAJOR}.${Fetpapi_VERSION_MINOR})
endif (WIN32)

if (WITH_ETP_SSL)
	target_compile_definitions(${PROJECT_NAME} PRIVATE WITH_ETP_SSL)
	if (NOT TARGET OpenSSL::SSL OR NOT TARGET OpenSSL::Crypto)
		find_package(OpenSSL REQUIRED)
	endif()
	target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif ()

if (WITH_FESAPI)
	if (WIN32)
		if (EXISTS ${FESAPI_LIBRARY_RELEASE} AND EXISTS ${FESAPI_LIBRARY_DEBUG})
			target_link_libraries (${PROJECT_NAME} PRIVATE optimized ${FESAPI_LIBRARY_RELEASE} debug ${FESAPI_LIBRARY_DEBUG})
		elseif (EXISTS ${FESAPI_LIBRARY_RELEASE})
			target_link_libraries (${PROJECT_NAME} PRIVATE ${FESAPI_LIBRARY_RELEASE})
		elseif (EXISTS ${FESAPI_LIBRARY_DEBUG})
			target_link_libraries (${PROJECT_NAME} PRIVATE ${FESAPI_LIBRARY_DEBUG})
		endif ()
	else (WIN32)
		target_link_libraries (${PROJECT_NAME} PRIVATE ${FESAPI_LIBRARY_RELEASE})
	endif (WIN32)
endif (WITH_FESAPI)

# ============================================================================
# SWIG wrapping
# ============================================================================

set (WITH_JAVA_WRAPPING OFF CACHE BOOL "Compile the swig cpp generated file for Java wrappers.")
set (WITH_DOTNET_WRAPPING OFF CACHE BOOL "Compile the swig cpp generated file for .NET wrappers.")
set (WITH_PYTHON_WRAPPING OFF CACHE BOOL "Compile the swig cpp generated file for Python wrappers.")

if (WITH_JAVA_WRAPPING)
	include(${CMAKE_SOURCE_DIR}/java/CMakeLists.txt)
endif (WITH_JAVA_WRAPPING)

if (WITH_DOTNET_WRAPPING)
	include(${CMAKE_SOURCE_DIR}/cs/CMakeLists.txt)
endif (WITH_DOTNET_WRAPPING)

if (WITH_PYTHON_WRAPPING)
	include(${CMAKE_SOURCE_DIR}/python/CMakeLists.txt)
endif (WITH_PYTHON_WRAPPING)

# ============================================================================
# Getting source files
# ============================================================================

# Namespaces definitions
set (FETPAPI_ETP_NS etp CACHE STRING "Namespace for etp objects")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/nsDefinitions.h ${CMAKE_CURRENT_SOURCE_DIR}/src/nsDefinitions.h) # Overwrite if different

file (GLOB FETPAPI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/etp/*.cpp)
file (GLOB FETPAPI_PROTOCOL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/etp/ProtocolHandlers/*.cpp)
file (GLOB FETPAPI_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/etp/*.h)
file (GLOB FETPAPI_PROTOCOL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/etp/ProtocolHandlers/*.h)
file (GLOB FETPAPI_TOOLS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/*.cpp)
file (GLOB FETPAPI_TOOLS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/*.h)
set (ALL_SOURCES_AND_HEADERS
	${ALL_SOURCES_AND_HEADERS}
	${FETPAPI_SOURCES}
	${FETPAPI_PROTOCOL_SOURCES}
	${FETPAPI_HEADERS}	
	${FETPAPI_PROTOCOL_HEADERS}	
	${FETPAPI_TOOLS_HEADERS}
	${FETPAPI_TOOLS_SOURCES}
)

if (WITH_ETP_SSL)
	file (GLOB FETPAPI_SSL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/etp/ssl/*.cpp)
	file (GLOB FETPAPI_SSL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/etp/ssl/*.h)
	
	set (ALL_SOURCES_AND_HEADERS
		${ALL_SOURCES_AND_HEADERS}
		${FETPAPI_SSL_SOURCES}
		${FETPAPI_SSL_HEADERS}
	)
endif (WITH_ETP_SSL)

if (WITH_FESAPI)
	file (GLOB FETPAPI_FESAPI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/etp/fesapi/*.cpp)
	file (GLOB FETPAPI_FESAPI_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/etp/fesapi/*.h)
	set (ALL_SOURCES_AND_HEADERS
		${ALL_SOURCES_AND_HEADERS}
		${FETPAPI_FESAPI_SOURCES}
		${FETPAPI_FESAPI_HEADERS}
	)
	target_include_directories(${PROJECT_NAME} PRIVATE ${HDF5_C_INCLUDE_DIR} ${FESAPI_INCLUDE_DIR})
endif (WITH_FESAPI)

target_sources(${PROJECT_NAME} PRIVATE ${ALL_SOURCES_AND_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${AVRO_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (WITH_ETP_SSL)
	target_include_directories(${PROJECT_NAME} PRIVATE ${OPENSSL_INCLUDE_DIR})
endif ()

target_include_directories(${PROJECT_NAME} INTERFACE
	$<INSTALL_INTERFACE:include>
)

# organizing sources and headers in the Visual Studio Project
if (WIN32)
	source_group ("tools" FILES ${FETPAPI_TOOLS_SOURCES} ${FETPAPI_TOOLS_HEADERS})
	
	set (ETP_PREFIX "etp")
	source_group ("${ETP_PREFIX}" FILES ${FETPAPI_SOURCES} ${FETPAPI_HEADERS})
	source_group ("${ETP_PREFIX}\\ProtocolHandlers" FILES ${FETPAPI_PROTOCOL_SOURCES} ${FETPAPI_PROTOCOL_HEADERS})
	
	if (WITH_ETP_SSL)
		source_group ("${ETP_PREFIX}\\ssl" FILES ${FETPAPI_SSL_SOURCES} ${FETPAPI_SSL_HEADERS})
	endif ()
	
	if (WITH_FESAPI)
		source_group ("${ETP_PREFIX}\\fesapi" FILES ${FETPAPI_FESAPI_SOURCES} ${FETPAPI_FESAPI_HEADERS})
	endif (WITH_FESAPI)
endif (WIN32)

# ============================================================================
# Install Fetpapi library
# ============================================================================
  
install(TARGETS ${PROJECT_NAME}
  EXPORT Fetpapi-export
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT Fetpapi-export
  FILE
    FetpapiTargets.cmake
  NAMESPACE
    Fetpapi::
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/Fetpapi
)

INSTALL (
	FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/nsDefinitions.h
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fetpapi
	COMPONENT fetpapi_headers
)

INSTALL (
	FILES ${FETPAPI_HEADERS}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fetpapi/etp
	COMPONENT fetpapi_headers
)

INSTALL (
	FILES ${FETPAPI_PROTOCOL_HEADERS}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fetpapi/etp/ProtocolHandlers
	COMPONENT fetpapi_headers
)

INSTALL (
	FILES ${FETPAPI_TOOLS_HEADERS}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fetpapi/tools
	COMPONENT fetpapi_headers
)

if (WITH_ETP_SSL)
	INSTALL (
		FILES ${FETPAPI_SSL_HEADERS}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fetpapi/etp/ssl
		COMPONENT fetpapi_headers
	)
endif (WITH_ETP_SSL)

if (WITH_FESAPI)
	INSTALL (
		FILES ${FETPAPI_FESAPI_HEADERS}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fetpapi/etp/fesapi
		COMPONENT fetpapi_headers
	)
endif (WITH_FESAPI)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/FetpapiConfig.cmake
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Fetpapi")