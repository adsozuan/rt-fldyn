#ifndef MODEL_H
#define MODEL_H

#include <xtensor/xfixed.hpp>
#include <xtensor/xtensor.hpp>

#include "basic.h"

// TODO must fill all vectors with 0.0

struct Model {
  using VectorkSize =
      xt::xtensor_fixed<double, xt::xshape<kGridSize + 2, kGridSize + 2>>;


  Model(double dt, double diffusion_rate, double viscosity)
      : dt(dt), diffusion_rate(diffusion_rate), viscosity(viscosity) {

      Clear();
 }

  void Clear(){
    u.fill(0.0);
    v.fill(0.0);
    u_previous.fill(0.0);
    v_previous.fill(0.0);
    density.fill(0.0);
    density_previous.fill(0.0);
  };


  double dt;
  double diffusion_rate;
  double viscosity;
  VectorkSize u = {};
  VectorkSize v = {};
  VectorkSize u_previous = {};
  VectorkSize v_previous = {};
  VectorkSize density = {};
  VectorkSize density_previous = {};
};

#endif //MODEL_H
