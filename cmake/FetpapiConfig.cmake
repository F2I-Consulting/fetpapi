include(CMakeFindDependencyMacro)

find_dependency(Threads)

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

find_dependency(Boost)
if (WIN32 AND (Boost_VERSION_MAJOR EQUAL 1) AND (Boost_VERSION_MINOR LESS 74) AND (Boost_VERSION_MINOR GREATER 71))
		message(WARNING "You may experience min/max issue with this boost version : See https://github.com/boostorg/beast/issues/1980")
endif ()

include("${CMAKE_CURRENT_LIST_DIR}/FetpapiTargets.cmake")
