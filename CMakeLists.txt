# 
# Copyright (C) 2021 Jordan Hendl
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

# 
# File:   CMakeLists.h
# Author: Jordan Hendl
# 
# Created on December 31, 2020, 11:30 PM
# 

CMAKE_MINIMUM_REQUIRED( VERSION 3.8.0 )

LIST( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

INCLUDE( Message   )
INCLUDE( BuildTest )

# Build options
OPTION( BUILD_DOCS    "Whether or not to generate documentation of the modules" OFF )
OPTION( BUILD_RELEASE "Whether or not the to build for release."                OFF )
OPTION( BUILD_TESTS   "Whether or not the to build tests."                      ON  )
OPTION( RUN_TESTS     "Whether or not the module tests should be run if built." OFF )

PROJECT( yggdrasil_module_collection_1 CXX )

MESSAGE( INFO "Yggdrasil Module Collection 1 Prebuild" ) 
MESSAGE( INFO " " ) 

IF( BUILD_RELEASE  )
  MESSAGE( INFO "Building for release" )
  IF( MSVC )
    ADD_COMPILE_OPTIONS( /W2 /03 )
  ELSEIF( UNIX AND NOT APPLE )
    ADD_COMPILE_OPTIONS( -Wall -Wextra -pedantic -Werror -fPIC -O3 )
  ENDIF()
ELSE()
  MESSAGE( INFO "Building for debug" )
  IF( MSVC )
    ADD_COMPILE_OPTIONS( /DEBUG )
  ELSEIF( UNIX AND NOT APPLE )
    ADD_COMPILE_OPTIONS( -Wall -Wextra -pedantic -Werror -fPIC -g )
  ENDIF()
ENDIF()

# Print build options
MESSAGE( STATUS "" ) 
MESSAGE( INFO "Build Options:" ) 
MESSAGE( INFO "├─BUILD_DOCS       ${BUILD_DOCS}   " )
MESSAGE( INFO "├─BUILD_RELEASE    ${BUILD_RELEASE}" )
MESSAGE( INFO "├─BUILD_TESTS      ${BUILD_TESTS}  " )
MESSAGE( INFO "└─RUN_TESTS        ${RUN_TESTS}    " )
MESSAGE( STATUS "" ) 

# Set build config.
SET( ARCHITECTURE "64bit" CACHE STRING "The system architecture."                     )
SET( CXX_STANDARD "17"    CACHE STRING "The C++ standard to use for building."        )
SET( MAJOR        "0"     CACHE STRING "The major version of this build."             )
SET( MINOR        "1"     CACHE STRING "The minor version of this build."             )
SET( BRANCH       "0"     CACHE STRING "The branch version of this build."            )
SET( REVISION     "0"     CACHE STRING "The revision of this build."                  )
SET( GENERATOR    "RPM"   CACHE STRING "The Package Generator to use for this build." )

IF( WIN32 )
  SET( CMAKE_PREFIX_PATH "C:/Program Files" CACHE STRING "The default path to look for dependancies." )
  SET( INSTALL_LOCATION "C:/Program Files" CACHE STRING "The default NSIS install location of this library" )
ELSEIF( UNIX AND NOT APPLE )
  SET( CMAKE_PREFIX_PATH "/usr/local/lib64/athena;/usr/local/lib64/iris;/usr/local/lib64/yggdrasil" CACHE STRING "The default path to look for dependancies." )
  SET( INSTALL_LOCATION  "/usr/local/lib64" CACHE STRING "The RPM install location of this library." ) 
ENDIF()

SET(CMAKE_CXX_STANDARD          17 )
SET(CMAKE_CXX_STANDARD_REQUIRED ON )

SET( PROJECT_VERSION "${MAJOR}.${MINOR}.${BRANCH}" )

# Print build configurations.
MESSAGE( INFO " " ) 
MESSAGE( INFO "Build Configuration:"                 )
MESSAGE( INFO "├─ARCHITECTURE:   ${ARCHITECTURE}"    )
MESSAGE( INFO "├─CXX_STANDARD:   ${CXX_STANDARD}"    )
MESSAGE( INFO "└─VERSION:        ${PROJECT_VERSION}" )
MESSAGE( INFO "  ├─MAJOR:        ${MAJOR}"           )
MESSAGE( INFO "  ├─MINOR:        ${MINOR}"           )
MESSAGE( INFO "  ├─BRANCH:       ${BRANCH}"          )
MESSAGE( INFO "  └─REVISION:     ${REVISION}"        )

STRING( TIMESTAMP DATE "%Y%m%d" )
STRING( TIMESTAMP Time "%H%M%S" )

# Handle compiler information
STRING( TOLOWER ${CMAKE_CXX_COMPILER_ID} CXX_COMPILER_NAME )
SET( CXX_COMPILER  "${CXX_COMPILER_NAME}${CMAKE_CXX_COMPILER_VERSION}" )

SET( CLANG FALSE )
SET( INTEL FALSE )
SET( GNU   FALSE )
SET( MSVC  FALSE )
IF( CXX_COMPILER_NAME MATCHES "clang" )
  SET( CLANG TRUE )
ELSEIF( CXX_COMPILER_NAME MATCHES "gnu" )
  SET( GNU TRUE )
ELSEIF( CXX_COMPILER_NAME MATCHES "intel" )
  SET( INTEL TRUE )
ELSEIF( CXX_COMPILER_NAME MATCHES "msvc" )
  SET( MSVC TRUE )
ENDIF()

MESSAGE( INFO " " ) 
MESSAGE( INFO  "System Configuration:"                     )
MESSAGE( INFO  "├─CXX_COMPILER_NAME: ${CXX_COMPILER_NAME}" )
MESSAGE( INFO  "├─CXX_COMPILER:      ${CXX_COMPILER}"      )
MESSAGE( INFO  "├─INTEL:             ${INTEL}"             )
MESSAGE( INFO  "├─GNU:               ${GNU}"               )
MESSAGE( INFO  "└─MSVC:              ${MSVC}"              )
MESSAGE( INFO " " ) 


SET( BIN_DIR     "bin"                            )
SET( INCLUDE_DIR "include/${CMAKE_PROJECT_NAME}"  )
SET( LIB_DIR     "modules"                        )
SET( DOC_DIR     "doc"                            )
SET( CONFIG_DIR   ${CMAKE_PROJECT_NAME}           )
SET( DOXYGEN_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/doc )

SET( EXPORT_BIN_DIR     "${BIN_DIR}"     )
SET( EXPORT_INCLUDE_DIR "${INCLUDE_DIR}" )
SET( EXPORT_LIB_DIR     "${LIB_DIR}"     )
SET( EXPORT_DOC_DIR     "${DOC_DIR}"     )
SET( EXPORT_CONFIG_DIR  "${CONFIG_DIR}"  )

# Set output directories.
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules )
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules )
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin     )
SET(DOCUMENTATION_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/doc     )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/src ) 

INCLUDE( KPackage  )
INCLUDE( BuildDocs )

ADD_SUBDIRECTORY( src )
