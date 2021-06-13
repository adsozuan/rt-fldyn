#pragma once

#include "model.h"

class Renderer {
 public:
  Renderer(std::size_t windows_size_x, std::size_t windows_size_y)
      : windows_size_x_(windows_size_x), windows_size_y_(windows_size_y) {}
 
  void Display(const Model::VectorkSize& density, const Model::VectorkSize& u,
               const Model::VectorkSize& v);

 private:
  void PreDisplay();
  void DrawDensity(const Model::VectorkSize& density);
  void DrawVelocity(const Model::VectorkSize& u, const Model::VectorkSize& v);
  void PostDisplay();

 private:
  std::size_t  windows_size_x_;
  std::size_t  windows_size_y_;
};
