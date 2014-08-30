#!/bin/bash
CPPFILES="sglengine.cpp sglengine_objparser.cpp test.cpp"
if [ `uname` == "Linux" ] ; then
  g++ -o test.x $CPPFILES -lGLEW -lglfw3  -std=c++11 -lXxf86vm -lpthread -lXi -lXrandr -lGL -lX11 -Wall
else 
  g++ -o test.x $CPPFILES -lglew -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -O2 -std=c++11
fi
