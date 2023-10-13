cmake_minimum_required (VERSION 3.12)

add_executable (unitTest "")

# ============================================================================
# Getting source files
# ============================================================================

set(FETPAPI_TEST_SOURCES 	${CMAKE_CURRENT_SOURCE_DIR}/unitTest.cpp )
set(FETPAPI_TEST_HEADERS 	${CMAKE_CURRENT_SOURCE_DIR}/catch.hpp )

target_sources(unitTest PRIVATE ${FETPAPI_TEST_SOURCES} ${FETPAPI_TEST_HEADERS})

# ============================================================================
# include directories
# ============================================================================

target_include_directories(unitTest PRIVATE ${FESAPI_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/test/ ${CMAKE_SOURCE_DIR}/src/)
target_include_directories(unitTest SYSTEM PRIVATE ${AVRO_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${FETPAPI_INCLUDE_DIR})
if (WITH_ETP_SSL)
	target_include_directories(unitTest SYSTEM PRIVATE ${OPENSSL_INCLUDE_DIR})
endif ()

if (WIN32)
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
	target_compile_definitions(unitTest PRIVATE _WIN32_WINNT=${ver})

	target_link_libraries (unitTest PRIVATE bcrypt.lib)
	
	set_target_properties (unitTest PROPERTIES
		LINK_FLAGS "/INCREMENTAL:NO"
		RUNTIME_OUTPUT_DIRECTORY ${FETPAPI_BINARY_DIR})
endif (WIN32)

if (NOT Boost_USE_STATIC_LIBS)
	# https://www.boost.org/doc/libs/1_75_0/libs/log/doc/html/log/rationale/namespace_mangling.html
	target_compile_definitions(unitTest PRIVATE BOOST_LOG_DYN_LINK)
endif ()

target_compile_definitions(unitTest PRIVATE BOOST_ALL_NO_LIB)
find_package( Threads )
target_link_libraries (unitTest PRIVATE ${FESAPI_LIBRARY_RELEASE} ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

#OpenSSL
if (WITH_ETP_SSL)
	target_compile_definitions(unitTest PRIVATE WITH_ETP_SSL)
	if (NOT TARGET OpenSSL::SSL OR NOT TARGET OpenSSL::Crypto)
		find_package(OpenSSL REQUIRED)
	endif()
	target_link_libraries(unitTest PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif ()

#AVRO
set (AVRO_INCLUDE_DIR AVRO_INCLUDE_DIR-NOTFOUND CACHE PATH "Path to the directory which contains the avro header files")
if (NOT IS_DIRECTORY ${AVRO_INCLUDE_DIR})
	message(FATAL_ERROR "The Avro include dir (AVRO_INCLUDE_DIR variable) does not look to be a valid directory. Please modify it.")
endif ()
set (AVRO_LIBRARY_RELEASE AVRO_LIBRARY_RELEASE-NOTFOUND CACHE FILEPATH "Path to the file which contains the avro library release")
if (NOT EXISTS ${AVRO_LIBRARY_RELEASE})
	message(WARNING "The avro library (AVRO_LIBRARY_RELEASE variable) does not look to be a valid file. Please modify it.")
endif ()
set (AVRO_LIBRARY_DEBUG AVRO_LIBRARY_DEBUG-NOTFOUND CACHE FILEPATH "Path to the file which contains the AVRO library DEBUG")
if (NOT EXISTS ${AVRO_LIBRARY_DEBUG})
	message(WARNING "The AVRO library (AVRO_LIBRARY_DEBUG variable) does not look to be a valid file. Please modify it.")
endif ()

if (WIN32)
	if (EXISTS unitTest AND EXISTS ${AVRO_LIBRARY_DEBUG})
		target_link_libraries (unitTest PRIVATE optimized ${AVRO_LIBRARY_RELEASE} debug ${AVRO_LIBRARY_DEBUG})
	elseif (EXISTS ${AVRO_LIBRARY_RELEASE})
		target_link_libraries (unitTest PRIVATE ${AVRO_LIBRARY_RELEASE})
	elseif (EXISTS ${AVRO_LIBRARY_DEBUG})
		target_link_libraries (unitTest PRIVATE ${AVRO_LIBRARY_DEBUG})
	endif ()
else (WIN32)
	target_link_libraries (unitTest PRIVATE ${AVRO_LIBRARY_RELEASE})
endif (WIN32)

# ============================================================================
# build fesapiTest
# ============================================================================

add_test(NAME allTests COMMAND unitTest)
add_dependencies (unitTest ${PROJECT_NAME})
target_link_libraries (unitTest PRIVATE ${PROJECT_NAME})
