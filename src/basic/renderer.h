#pragma once

#include "model.h"

class Renderer {
 public:
  void Display(const Model::VectorkSize& density, const Model::VectorkSize& u,
               const Model::VectorkSize& v);

 private:
  void PreDisplay();
  void DrawDensity(const Model::VectorkSize& density);
  void DrawVelocity(const Model::VectorkSize& u, const Model::VectorkSize& v);
  void PostDisplay();
};
