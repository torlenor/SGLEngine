#!/bin/bash
CPPFILES="sglengine.cpp sglengine_objparser.cpp test.cpp"
g++ -o test.x $CPPFILES -lglew -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
