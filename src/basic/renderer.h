#ifndef RENDERER_H
#define RENDERER_H

#include "model.h"
#include <SDL2/SDL_opengl.h>
#include <gl/GL.h>
#include <gl/GLU.h>

class Renderer {
 public:

  Renderer(size_t grid_size, std::size_t windows_size_x, std::size_t windows_size_y);
       
  void Display(const Model::VectorkSize& density, const Model::VectorkSize& u,
               const Model::VectorkSize& v);

 private:
  void PreDisplay();
  void DrawDensity(const Model::VectorkSize& density);
  void DrawVelocity(const Model::VectorkSize& u, const Model::VectorkSize& v);
  void PostDisplay();
  void CheckGLError();

 private:

  std::size_t  grid_size_;
  std::size_t  windows_size_x_;
  std::size_t  windows_size_y_;

};

#endif //RENDERER_H
