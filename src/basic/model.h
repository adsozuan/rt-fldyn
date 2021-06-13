#pragma once

#include <xtensor/xfixed.hpp>
#include <xtensor/xtensor.hpp>

#include "basic.h"

struct Model {
  using VectorkSize =
      xt::xtensor_fixed<double, xt::xshape<kGridSize + 2, kGridSize + 2>>;

  Model(double dt, double diffusion_rate, double viscosity)
      : dt(dt), diffusion_rate(diffusion_rate), viscosity(viscosity) {}

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
