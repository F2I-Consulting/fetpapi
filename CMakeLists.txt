cmake_minimum_required(VERSION 3.5)

project(Fetpapi)

set (LIBRARY_NAME Fetpapi)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

add_library (${LIBRARY_NAME} SHARED "")

# ============================================================================
# checking for required dependencies
# ============================================================================

find_package (Threads)

# avro DEPENDENCY
set (AVRO_INCLUDE_DIR AVRO_INCLUDE_DIR-NOTFOUND CACHE PATH "Path to the directory which contains the avro header files")
if (NOT IS_DIRECTORY ${AVRO_INCLUDE_DIR})
	message(WARNING "The Avro include dir (AVRO_INCLUDE_DIR variable) does not look to be a valid directory. Please modify it.")
endif ()
set (AVRO_LIBRARY_RELEASE AVRO_LIBRARY_RELEASE-NOTFOUND CACHE FILEPATH "Path to the file which contains the avro library release")
if (NOT EXISTS ${AVRO_LIBRARY_RELEASE})
	message(WARNING "The avro library (AVRO_LIBRARY_RELEASE variable) does not look to be a valid file. Please modify it.")
endif ()
set (AVRO_LIBRARY_DEBUG AVRO_LIBRARY_DEBUG-NOTFOUND CACHE FILEPATH "Path to the file which contains the AVRO library DEBUG")
if (NOT EXISTS ${AVRO_LIBRARY_DEBUG})
	message(WARNING "The AVRO library (AVRO_LIBRARY_DEBUG variable) does not look to be a valid file. Please modify it.")
endif ()

# Boost DEPENDENCY
find_package(Boost 1.66.0 REQUIRED system)
if (WIN32 AND (Boost_VERSION_MAJOR EQUAL 1) AND (Boost_VERSION_MINOR LESS 74) AND (Boost_VERSION_MINOR GREATER 71))
		message(WARNING "You may experience min/max issue with this boost version : See https://github.com/boostorg/beast/issues/1980")
endif ()

if (WIN32)
	target_link_libraries (${LIBRARY_NAME} PRIVATE bcrypt.lib)
endif (WIN32)

target_compile_definitions(${LIBRARY_NAME} PRIVATE BOOST_ALL_NO_LIB)
target_link_libraries (${LIBRARY_NAME} PRIVATE ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
if (WIN32)
	if (EXISTS ${AVRO_LIBRARY_RELEASE} AND EXISTS ${AVRO_LIBRARY_DEBUG})
		target_link_libraries (${LIBRARY_NAME} PRIVATE optimized ${AVRO_LIBRARY_RELEASE} debug ${AVRO_LIBRARY_DEBUG})
	elseif (EXISTS ${AVRO_LIBRARY_RELEASE})
		target_link_libraries (${LIBRARY_NAME} PRIVATE ${AVRO_LIBRARY_RELEASE})
	elseif (EXISTS ${AVRO_LIBRARY_DEBUG})
		target_link_libraries (${LIBRARY_NAME} PRIVATE ${AVRO_LIBRARY_DEBUG})
	endif ()
else (WIN32)
	target_link_libraries (${LIBRARY_NAME} PRIVATE ${AVRO_LIBRARY_RELEASE})
endif (WIN32)

if (WITH_ETP_SSL)
	if (NOT TARGET OpenSSL::SSL OR NOT TARGET OpenSSL::Crypto)
		find_package(OpenSSL REQUIRED)
	endif()
	target_link_libraries(${LIBRARY_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif ()

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
target_sources(${LIBRARY_NAME} PRIVATE ${ALL_SOURCES_AND_HEADERS})

target_include_directories(${LIBRARY_NAME} PRIVATE ${AVRO_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
target_include_directories(${LIBRARY_NAME} INTERFACE
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
endif (WIN32)

# ============================================================================
# Install Fetpapi library
# ============================================================================
 
IF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install CACHE PATH "${LIBRARY_NAME} install prefix" FORCE)
ENDIF (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# required to define CMAKE_INSTALL_BINDIR, CMAKE_INSTALL_LIBDIR and CMAKE_INSTALL_INCLUDEDIR
include(GNUInstallDirs) 
 
install(TARGETS ${LIBRARY_NAME}
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
	
install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/fetpapi
	FILES_MATCHING PATTERN "*.h"
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/FetpapiConfig.cmake
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Fetpapi")