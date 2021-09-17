# Included in ../src/CmakeLists.txt if java build is selected.

find_package(SWIG 3.0 REQUIRED)

find_package(JNI REQUIRED)
find_package(Java REQUIRED)
include(UseJava)

set(SWIG_LINKED_TO_RELEASE ON CACHE BOOL "Is your SWIG generated library linked to the release or debug version of FesapiCpp ?")

message("Generating SWIG Java files...")
#Cleaning
file (GLOB TO_DELETE ${CMAKE_SOURCE_DIR}/java/src/com/f2i_consulting/fetpapi/*.java)
if (TO_DELETE)
	file (REMOVE ${TO_DELETE})
endif (TO_DELETE)
file (REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/java/src/com/f2i_consulting/fetpapi/etp)

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
list (APPEND EXECUTE_COMMAND -java)
if ("${SWIG_VERSION}" VERSION_GREATER_EQUAL "4.0.0")
        list (APPEND EXECUTE_COMMAND -doxygen)
endif ()
if (WITH_FESAPI)
        list (APPEND EXECUTE_COMMAND -DWITH_FESAPI)
endif (WITH_FESAPI)
if (WITH_ETP_SSL)
        list (APPEND EXECUTE_COMMAND -DWITH_ETP_SSL)
endif (WITH_ETP_SSL)
list (APPEND EXECUTE_COMMAND -package)
list (APPEND EXECUTE_COMMAND com.f2i_consulting.fetpapi)
list (APPEND EXECUTE_COMMAND -o)
list (APPEND EXECUTE_COMMAND ${CMAKE_SOURCE_DIR}/swig/swigGeneratedJavaWrapper.cpp)
list (APPEND EXECUTE_COMMAND -outdir)
list (APPEND EXECUTE_COMMAND ${CMAKE_SOURCE_DIR}/java/src/com/f2i_consulting/fetpapi/)
list (APPEND EXECUTE_COMMAND ${CMAKE_SOURCE_DIR}/swig/swigEtp1_2Include.i)
if (WIN32)
        execute_process(COMMAND powershell ${EXECUTE_COMMAND})
elseif (UNIX)
        execute_process(COMMAND ${EXECUTE_COMMAND})
endif (WIN32)
message("SWIG Java files have been generated.")

# Building the jar
set(CMAKE_JAVA_COMPILE_FLAGS -g)
file (GLOB_RECURSE SWIG_JAVA_FILES ${CMAKE_SOURCE_DIR}/java/src/com/f2i_consulting/fetpapi/*.java)
if (SWIG_LINKED_TO_RELEASE)
	if (NOT EXISTS ${FESAPI_JAR})
		add_jar(fetpapiJava${CMAKE_RELEASE_POSTFIX} ${SWIG_JAVA_FILES}
				VERSION ${Fetpapi_VERSION})
	else ()
		add_jar(fetpapiJava${CMAKE_RELEASE_POSTFIX} ${SWIG_JAVA_FILES}
				INCLUDE_JARS ${FESAPI_JAR}
				VERSION ${Fetpapi_VERSION})
	endif ()
else (SWIG_LINKED_TO_RELEASE)
	if (NOT EXISTS ${FESAPI_JAR})
		add_jar(fetpapiJava${CMAKE_DEBUG_POSTFIX} ${SWIG_JAVA_FILES}
				VERSION ${Fetpapi_VERSION})
	else ()
		add_jar(fetpapiJava${CMAKE_DEBUG_POSTFIX} ${SWIG_JAVA_FILES}
				INCLUDE_JARS ${FESAPI_JAR}
				VERSION ${Fetpapi_VERSION})
	endif ()
endif (SWIG_LINKED_TO_RELEASE)

target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/swig/swigGeneratedJavaWrapper.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/swig/swigGeneratedJavaWrapper.h)

target_include_directories(${PROJECT_NAME} PUBLIC ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2})

if (SWIG_LINKED_TO_RELEASE)
	install (
		FILES ${CMAKE_CURRENT_BINARY_DIR}/fetpapiJava${CMAKE_RELEASE_POSTFIX}-${Fetpapi_VERSION}.jar
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)
else (SWIG_LINKED_TO_RELEASE)
	install (
		FILES ${CMAKE_CURRENT_BINARY_DIR}/fetpapiJava${CMAKE_DEBUG_POSTFIX}-${Fesapi_VERSION}.jar
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)
endif (SWIG_LINKED_TO_RELEASE)
