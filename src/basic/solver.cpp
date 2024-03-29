#include "solver.h"

#include <algorithm>
#include <xtensor/xview.hpp>

Solver::Solver(double dt, double diffusion_rate, double viscosity)
    : grid_size_(model_.kGridSize), model_(dt, diffusion_rate, viscosity) {
  grid_spacing_ = 1.0 / model_.kGridSize;
}

void Solver::OneStep() {
  Diffuse(1, model_.u0, model_.u, model_.viscosity, model_.dt);
  Diffuse(2, model_.v0, model_.v, model_.viscosity, model_.dt);

  Project(model_.u0, model_.v0, model_.u, model_.v);

  AddVection(1, model_.u, model_.u0, model_.u0, model_.v0, model_.dt);
  AddVection(2, model_.v, model_.v0, model_.u0, model_.v0, model_.dt);

  Project(model_.u, model_.v, model_.u0, model_.v0);

  Diffuse(0, model_.density0, model_.density, model_.diffusion_rate, model_.dt);
  AddVection(0, model_.density, model_.density0, model_.u, model_.v, model_.dt);
}

void Solver::DensityStep() {
  AddSource(model_.density, model_.density0, model_.dt);

  std::swap(model_.density0, model_.density);
  Diffuse(0, model_.density, model_.density0, model_.diffusion_rate, model_.dt);

  std::swap(model_.density0, model_.density);
  AddVection(0, model_.density, model_.density0, model_.u, model_.v, model_.dt);
}

void Solver::VelocityStep() {
  AddSource(model_.u, model_.u0, model_.dt);
  AddSource(model_.v, model_.v0, model_.dt);
  std::swap(model_.u0, model_.u);
  Diffuse(1, model_.u, model_.u0, model_.viscosity, model_.dt);

  std::swap(model_.v0, model_.v);
  Diffuse(2, model_.v, model_.v0, model_.viscosity, model_.dt);
  Project(model_.u, model_.v, model_.u0, model_.v0);

  std::swap(model_.u0, model_.u);
  std::swap(model_.v0, model_.v);

  AddVection(2, model_.v, model_.v0, model_.u0, model_.v0, model_.dt);
  AddVection(1, model_.u, model_.u0, model_.u0, model_.v0, model_.dt);
  Project(model_.u, model_.v, model_.u0, model_.v0);
}

void Solver::AddVelocity(double amount_x, double amount_y, int x, int y) {
  model_.u(x, y) += amount_x;
  model_.v(x, y) += amount_y;
}

void Solver::ApplyForceAtPoint(double force, int x, int y, int dmx, int dmy) {
  std::cout << "force: " << force << ", x: " << x << ", y: " << y
            << ", dmx: " << dmx << ", dmy: " << dmy << '\n';
  model_.u(x, y) = force * dmx;
  model_.v(x, y) = force * dmy;
}

void Solver::ApplySourceAtPoint(double source, int x, int y) {
  model_.density(x, y) += source;
}

void Solver::Reset() { model_.Clear(); }

void Solver::ResetPreviousVelocity() {
  model_.u0.fill(0.0);
  model_.v0.fill(0.0);
}

void Solver::AddSource(VectorkSize& x, VectorkSize& s, double dt) {
  auto size = grid_size_ + 2;
  xt::view(x, xt::range(0, size), xt::range(0, size)) +=
      dt * xt::view(s, xt::range(0, size), xt::range(0, size));
}

void Solver::AddForce(VectorkSize& x, VectorkSize& s, double dt) {
  std::size_t size = grid_size_ + 2;
  std::size_t item = 0;

  for (int i = 0; i < size; i++) {
    x(i, i) += dt * s(i, i);
  }
}

void Solver::Diffuse(std::size_t bound, VectorkSize& x, VectorkSize& x0,
                     double diffusion_rate, double dt) {
  double a = dt * diffusion_rate * (grid_size_-2) * (grid_size_ -2);
  LinearSolve(bound, x, x0, a, 1 + 4 * a);
}

void Solver::AddVection(std::size_t bound, VectorkSize& d, VectorkSize& d0,
                        VectorkSize& u, VectorkSize& v, double dt) {
  double dt0 = dt * grid_size_;
  for (size_t i = 1; i <= grid_size_; i++) {
    for (size_t j = 1; j <= grid_size_; j++) {
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

      // must clamp here, otherwise we push outside of bounds
      i = std::clamp(static_cast<int>(i), 1, static_cast<int>(grid_size_));
      j = std::clamp(static_cast<int>(j), 1, static_cast<int>(grid_size_));
      i0 = std::clamp(static_cast<int>(i0), 1, static_cast<int>(grid_size_));
      j0 = std::clamp(static_cast<int>(j0), 1, static_cast<int>(grid_size_));
      i1 = std::clamp(static_cast<int>(i1), 1, static_cast<int>(grid_size_));
      j1 = std::clamp(static_cast<int>(j1), 1, static_cast<int>(grid_size_));
      d(i, j) = (s0 * (t0 * d0(i0, j0) + t1 * d0(i0, j1)) +
                 s1 * (t0 * d0(i1, j0) + t1 * d0(i1, j1)));
    }
  }
  SetBound(bound, d);
}

void Solver::SetBound(size_t bound, VectorkSize& x) {
  // TODO Check if all tests against NaN can be avoided
  for (int i = 1; i < grid_size_ + 1; i++) {
    if (bound == 1) {
      auto value = -x(1, i);
      if (std::isnan(value)) {
        x(0, i) = 0.0;
      } else {
        x(0, i) = value;
      }
    } else {
      x(0, i) = x(1, i);
    }

    if (bound == 1) {
      auto value = -x(grid_size_, i);
      if (std::isnan(value)) {
        x(grid_size_ + 1, i) = 0.0;
      } else {
        x(grid_size_ + 1, i) = value;
      }
    } else {
      x(grid_size_ + 1, i) = x(grid_size_, i);
    }

    if (bound == 2) {
      auto value = -x(i, 1);
      if (std::isnan(value)) {
        x(i, 0) = 0.0;
      } else {
        x(i, 0) = value;
      }
    } else {
      x(i, 0) = x(i, 1);
    }

    if (bound == 2) {
      auto value = -x(i, grid_size_);
      if (std::isnan(value)) {
        x(i, grid_size_ + 1) = 0.0;
      } else {
        x(i, grid_size_ + 1) = value;
      }
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

void Solver::LinearSolve(std::size_t bound, VectorkSize& x, VectorkSize& x0,
                         double a, double c) {
  for (int k = 0; k < 20; k++) {
    xt::view(x, xt::range(1, grid_size_ + 1), xt::range(1, grid_size_ + 1)) =
        (xt::view(x0, xt::range(1, grid_size_ + 1),
                  xt::range(1, grid_size_ + 1)) +
         a * (xt::view(x, xt::range(0, grid_size_),
                       xt::range(1, grid_size_ + 1)) +
              xt::view(x, xt::range(2, grid_size_ + 2),
                       xt::range(1, grid_size_ + 1)) +
              xt::view(x, xt::range(1, grid_size_ + 1),
                       xt::range(0, grid_size_)) +
              xt::view(x, xt::range(1, grid_size_ + 1),
                       xt::range(2, grid_size_ + 2)))) /
        c;

    SetBound(bound, x);
  }
}

void Solver::Project(VectorkSize& u, VectorkSize& v, VectorkSize& p,
                     VectorkSize& div) {
  for (size_t i = 1; i <= grid_size_; i++) {
    for (size_t j = 1; j <= grid_size_; j++) {
      div(i, j) = -0.5 *
                  (u(i + 1, j) - u(i - 1, j) + v(i, j + 1) - v(i, j - 1)) /
                  grid_size_;
      p(i, j) = 0;
    }
  }

  SetBound(0, div);
  SetBound(0, p);
  LinearSolve(0, p, div, 1, 4);

  for (size_t i = 1; i <= grid_size_; i++) {
    for (size_t j = 1; j <= grid_size_; j++) {
      u(i, j) -= 0.5 * grid_size_ * (p(i + 1, j) - p(i - 1, j));
      v(i, j) -= 0.5 * grid_size_ * (p(i, j + 1) - p(i, j - 1));
    }
  }

  SetBound(1, u);
  SetBound(2, v);
}
