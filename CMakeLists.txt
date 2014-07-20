cmake_minimum_required(VERSION 2.8)

project( SIMPLE_PROJECT ) # this is case sensitive 

######################################
# finds OpenGL, GLU and X11
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
find_package(PkgConfig REQUIRED)

if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)

pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

set(GL_LIBRARY GL GLU X11)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-O3 -Wall -std=c++11")

add_executable(test test.cpp sglengine.cpp sglengine_objparser.cpp)

# linking "glfw" and not "glfw3" 
# assumes that glfw was built with BUILD_SHARED_LIBS to ON
target_link_libraries(test ${GLFW_LIBRARIES} ${GL_LIBRARY} ${X11_LIBRARIES} m Xxf86vm pthread Xi Xrandr)
