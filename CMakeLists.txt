if (WIN32)
	cmake_minimum_required(VERSION 3.21)
else ()
	cmake_minimum_required(VERSION 3.12)
endif (WIN32)

project(Fetpapi)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "DEBUG_POSTFIX property is initialized when the target is created to the value of this variable except for executable targets")

# Allow to have all executables generated in the same binary directory. Otherwise there would be in a directory different for each add_subdirectory cmake commande.
set (FETPAPI_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# version mechanism
set (Fetpapi_VERSION_MAJOR 0)
set (Fetpapi_VERSION_MINOR 2)
set (Fetpapi_VERSION_PATCH 0)
set (Fetpapi_VERSION_TWEAK 0)

set (Fetpapi_VERSION ${Fetpapi_VERSION_MAJOR}.${Fetpapi_VERSION_MINOR}.${Fetpapi_VERSION_PATCH}.${Fetpapi_VERSION_TWEAK})

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set (CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/install CACHE PATH "${PROJECT_NAME} install prefix" FORCE)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

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
	
	#FROM https://stackoverflow.com/a/40217291
    macro(get_WIN32_WINNT version)
        if(CMAKE_SYSTEM_VERSION)
            set(ver ${CMAKE_SYSTEM_VERSION})
            string(REGEX MATCH "^([0-9]+).([0-9])" ver ${ver})
            string(REGEX MATCH "^([0-9]+)" verMajor ${ver})
            # Check for Windows 10, b/c we'll need to convert to hex 'A'.
            if("${verMajor}" MATCHES "10")
                set(verMajor "A")
                string(REGEX REPLACE "^([0-9]+)" ${verMajor} ver ${ver})
            endif()
            # Remove all remaining '.' characters.
            string(REPLACE "." "" ver ${ver})
            # Prepend each digit with a zero.
            string(REGEX REPLACE "([0-9A-Z])" "0\\1" ver ${ver})
            set(${version} "0x${ver}")
        endif()
    endmacro()

    get_WIN32_WINNT(ver)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32_WINNT=${ver})
endif (WIN32)

# ============================================================================
# some useful variables
# ============================================================================

set (WITH_FESAPI OFF CACHE BOOL "Build FETPAPI with FESAPI support.")
set (WITH_ETP_SSL ON CACHE BOOL "Build FETPAPI with ETP SSL support.")

# ============================================================================
# checking for required dependencies
# ============================================================================

find_package (Threads)

# Boost DEPENDENCY
find_package(Boost 1.70)
if (NOT Boost_FOUND)
	# Boost system is required for Beast until version 1.70 : https://www.boost.org/doc/libs/1_69_0/libs/beast/doc/html/beast/introduction.html
	find_package(Boost 1.66 REQUIRED system)
endif()
if (WIN32 AND (Boost_VERSION_MAJOR EQUAL 1) AND (Boost_VERSION_MINOR LESS 74) AND (Boost_VERSION_MINOR GREATER 71))
	message(WARNING "You may experience min/max issue with this boost version : See https://github.com/boostorg/beast/issues/1980")
endif ()

# avro DEPENDENCY
find_package(AVRO REQUIRED)

if (WITH_FESAPI)
	find_package(FESAPI REQUIRED)
	if (WITH_JAVA_WRAPPING)
		set (FESAPI_JAR FESAPI_JAR-NOTFOUND CACHE FILEPATH "Path to the jar of FESAPI")
		if (NOT EXISTS ${FESAPI_JAR})
			message(SEND_ERROR "The FESAPI library (FESAPI_JAR variable) does not look to be a valid file. Please modify it.")
		endif ()
	endif (WITH_JAVA_WRAPPING)
endif (WITH_FESAPI)

target_compile_definitions(${PROJECT_NAME} PRIVATE BOOST_ALL_NO_LIB)
target_link_libraries (${PROJECT_NAME} PRIVATE AVRO::AVRO ${CMAKE_THREAD_LIBS_INIT})
if (DEFINED Boost_SYSTEM_LIBRARY)
	target_link_libraries (${PROJECT_NAME} PRIVATE ${Boost_SYSTEM_LIBRARY})
endif()
if (WIN32)
	target_link_libraries (${PROJECT_NAME} PRIVATE bcrypt.lib)

	set_target_properties(${PROJECT_NAME} PROPERTIES
		PDB_NAME ${PROJECT_NAME}.${Fetpapi_VERSION}
		PDB_NAME_DEBUG ${PROJECT_NAME}${CMAKE_DEBUG_POSTFIX}.${Fetpapi_VERSION}
		LINK_FLAGS "/INCREMENTAL:NO /OPT:REF"
		RUNTIME_OUTPUT_DIRECTORY ${FETPAPI_BINARY_DIR}
		SUFFIX .${Fetpapi_VERSION}.dll
		IMPORT_SUFFIX .${Fetpapi_VERSION}.lib
		VERSION ${Fetpapi_VERSION_MAJOR}.${Fetpapi_VERSION_MINOR}
	)
else (WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		VERSION ${Fetpapi_VERSION}
		SOVERSION ${Fetpapi_VERSION_MAJOR}.${Fetpapi_VERSION_MINOR})
endif (WIN32)

if (WITH_ETP_SSL)
	target_compile_definitions(${PROJECT_NAME} PRIVATE WITH_ETP_SSL)
	if (NOT TARGET OpenSSL::SSL OR NOT TARGET OpenSSL::Crypto)
		find_package(OpenSSL REQUIRED)
	endif()
	target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto OpenSSL::applink)
endif ()

if (WITH_FESAPI)
	target_link_libraries(${PROJECT_NAME} PRIVATE FESAPI::FESAPI)
endif (WITH_FESAPI)

# Namespaces definitions
set (FETPAPI_ETP_NS etp CACHE STRING "Namespace for etp objects")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/nsDefinitions.h ${CMAKE_CURRENT_SOURCE_DIR}/src/nsDefinitions.h) # Overwrite if different

# ============================================================================
# SWIG wrapping
# ============================================================================

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/swigEtp1_2Include.i.in ${CMAKE_CURRENT_SOURCE_DIR}/swig/swigEtp1_2Include.i) # Overwrite if different
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
	target_include_directories(${PROJECT_NAME} PRIVATE ${FESAPI_INCLUDE_DIR})
endif (WITH_FESAPI)

target_sources(${PROJECT_NAME} PRIVATE ${ALL_SOURCES_AND_HEADERS})

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${AVRO_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (WITH_ETP_SSL)
	target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${OPENSSL_INCLUDE_DIR})
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

add_subdirectory(doc)

if (WITH_FESAPI)
	set (WITH_TEST OFF CACHE BOOL "Also build the unit tests.")
	if (WITH_TEST)
		enable_testing()
		add_subdirectory(test)
	endif (WITH_TEST)
endif (WITH_FESAPI)

set (WITH_EXAMPLE OFF CACHE BOOL "Also builds and installs an ETP1.2 client example.")
if (WITH_EXAMPLE)
	if (WITH_FESAPI)
		add_subdirectory(example/withFesapi)
	else ()
		add_subdirectory(example/withoutFesapi)
	endif (WITH_FESAPI)
endif (WITH_EXAMPLE)


# ============================================================================
# Install Fetpapi library
# ============================================================================
  
install(TARGETS ${PROJECT_NAME}
  EXPORT Fetpapi-export
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

IF (WIN32)
	#PDB for debug
	INSTALL (
		FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
		CONFIGURATIONS Debug RelWithDebInfo
		DESTINATION ${CMAKE_INSTALL_BINDIR}
		OPTIONAL
	)
ENDIF()

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

if (WITH_PYTHON_WRAPPING)
	IF (WIN32)
		install(
			TARGETS ${PROJECT_NAME}
			RUNTIME_DEPENDENCY_SET dep-set
		)
		cmake_path(GET SSL_EAY_RELEASE PARENT_PATH openSslLibPath)
		cmake_path(GET openSslLibPath PARENT_PATH openSslPath)
		cmake_path(APPEND openSslPath "bin" OUTPUT_VARIABLE openSslBinPath)
		install(
			RUNTIME_DEPENDENCY_SET dep-set
			PRE_EXCLUDE_REGEXES "api-ms-.*" "ext-ms-.*" "[Ff]esapi[Cc]pp.*\\.dll"
			POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
			DIRECTORIES ${openSslBinPath}
			RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/python/fetpapi/
		)
	ELSE ()
		install(
			TARGETS ${PROJECT_NAME}
			LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/python/fetpapi/
		)
	ENDIF (WIN32)
	install(CODE "execute_process(COMMAND	${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/python/setup.py bdist_wheel
											WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/python)")
endif (WITH_PYTHON_WRAPPING)