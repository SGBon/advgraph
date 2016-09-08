cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(lab01)

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# FIND FREEIMAGE
#########################################################
find_path(FREEIMAGE_INCLUDE FreeImage.h)
find_library(FREEIMAGE_LIB freeimage)
include_directories(${FREEIMAGE_INCLUDE})

#########################################################
# Include Files
#########################################################
add_executable(lab01 test.c)

########################################################
# Linking & stuff
#########################################################

  # create the program "test"
  target_link_libraries(lab01 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${FREEIMAGE_LIB})
