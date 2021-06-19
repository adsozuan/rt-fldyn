#ifndef SOLVER_H
#define SOLVER_H

#include <xtensor/xarray.hpp>
#include <xtensor/xfixed.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xtensor.hpp>

#include "basic.h"
#include "model.h"

class Solver {
 public:
  Solver(std::size_t grid_size, double dt, double diffusion_rate,
         double viscosity)
      : grid_size_(grid_size), model_(dt, diffusion_rate, viscosity) {
    grid_spacing_ = 1.0 / grid_size_;
  }

  using VectorkSize =
      xt::xtensor_fixed<double, xt::xshape<kGridSize + 2, kGridSize + 2>>;

  void DensityStep();
  void VelocityStep();
  void ApplyForceAtPoint(double force, int x, int y);
  void ApplySourceAtPoint(double source, int x, int y);

  const VectorkSize& density() { return model_.density; }
  const VectorkSize& u_velocity() { return model_.u; }
  const VectorkSize& v_velocity() { return model_.v; }

 private:
  void SetBound(size_t bound, VectorkSize& x);

  void LinearSolve(std::size_t bound, VectorkSize& x, VectorkSize& x0, double a,
                   double c);

  void AddSource(VectorkSize& x, VectorkSize& s, double dt);

  void AddForce(VectorkSize& x, VectorkSize& s, double dt);

  void Diffuse(std::size_t bound, VectorkSize& x, VectorkSize& x0,
               double diffusion_rate, double dt);

  void AddVection(std::size_t bound, VectorkSize& d, VectorkSize& d0,
                  VectorkSize& u, VectorkSize& v, double dt);

  void Project(VectorkSize& u, VectorkSize& v, VectorkSize& p,
               VectorkSize& div);

 private:
  std::size_t grid_size_;
  double grid_spacing_;
  Model model_;
};

#endif  // SOLVER_H
