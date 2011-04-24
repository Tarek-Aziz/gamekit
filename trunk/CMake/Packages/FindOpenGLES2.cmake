#-------------------------------------------------------------------
# This file is part of the CMake build system for OGRE
#     (Object-oriented Graphics Rendering Engine)
# For the latest info, see http://www.ogre3d.org/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# - Try to find OpenGLES and EGL
# Once done this will define
#  
#  OPENGLES2_FOUND        - system has OpenGLES
#  OPENGLES2_INCLUDE_DIR  - the GL include directory
#  OPENGLES2_LIBRARIES    - Link these to use OpenGLES
#
#  EGL_FOUND        - system has EGL
#  EGL_INCLUDE_DIR  - the EGL include directory
#  EGL_LIBRARIES    - Link these to use EGL

IF (WIN32)
  IF (CYGWIN)

    FIND_PATH(OPENGLES2_INCLUDE_DIR GLES2/gl2.h )

    FIND_LIBRARY(OPENGLES2_gl_LIBRARY libGLESv2 )

  ELSE (CYGWIN)

    IF(BORLAND)
      SET (OPENGLES2_gl_LIBRARY import32 CACHE STRING "OpenGL ES 2.x library for win32")
    ELSE(BORLAND)
	  #MS compiler - todo - fix the following line:
      #SET (OPENGLES_gl_LIBRARY ${OGRE_SOURCE_DIR}/Dependencies/lib/release/libGLESv2.lib CACHE STRING "OpenGL ES 2.x library for win32")
      #manual setting
      #SET (DEFAULT_OPENGLES_gl_LIBRARY libEGL libGLESv2) 
      SET (DEFAULT_OPENGLES2_INCLUDE_DIR ${OGREKIT_SOURCE_DIR}/Dependencies/Win32/gles2/include)
      SET (DEFAULT_OPENGLES2_LIBRARY_PATH ${OGREKIT_SOURCE_DIR}/Dependencies/Win32/gles2/lib)
      SET (OPENGLES_gl_LIBRARY   ${DEFAULT_OPENGLES2_LIBRARY_PATH}/libGLESv2.lib) #${DEFAULT_OPENGLES_gl_LIBRARY} CACHE STRING "OpenGL ES 2.x library for win32")
      #SET (EGL_LIBRARIES ${DEFAULT_OPENGLES2_LIBRARY_PATH}/libEGL.lib)
      SET (OPENGLES2_LIBRARIES ${DEFAULT_OPENGLES2_LIBRARY_PATH}/libGLESv2.lib
        ${DEFAULT_OPENGLES2_LIBRARY_PATH}/libEGL.lib)
      SET (OPENGLES2_INCLUDE_DIR ${DEFAULT_OPENGLES2_INCLUDE_DIR} CACHE STRING "OpenGL ES 2.x headers for win32")
      SET (OPENGLES2_LIBRARY_PATH ${DEFAULT_OPENGLES2_LIBRARY_PATH} CACHE STRING "OpenGL ES 2.x library path for win32")
    ENDIF(BORLAND)

  ENDIF (CYGWIN)

ELSE (WIN32)

  IF (APPLE)

	create_search_paths(/Developer/Platforms)
	findpkg_framework(OpenGLES2)
    set(OPENGLES2_gl_LIBRARY "-framework OpenGLES")

  ELSE(APPLE)

    FIND_PATH(OPENGLES2_INCLUDE_DIR GLES2/gl2.h
      /usr/openwin/share/include
      /opt/graphics/OpenGL/include /usr/X11R6/include
      /usr/include
    )

    FIND_LIBRARY(OPENGLES2_gl_LIBRARY
      NAMES GLESv2
      PATHS /opt/graphics/OpenGL/lib
            /usr/openwin/lib
            /usr/shlib /usr/X11R6/lib
            /usr/lib
    )

    IF (NOT OGREKIT_BUILD_ANDROID)
		FIND_PATH(EGL_INCLUDE_DIR EGL/egl.h
		  /usr/openwin/share/include
		  /opt/graphics/OpenGL/include /usr/X11R6/include
		  /usr/include
		)

		FIND_LIBRARY(EGL_egl_LIBRARY
		  NAMES EGL
		  PATHS /opt/graphics/OpenGL/lib
				/usr/openwin/lib
				/usr/shlib /usr/X11R6/lib
				/usr/lib
		)

		# On Unix OpenGL most certainly always requires X11.
		# Feel free to tighten up these conditions if you don't 
		# think this is always true.
		# It's not true on OSX.

		IF (OPENGLES2_gl_LIBRARY)
		  IF(NOT X11_FOUND)
			INCLUDE(FindX11)
		  ENDIF(NOT X11_FOUND)
		  IF (X11_FOUND)
			IF (NOT APPLE)
			  SET (OPENGLES2_LIBRARIES ${X11_LIBRARIES})
			ENDIF (NOT APPLE)
		  ENDIF (X11_FOUND)
		ENDIF (OPENGLES2_gl_LIBRARY)
    ENDIF ()

  ENDIF(APPLE)
ENDIF (WIN32)

SET( OPENGLES2_FOUND "YES" )
#SET( OPENGLES2_LIBRARIES ${OPENGLES2_gl_LIBRARY} ${OPENGLES2_LIBRARIES})

IF (OGREKIT_BUILD_ANDROID)
  IF(OPENGLES2_gl_LIBRARY)

      SET( OPENGLES2_LIBRARIES ${OPENGLES2_gl_LIBRARY} ${OPENGLES2_LIBRARIES})
      SET( EGL_LIBRARIES)
      SET( OPENGLES2_FOUND "YES" )
	  
	  message("GLESL FOUND" ${OPENGLES2_LIBRARIES})

  ENDIF(OPENGLES2_gl_LIBRARY)

ELSE ()

  SET( OPENGLES2_LIBRARIES ${OPENGLES2_gl_LIBRARY} ${OPENGLES2_LIBRARIES})

  IF(OPENGLES2_gl_LIBRARY AND EGL_egl_LIBRARY)

    SET( OPENGLES2_LIBRARIES ${OPENGLES2_gl_LIBRARY} ${OPENGLES2_LIBRARIES})
    SET( EGL_LIBRARIES ${EGL_egl_LIBRARY} ${EGL_LIBRARIES})
    SET( OPENGLES2_FOUND "YES" )

  ENDIF(OPENGLES2_gl_LIBRARY AND EGL_egl_LIBRARY)

ENDIF ()

if (MSVC)
  if (NOT OPENGLES2_LIBRARIES)
    #message(STATUS "not found")
    #set(OPENGLES2_FOUND "")
  endif()
endif ()


MARK_AS_ADVANCED(
  OPENGLES2_INCLUDE_DIR
  OPENGLES2_gl_LIBRARY
  EGL_INCLUDE_DIR
  EGL_egl_LIBRARY
)