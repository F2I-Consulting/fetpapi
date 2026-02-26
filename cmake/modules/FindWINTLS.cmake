# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
WINTLS
--------

Find the native WinTLS includes.
Heavily inspired by the official FindZLIB cmake module v3.27.

IMPORTED Targets
^^^^^^^^^^^^^^^^

This module defines :prop_tgt:`IMPORTED` target ``WINTLS::WINTLS``, if
WINTLS has been found.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

::

  WINTLS_INCLUDE_DIRS   - where to find wintls.hpp
  WINTLS_FOUND          - True if WINTLS found.

Hints
^^^^^

A user may set ``WINTLS_ROOT`` to a WINTLS installation root to tell this
module where to look.
#]=======================================================================]

if(WINTLS_FIND_COMPONENTS AND NOT WINTLS_FIND_QUIETLY)
  message(AUTHOR_WARNING
    "WINTLS does not provide any COMPONENTS.  Calling\n"
    "  find_package(WINTLS COMPONENTS ...)\n"
    "will always fail."
    )
endif()

set(_WINTLS_SEARCHES)

# Search WINTLS_ROOT first if it is set.
if(WINTLS_ROOT)
  set(_WINTLS_SEARCH_ROOT PATHS ${WINTLS_ROOT} NO_DEFAULT_PATH)
  list(APPEND _WINTLS_SEARCHES _WINTLS_SEARCH_ROOT)
endif()

# Normal search.
set(_WINTLS_x86 "(x86)")
set(_WINTLS_SEARCH_NORMAL
    PATHS "$ENV{ProgramFiles}/WINTLS"
          "$ENV{ProgramFiles${_WINTLS_x86}}/WINTLS")
unset(_WINTLS_x86)
list(APPEND _WINTLS_SEARCHES _WINTLS_SEARCH_NORMAL)

# Try each search configuration.
foreach(search ${_WINTLS_SEARCHES})
  find_path(WINTLS_INCLUDE_DIR NAMES wintls.hpp ${${search}} PATH_SUFFIXES include)
endforeach()

mark_as_advanced(WINTLS_INCLUDE_DIR)

find_package_handle_standard_args(WINTLS REQUIRED_VARS WINTLS_INCLUDE_DIR)

if(WINTLS_FOUND)
    set(WINTLS_INCLUDE_DIRS ${WINTLS_INCLUDE_DIR})

    if(NOT TARGET WINTLS::WINTLS)
      add_library(WINTLS::WINTLS INTERFACE IMPORTED)
      set_target_properties(WINTLS::WINTLS PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${WINTLS_INCLUDE_DIRS}")
    endif()
endif()
