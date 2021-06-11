#include "solver.h"

#include <xtensor/xview.hpp>

void Solver::DensityStep(VectorkSize x, VectorkSize x0,
 VectorkSize u,
                 VectorkSize v, double diff, double dt) {
  AddSource(x, x0, dt);
  // need to swap x, x0
  Diffuse(0, x0, x, diff, dt);
  // need to swap x, x0
  AddVection(0, x, x0, u, v, dt);
}

void Solver::VelocityStep(VectorkSize u, VectorkSize v,
 VectorkSize u0,
                  VectorkSize v0, double viscosity, double dt) {
  AddSource(u, u0, dt);
  AddForce(v, v0, dt);
  // need to swap u, u0
  Diffuse(1, u, u0, viscosity, dt);
  // need to swap v, v0
  Diffuse(2, v, v0, viscosity, dt);
  Project(u, v, u0, v0);
  // need to swap u, u0
  // need to swap v, v0
  AddVection(1, u, u0, u0, v0, dt);
  AddVection(2, v, u0, u0, v0, dt);
  Project(u, v, u0, v0);
}

void Solver::AddSource(VectorkSize x, VectorkSize s, double dt) {
  auto size = grid_size_ + 2;
  xt::view(x, xt::range(0, size)) += dt * xt::view(x, xt::range(0, size));
}

void Solver::AddForce(VectorkSize x, VectorkSize s, double dt) {
  std::size_t size = grid_size_ + 2;
  std::size_t item = 0;

  for (int i = 0; i < size; i++) {
    x(i, i) += dt * s(i, i);
  }
}

void Solver::Diffuse(std::size_t bound, VectorkSize x,
 VectorkSize x0,
             double diff, double dt) {
  double a = dt * diff * grid_size_ * grid_size_;
  LinearSolve(bound, x, x0, a, 1 + 4 * a);
}

void Solver::AddVection(std::size_t bound, VectorkSize d,
 VectorkSize d0,
                VectorkSize u, VectorkSize v, double dt) {
  double dt0 = dt * grid_size_;
  for (size_t i = 1; i < grid_size_ + 1; i++) {
    for (size_t j = 1; j < grid_size_ + 1; j++) {
      auto x = i - dt0 * u(i, j);
      auto y = j - dt0 * v(i, j);
      if (x < 0.5) {
        x = 0.5;
      }
      if (x > grid_size_ + 0.5) {
        x = grid_size_ + 0.5;
      }
      auto i0 = static_cast<int>(std::round(x));
      auto i1 = i0 + 1;

      if (y < 0.5) {
        y = 0.5;
      }
      if (y > grid_size_ + 0.5) {
        y = grid_size_ + 0.5;
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
void Solver::SetBound(size_t bound, VectorkSize x) {
  for (int i = 1; i < grid_size_; i++) {
    if (bound == 1) {
      x(0, i) = -x(1, i);
    } else {
      x(0, i) = x(1, i);
    }
    if (bound == 1) {
      x(grid_size_ + 1, i) = -x(grid_size_, i);
    } else {
      x(grid_size_ + 1, i) = x(grid_size_, i);
    }
    if (bound == 2) {
      x(i, 0) = -x(i, 0);
    } else {
      x(i, 0) = x(i, 1);
    }
    if (bound == 2) {
      x(i, grid_size_ + 1) = -x(i, grid_size_);
    } else {
      x(i, grid_size_ + 1) = x(i, grid_size_);
    }
  }

  x(0, 0) = 0.5 * (x(1, 0) + x(0, 1));
  x(0, grid_size_ + 1) = 0.5 * (x(1, grid_size_ + 1) + x(0, grid_size_));
  x(grid_size_ + 1, 0) = 0.5 * (x(grid_size_, 0) + x(grid_size_ + 1, 1));
  x(grid_size_ + 1, grid_size_ + 1) =
      0.5 * (x(grid_size_, grid_size_ + 1) + x(grid_size_ + 1, grid_size_));
}

void Solver::LinearSolve(std::size_t bound, VectorkSize x,
 VectorkSize x0,
                 double a, double c) {
  // TODO DEBUG HERE incompatible size array exception from xtensor

  for (int k = 0; k < 20; k++) {
    auto b = x0[1] + a;
    xt::view(x, xt::range(1, grid_size_)) =
        xt::view(x0, xt::range(0, grid_size_)) +
        a *
            (xt::view(x, xt::range(0, grid_size_)) +
             xt::view(x, xt::range(2, grid_size_)) +
             xt::view(x, xt::range(1, grid_size_)) +
             xt::view(x, xt::range(1, grid_size_ + 2))) /
            c;

    SetBound(bound, x);
  }
}


void Solver::Project(VectorkSize u, VectorkSize v, VectorkSize p,
 VectorkSize div) {
  auto h = 1.0 / grid_size_;
  xt::view(div, xt::range(1, grid_size_ + 1), xt::range(1, grid_size_ + 1)) =
      (-0.5 * h *
           xt::view(u, xt::range(2, grid_size_ + 2),
                    xt::range(1, grid_size_ + 1)) -
       xt::view(u, xt::range(0, grid_size_), xt::range(1, grid_size_ + 1)) +
       xt::view(v, xt::range(1, grid_size_ + 1), xt::range(2, grid_size_ + 2)) -
       xt::view(v, xt::range(1, grid_size_ + 1), xt::range(0, grid_size_)));

  xt::view(p, xt::range(1, grid_size_ + 1), xt::range(1, grid_size_ + 1)) = 0;

  SetBound(0, div);
  SetBound(0, p);
  LinearSolve(0, p, div, 1, 4);

  xt::view(u, xt::range(1, grid_size_ + 1), xt::range(1, grid_size_ + 1)) -=
      0.5 *
      (xt::view(p, xt::range(2, grid_size_ + 2), xt::range(1, grid_size_ + 1)) -
       xt::view(p, xt::range(0, grid_size_), xt::range(1, grid_size_ + 1))) /
      h;

  xt::view(v, xt::range(1, grid_size_ + 1), xt::range(1, grid_size_ + 1)) -=
      0.5 *
      (xt::view(p, xt::range(1, grid_size_ + 1), xt::range(2, grid_size_ + 2)) -
       xt::view(p, xt::range(1, grid_size_ + 2), xt::range(0, grid_size_))) /
      h;

  SetBound(1, u);
  SetBound(2, v);
}
