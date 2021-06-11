#include "solver.h"

#include <xtensor/xview.hpp>

void SetBound(std::size_t grid_size, size_t bound, VectorkSize x) {
  for (int i = 1; i < grid_size; i++) {
    if (bound == 1) {
      x(0, i) = -x(1, i);
    } else {
      x(0, i) = x(1, i);
    }
    if (bound == 1) {
      x(grid_size + 1, i) = -x(grid_size, i);
    } else {
      x(grid_size + 1, i) = x(grid_size, i);
    }
    if (bound == 2) {
      x(i, 0) = -x(i, 0);
    } else {
      x(i, 0) = x(i, 1);
    }
    if (bound == 2) {
      x(i, grid_size + 1) = -x(i, grid_size);
    } else {
      x(i, grid_size + 1) = x(i, grid_size);
    }
  }

  x(0, 0) = 0.5 * (x(1, 0) + x(0, 1));
  x(0, grid_size + 1) = 0.5 * (x(1, grid_size + 1) + x(0, grid_size));
  x(grid_size + 1, 0) = 0.5 * (x(grid_size, 0) + x(grid_size + 1, 1));
  x(grid_size + 1, grid_size + 1) =
      0.5 * (x(grid_size, grid_size + 1) + x(grid_size + 1, grid_size));
}

void LinearSolve(std::size_t grid_size, std::size_t bound, VectorkSize x,
                 VectorkSize x0, double a, double c) {
  // TODO DEBUG HERE incompatible size array exception from xtensor

  for (int k = 0; k < 20; k++) {
    auto b = x0[1] + a;
    xt::view(x, xt::range(1, grid_size)) =
        xt::view(x0, xt::range(0, grid_size)) +
        a *
            (xt::view(x, xt::range(0, grid_size)) +
             xt::view(x, xt::range(2, grid_size)) +
             xt::view(x, xt::range(1, grid_size)) +
             xt::view(x, xt::range(1, grid_size + 2))) /
            c;

    SetBound(grid_size, bound, x);
  }
}

void AddSource(std::size_t grid_size, VectorkSize x, VectorkSize s, double dt) {
  auto size = grid_size + 2;
  xt::view(x, xt::range(0, size)) += dt * xt::view(x, xt::range(0, size));
}

void AddForce(std::size_t grid_size, VectorkSize x, VectorkSize s, double dt) {
  std::size_t size = grid_size + 2;
  std::size_t item = 0;

  for (int i = 0; i < size; i++) {
    x(i, i) += dt * s(i, i);
  }
}

void Diffuse(std::size_t grid_size, std::size_t bound, VectorkSize x,
             VectorkSize x0, double diff, double dt) {
  double a = dt * diff * grid_size * grid_size;
  LinearSolve(grid_size, bound, x, x0, a, 1 + 4 * a);
}

void AddVection(std::size_t grid_size, std::size_t bound, VectorkSize d,
                VectorkSize d0, VectorkSize u, VectorkSize v, double dt) {
  double dt0 = dt * grid_size;
  for (size_t i = 1; i < grid_size + 1; i++) {
    for (size_t j = 1; j < grid_size + 1; j++) {
      auto x = i - dt0 * u(i, j);
      auto y = j - dt0 * v(i, j);
      if (x < 0.5) {
        x = 0.5;
      }
      if (x > grid_size + 0.5) {
        x = grid_size + 0.5;
      }
      auto i0 = static_cast<int>(std::round(x));
      auto i1 = i0 + 1;

      if (y < 0.5) {
        y = 0.5;
      }
      if (y > grid_size + 0.5) {
        y = grid_size + 0.5;
      }
      auto j0 = static_cast<int>(std::round(y));
      auto j1 = j0 + 1;
      auto s1 = x - i0;
      auto s0 = 1 - s1;
      auto t1 = y - j0;
      auto t0 = 1 - t1;
      d(i, j) = (s0 * (t0 * d0(i0, j0) + t1 * d0(i0, j1)) +
                 s1 * (t0 * d0(i1, j0) + t1 * d0(i1, j1)));
    }
  }
}

void Project(std::size_t grid_size, VectorkSize u, VectorkSize v, VectorkSize p,
             VectorkSize div) {
  auto h = 1.0 / grid_size;
  xt::view(div, xt::range(1, grid_size + 1), xt::range(1, grid_size + 1)) =
      (-0.5 * h *
           xt::view(u, xt::range(2, grid_size + 2),
                    xt::range(1, grid_size + 1)) -
       xt::view(u, xt::range(0, grid_size), xt::range(1, grid_size + 1)) +
       xt::view(v, xt::range(1, grid_size + 1), xt::range(2, grid_size + 2)) -
       xt::view(v, xt::range(1, grid_size + 1), xt::range(0, grid_size)));

  xt::view(p, xt::range(1, grid_size + 1), xt::range(1, grid_size + 1)) = 0;

  SetBound(grid_size, 0, div);
  SetBound(grid_size, 0, p);
  LinearSolve(grid_size, 0, p, div, 1, 4);

  xt::view(u, xt::range(1, grid_size + 1), xt::range(1, grid_size + 1)) -=
      0.5 *
      (xt::view(p, xt::range(2, grid_size + 2), xt::range(1, grid_size + 1)) -
       xt::view(p, xt::range(0, grid_size), xt::range(1, grid_size + 1))) /
      h;

  xt::view(v, xt::range(1, grid_size + 1), xt::range(1, grid_size + 1)) -=
      0.5 *
      (xt::view(p, xt::range(1, grid_size + 1), xt::range(2, grid_size + 2)) -
       xt::view(p, xt::range(1, grid_size + 2), xt::range(0, grid_size))) /
      h;

  SetBound(grid_size, 1, u);
  SetBound(grid_size, 2, v);
}

void DensityStep(std::size_t grid_size, VectorkSize x, VectorkSize x0,
                 VectorkSize u, VectorkSize v, double diff, double dt) {
  AddSource(grid_size, x, x0, dt);
  // need to swap x, x0
  Diffuse(grid_size, 0, x0, x, diff, dt);
  // need to swap x, x0
  AddVection(grid_size, 0, x, x0, u, v, dt);
}

void VelocityStep(std::size_t grid_size, VectorkSize u, VectorkSize v,
                  VectorkSize u0, VectorkSize v0, double viscosity, double dt) {
  AddSource(grid_size, u, u0, dt);
  AddForce(grid_size, v, v0, dt);
  // need to swap u, u0
  Diffuse(grid_size, 1, u, u0, viscosity, dt);
  // need to swap v, v0
  Diffuse(grid_size, 2, v, v0, viscosity, dt);
  Project(grid_size, u, v, u0, v0);
  // need to swap u, u0
  // need to swap v, v0
  AddVection(grid_size, 1, u, u0, u0, v0, dt);
  AddVection(grid_size, 2, v, u0, u0, v0, dt);
  Project(grid_size, u, v, u0, v0);
}