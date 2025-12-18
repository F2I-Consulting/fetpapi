set(FETPAPI_ETP_SOURCES	${CMAKE_CURRENT_LIST_DIR}/AbstractSession.cpp
						${CMAKE_CURRENT_LIST_DIR}/ClientSessionLaunchers.cpp
						${CMAKE_CURRENT_LIST_DIR}/EtpHelpers.cpp
						${CMAKE_CURRENT_LIST_DIR}/InitializationParameters.cpp
						${CMAKE_CURRENT_LIST_DIR}/PlainClientSession.cpp )
set(FETPAPI_ETP_HEADERS	${CMAKE_CURRENT_LIST_DIR}/AbstractClientSessionCRTP.h
						${CMAKE_CURRENT_LIST_DIR}/AbstractSession.h
						${CMAKE_CURRENT_LIST_DIR}/ClientSession.h
						${CMAKE_CURRENT_LIST_DIR}/ClientSessionLaunchers.h
						${CMAKE_CURRENT_LIST_DIR}/EtpException.h
						${CMAKE_CURRENT_LIST_DIR}/EtpHelpers.h
						${CMAKE_CURRENT_LIST_DIR}/EtpMessages.h
						${CMAKE_CURRENT_LIST_DIR}/HttpClientSession.h
						${CMAKE_CURRENT_LIST_DIR}/InitializationParameters.h
						${CMAKE_CURRENT_LIST_DIR}/PlainClientSession.h )

include(${CMAKE_CURRENT_LIST_DIR}/protocolHandlers/CMakeLists.txt)
if (WITH_FESAPI)
include(${CMAKE_CURRENT_LIST_DIR}/fesapi/CMakeLists.txt)
endif (WITH_FESAPI)
if (WITH_ETP_SSL)
include(${CMAKE_CURRENT_LIST_DIR}/ssl/CMakeLists.txt)
endif (WITH_ETP_SSL)
