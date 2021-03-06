# - Find LCMS2
# Find the LCMS2 includes and library
# This module defines
#  LCMS2_INCLUDE_DIR, where to find lcms.h
#  LCMS2_LIBRARIES, the libraries needed to use LCMS2.
#  LCMS2_VERSION, The value of LCMS_VERSION defined in lcms.h
#  LCMS2_FOUND, If false, do not try to use LCMS2.


# Copyright (c) 2008, Adrian Page, <adrian@pagenet.plus.com>
# Copyright (c) 2009, Cyrille Berger, <cberger@cberger.net>
#
# Redistribution and use is allowed according to the terms of the BSD license.
#
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#
#   1. Redistributions of source code must retain the copyright
#      notice, this list of conditions and the following disclaimer.
#   2. Redistributions in binary form must reproduce the copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#   3. The name of the author may not be used to endorse or promote products
#      derived from this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
#   IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
#   OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
#   IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
#   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
#   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
#   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
#   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
#   THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
if(NOT WIN32)
   find_package(PkgConfig)
   pkg_check_modules(PC_LCMS2 lcms2)
   set(LCMS2_DEFINITIONS ${PC_LCMS2_CFLAGS_OTHER})
endif()

find_path(LCMS2_INCLUDE_DIR lcms2.h
   PATHS
   ${PC_LCMS2_INCLUDEDIR}
   ${PC_LCMS2_INCLUDE_DIRS}
   PATH_SUFFIXES lcms2 liblcms2
)

find_library(LCMS2_LIBRARIES NAMES lcms2 liblcms2 lcms-2 liblcms-2
   PATHS     
   ${PC_LCMS2_LIBDIR}
   ${PC_LCMS2_LIBRARY_DIRS}
   PATH_SUFFIXES lcms2
)

if(LCMS2_INCLUDE_DIR AND LCMS2_LIBRARIES)
   set(LCMS2_FOUND TRUE)
else()
   set(LCMS2_FOUND FALSE)
endif()

if(LCMS2_FOUND)
   file(READ ${LCMS2_INCLUDE_DIR}/lcms2.h LCMS2_VERSION_CONTENT)
   string(REGEX MATCH "#define LCMS_VERSION[ ]*[0-9]*\n" LCMS2_VERSION_MATCH ${LCMS2_VERSION_CONTENT})
   if(LCMS2_VERSION_MATCH)
      string(REGEX REPLACE "#define LCMS_VERSION[ ]*([0-9]*)\n" "\\1" LCMS2_VERSION ${LCMS2_VERSION_MATCH})
      if(NOT LCMS2_FIND_QUIETLY)
         string(SUBSTRING ${LCMS2_VERSION} 0 1 LCMS2_MAJOR_VERSION)
         string(SUBSTRING ${LCMS2_VERSION} 1 2 LCMS2_MINOR_VERSION)
         message(STATUS "Found lcms version ${LCMS2_MAJOR_VERSION}.${LCMS2_MINOR_VERSION}, ${LCMS2_LIBRARIES}")
      endif()
   else()
      if(NOT LCMS2_FIND_QUIETLY)
         message(STATUS "Found lcms2 but failed to find version ${LCMS2_LIBRARIES}")
      endif()
      set(LCMS2_VERSION NOTFOUND)
   endif()
else()
   if(NOT LCMS2_FIND_QUIETLY)
      if(LCMS2_FIND_REQUIRED)
         message(FATAL_ERROR "Required package lcms2 NOT found")
      else()
         message(STATUS "lcms2 NOT found")
      endif()
   endif()
endif()

mark_as_advanced(LCMS2_INCLUDE_DIR LCMS2_LIBRARIES LCMS2_VERSION)

