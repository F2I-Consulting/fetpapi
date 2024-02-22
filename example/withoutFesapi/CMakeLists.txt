CMAKE_MINIMUM_REQUIRED (VERSION 3.12)

project (etpClient)

# ============================================================================
# build etpClient
# ============================================================================

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

set(CMAKE_INSTALL_RPATH "\$ORIGIN/${CMAKE_INSTALL_LIBDIR}")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_executable (${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/etpClient.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MyOwnCoreProtocolHandlers.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MyOwnCoreProtocolHandlers.h
	${CMAKE_CURRENT_SOURCE_DIR}/MyOwnDiscoveryProtocolHandlers.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MyOwnDiscoveryProtocolHandlers.h
	${CMAKE_CURRENT_SOURCE_DIR}/MyOwnStoreNotificationProtocolHandlers.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MyOwnStoreNotificationProtocolHandlers.h
	${CMAKE_CURRENT_SOURCE_DIR}/MyOwnStoreProtocolHandlers.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/MyOwnStoreProtocolHandlers.h)

target_include_directories (${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/)

add_dependencies (${PROJECT_NAME} Fetpapi)
target_link_libraries (${PROJECT_NAME} PRIVATE Fetpapi)

target_compile_definitions(${PROJECT_NAME} PRIVATE BOOST_ALL_NO_LIB)
target_link_libraries (${PROJECT_NAME} PRIVATE AVRO::AVRO ${CMAKE_THREAD_LIBS_INIT})
if (DEFINED Boost_SYSTEM_LIBRARY)
	target_link_libraries (${PROJECT_NAME} PRIVATE ${Boost_SYSTEM_LIBRARY})
endif()
if (WIN32)
	target_link_libraries (${PROJECT_NAME} PRIVATE bcrypt.lib)
endif()
if (WITH_ETP_SSL)
	target_compile_definitions(${PROJECT_NAME} PRIVATE WITH_ETP_SSL)
	target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif ()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${AVRO_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (WITH_ETP_SSL)
	target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${OPENSSL_INCLUDE_DIR})
endif ()

if (WIN32)
	set_target_properties (${PROJECT_NAME} PROPERTIES
		LINK_FLAGS "/INCREMENTAL:NO"
		RUNTIME_OUTPUT_DIRECTORY ${FETPAPI_BINARY_DIR})
endif (WIN32)

# The value of DEBUG_POSTFIX property is initialized when the target is created to the value of the variable CMAKE_<CONFIG>_POSTFIX
# except for executable targets because earlier CMake versions which did not use this variable for executables.
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

install (
	TARGETS ${PROJECT_NAME}
	DESTINATION ${CMAKE_INSTALL_PREFIX}
)
