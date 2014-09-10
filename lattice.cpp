#include <iostream>
#include <cstdio>
#include <cstdlib>

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "./lattice.x xNum yNum zNum" <<std::endl;
    exit(1);
  }
  int Nx=atoi(argv[1]), Ny=atoi(argv[1]), Nz=atoi(argv[2]);
  float xdelta=2, ydelta=2, zdelta=2;
  float xScale=1.0, yScale=1.0, zScale=1.0;
  int white=0;
  cout << "o obj/cube_uv.obj 1.0 1.0 1.0\n"
       << "o obj/cube_uv.obj 0.0 0.0 0.0\n"
       << "o obj/cube_uv.obj 1.0 0.0 0.0\n"
       << "o obj/cube_uv.obj 0.2 0.2 0.2\n"
       << "t textures/cubetex1.bmp\n"
       << "s shaders/simple.vs shaders/simple_color.fs" << endl;
  for (int nx=0; nx<Nx; nx++) {
    for (int ny=0; ny<Ny; ny++) {
      for (int nz=0; nz<Nz; nz++) {
      xScale=1.0; yScale=1.0; zScale=1.0;
      cout << "sc " << rand() % 4 << " 0 0 "
        << nx*xdelta << " " << ny*ydelta << " " << nz*zdelta << " " 
        << xScale << " " << yScale << " " << zScale << " "
        << "0.0 0.0 0.0" << endl;
      }
    }
  }
}
