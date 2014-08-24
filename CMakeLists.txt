cmake_minimum_required(VERSION 2.8)

project( SGLENGINE_PROJECT ) # this is case sensitive 

######################################

find_package(OpenGL REQUIRED)
# find_package(X11 REQUIRED)
# find_package(PkgConfig REQUIRED)

if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)

# pkg_search_module(GLFW REQUIRED glfw3)
# find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS} ${X11_INCLUDE_DIR})

# set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /opt/local/lib)

# set(GL_LIBRARY GL GLU X11)

set(CMAKE_BUILD_TYPE Release)
SET(CXX_ADDITIONAL_COMPILE_FLAGS  "-O2 -Wall -std=c++11")
SET(CXX_ADDITIONAL_LINK_FLAGS     "")
SET(CXX_ADDITIONAL_LIBRARIES      "-lGLEW -lglfw3 -lXxf86vm -lpthread -lXi -lXrandr -lGL -lX11")

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CXX_ADDITIONAL_COMPILE_FLAGS}" )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${CXX_ADDITIONAL_LINK_FLAGS}" )

add_executable(test test.cpp sglengine.cpp sglengine_objparser.cpp)

target_link_libraries(test ${OPENGL_LIBRARY} ${X11_LIBRARIES} ${CXX_ADDITIONAL_LIBRARIES})
