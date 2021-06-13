#pragma once

#include <xtensor/xfixed.hpp>
#include <xtensor/xtensor.hpp>

#include "basic.h"

struct Model {
  using VectorkSize =
      xt::xtensor_fixed<double, xt::xshape<kGridSize + 2, kGridSize + 2>>;

  VectorkSize u = {};
  VectorkSize v = {};
  VectorkSize u_previous = {};
  VectorkSize v_previous = {};
  VectorkSize density = {};
  VectorkSize density_previous = {};
};
