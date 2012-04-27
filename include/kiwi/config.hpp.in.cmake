/*
 *******************************************************************************
 *   Copyright (C) 2012 Vasiliy Sheredeko
 *   MIT license. All Rights Reserved.
 *******************************************************************************
 */
#ifndef KIWI_CONFIG_INCLUDED
#define KIWI_CONFIG_INCLUDED

#include <boost/preprocessor/stringize.hpp>

/*
 * AUTO-GENERATION WARNING:
 *     This file has been automatically generated from "Application.hpp.in.cmake".
 *     DO NOT edit this file, as any changes will be automatically discarded.
 */


///==--------------------------------------------------------------------------------------------------------------==///
///   General declorations
///==--------------------------------------------------------------------------------------------------------------==///
#cmakedefine KIWI_NAME               "@KIWI_NAME@"
#cmakedefine KIWI_CODENAME           "@KIWI_CODENAME@"
#cmakedefine KIWI_COPYRIGHT_YEARS    "@KIWI_COPYRIGHT_YEARS@"
#cmakedefine KIWI_VERSION_MAJOR      @KIWI_VERSION_MAJOR@
#cmakedefine KIWI_VERSION_MINOR      @KIWI_VERSION_MINOR@
#cmakedefine KIWI_VERSION_PATCH      @KIWI_VERSION_PATCH@
#cmakedefine KIWI_VERSION_TYPE       "@KIWI_VERSION_TYPE@"
#cmakedefine KIWI_VERSION_STRING     "@KIWI_VERSION_STRING@"
#cmakedefine KIWI_VENDOR_ID          "@KIWI_VENDOR_ID@"
#cmakedefine KIWI_VENDOR_NAME        "@KIWI_VENDOR_NAME@"
#cmakedefine KIWI_VENDOR_URL         "@KIWI_VENDOR_URL@"
#cmakedefine KIWI_ID                 "@KIWI_VENDOR_ID@"

///==--------------------------------------------------------------------------------------------------------------==///
///   OS type declorations
///==--------------------------------------------------------------------------------------------------------------==///

#ifndef KIWI_UNIX
#   define KIWI_UNIX ${KIWI_UNIX}
#endif

#ifndef KIWI_WIN32
#   define KIWI_WIN32 ${KIWI_WIN32}
#endif

///==--------------------------------------------------------------------------------------------------------------==///
///   Build type declorations
///==--------------------------------------------------------------------------------------------------------------==///

/// \def KIWI_LIBRARY
/// All public and exported symbols must be marked with this macros
#ifndef KIWI_LIBRARY
#   define KIWI_LIBRARY ${KIWI_LIBRARY}
#endif

/// \def KIWI_DEBUG
#cmakedefine KIWI_DEBUG

/// \def KIWI_RELEASE
#cmakedefine KIWI_RELEASE


#cmakedefine KIWI_GC

///==--------------------------------------------------------------------------------------------------------------==///
///   Export and import declorations
///==--------------------------------------------------------------------------------------------------------------==///

#if defined _WIN32 || defined __CYGWIN__
#   define KIWI_API_IMPORT __declspec(dllimport)
#   define KIWI_API_EXPORT __declspec(dllexport)
#   define KIWI_API_LOCAL
#else
#   if __GNUC__ >= 4
#       define KIWI_API_IMPORT __attribute__ ((visibility("default")))
#       define KIWI_API_EXPORT __attribute__ ((visibility("default")))
#       define KIWI_API_LOCAL  __attribute__ ((visibility("hidden")))
#   else
#       define KIWI_API_IMPORT
#       define KIWI_API_EXPORT
#       define KIWI_API_LOCAL
#   endif
#endif

#ifdef KIWI_LIBRARY
#   ifdef KIWI_DLL_EXPORTS // defined if we are building the KIWI DLL (instead of using it)
#       define KIWI_API KIWI_API_EXPORT
#   else
#       define KIWI_API KIWI_API_IMPORT
#   endif // KIWI_DLL_EXPORTS
#endif // KIWI_DLL

/// \def KIWI_LOCAL
/// All public and exported symbols must be marked with this macros
#ifndef KIWI_LOCAL
#   define KIWI_LOCAL KIWI_API_LOCAL
#endif

/// \def KIWI_API
/// All public and exported symbols must be marked with this macros
#ifndef KIWI_API
#   define KIWI_API
#endif

/// \def KIWI_LOCAL
/// All private and hidden symbols must be marked with this macros
#ifndef KIWI_LOCAL
#   define KIWI_LOCAL
#endif

/// \def KIWI_RUNTIME
/// All internal methods mustbe marked with this macros
#ifndef KIWI_RUNTIME
#   define KIWI_RUNTIME extern "C"
#endif

#ifndef KIWI_NAME
#   error "Please specify the application name in the top-level CMakeLists.txt file."
#endif

#ifndef KIWI_CODENAME
#   error "Please specify the application codename in the top-level CMakeLists.txt file."
#endif

#ifndef KIWI_COPYRIGHT_YEARS
#   error "Please specify the application copyright years (years during which this application was created, edited, or modified) in the top-level CMakeLists.txt file."
#endif

#ifndef KIWI_VERSION_MAJOR
#   define KIWI_VERSOIN_MAJOR 0
#endif

#ifndef KIWI_VERSION_MINOR
#   define KIWI_VERSION_MINOR 0
#endif

#ifndef KIWI_VERSION_PATCH
#   define KIWI_VERSION_PATCH 0
#endif

#ifndef KIWI_VERSION_TYPE
#   define KIWI_VERSION_TYPE "SNAPSHOT"
#endif

#ifndef KIWI_VERSION_STRING
#   define KIWI_VERSION_STRING BOOST_PP_STRINGIZE(KIWI_VERSION_MAJOR) "." BOOST_PP_STRINGIZE(KIWI_VERSION_MINOR) "." BOOST_PP_STRINGIZE(KIWI_VERSION_PATCH) "-" KIWI_VERSION_TYPE
#endif

#ifndef KIWI_VENDOR_ID
#   error "Please specify the application vendor id in the top-level CMakeLists.txt file."
#endif

#ifndef KIWI_VENDOR_NAME
#   error "Please specify the application vendor name in the top-level CMakeLists.txt file."
#endif

#ifndef KIWI_VENDOR_URL
#   error "Please specify the application vendor URL in the top-level CMakeLists.txt file."
#endif

#ifndef KIWI_ID
#   define KIWI_ID KIWI_VENDOR_ID "." KIWI_CODENAME
#endif

#endif
