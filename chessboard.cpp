#include <iostream>
#include <cstdio>
#include <cstdlib>

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "./chessboard.x xNum yNum" <<std::endl;
    exit(1);
  }
  int Nx=atoi(argv[1]), Ny=atoi(argv[1]);
  float xdelta=2, ydelta=2;
  float xScale=1.0, yScale=1.0, zScale=1.0;
  int white=0;
  cout << "o obj/cube_uv.obj 1.0 1.0 1.0" << std::endl
       << "o obj/cube_uv.obj 0.0 0.0 0.0" << std::endl
       << "o obj/cube_uv.obj 1.0 0.0 0.0" << std::endl
       << "o obj/VW-new-beetle.obj 0.0 1.0 0.0" << std::endl
       << "t textures/cubetex1.bmp\ns shaders/simple.vs shaders/simple_color.fs" << endl;
  for (int nx=0; nx<Nx; nx++) {
    for (int ny=0; ny<Ny; ny++) {
      if ( (nx + ny) % 2 == 0) {
        white=0;
      } else {
        white=1;
      }
      xScale=1.0; yScale=1.0; zScale=1.0;
      cout << "sc " << white << " 0 0 " 
        << nx*xdelta << " " << ny*ydelta << " " << 0 << " " 
        << xScale << " " << yScale << " " << zScale << " "
        << "0.0 0.0 0.0" << endl;
    }
  }

  for (int nx=0; nx<Nx; nx++) {
      xScale=1.0; yScale=0.5; zScale=1.0;
      cout << "sc " << "2" << " 0 0 " << nx*xdelta << " " << Ny*ydelta-0.5 << " " << 0 << " "
        << xScale << " " << yScale << " " << zScale << " "
        << "0.0 0.0 0.0" << endl;
      cout << "sc " << "2" << " 0 0 " << nx*xdelta << " " << -1.5 << " " << 0 << " "
        << xScale << " " << yScale << " " << zScale << " "
        << "0.0 0.0 0.0" << endl;
  }
  
  for (int ny=0; ny<Ny; ny++) {
      xScale=0.5; yScale=1.0; zScale=1.0;
      cout << "sc " << "2" << " 0 0 " << Nx*xdelta-0.5 << " " << ny*ydelta << " " << 0 << " "
        << xScale << " " << yScale << " " << zScale << " "
        << "0.0 0.0 0.0" << endl;
      cout << "sc " << "2" << " 0 0 " << -1.5 << " " << ny*ydelta << " " << 0 << " "
        << xScale << " " << yScale << " " << zScale << " "
        << "0.0 0.0 0.0" << endl;
  }
      
  xScale=0.5; yScale=0.5; zScale=1.0;
  cout << "sc " << "2" << " 0 0 " << -1.5 << " " << -1.5 << " " << 0 << " "
    << xScale << " " << yScale << " " << zScale << " "
    << "0.0 0.0 0.0" << endl;
  cout << "sc " << "2" << " 0 0 " << -1.5 << " " << Ny*ydelta-0.5 << " " << 0 << " "
    << xScale << " " << yScale << " " << zScale << " "
    << "0.0 0.0 0.0" << endl;
  
  cout << "sc " << "2" << " 0 0 " << Nx*xdelta-0.5 << " " << -1.5 << " " << 0 << " "
    << xScale << " " << yScale << " " << zScale << " "
    << "0.0 0.0 0.0" << endl;
  cout << "sc " << "2" << " 0 0 " << Nx*xdelta-0.5 << " " << Ny*ydelta-0.5 << " " << 0 << " "
    << xScale << " " << yScale << " " << zScale << " "
    << "0.0 0.0 0.0" << endl;

  // Add an additional cube
  xScale=1.0; yScale=1.0; zScale=1.0;
  cout << "sc " << "3" << " 0 0 " << Nx/(float)2.0*xdelta << " " << Ny/(float)2.0*ydelta-5.5 << " " << -120 << " "
    << xScale << " " << yScale << " " << zScale << " "
    << "0.0 0.0 0.0" << endl;

}
