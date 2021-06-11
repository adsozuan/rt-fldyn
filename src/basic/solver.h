#include <xtensor/xarray.hpp>
#include <xtensor/xfixed.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xtensor.hpp>

#include "basic.h"

class Solver {
 public:
  Solver(std::size_t grid_size) : grid_size_{grid_size} {};

  using VectorkSize =
      xt::xtensor_fixed<double, xt::xshape<kGridSize + 2, kGridSize + 2>>;

  void DensityStep(VectorkSize x, VectorkSize x0, VectorkSize u, VectorkSize v,
                   double diff, double dt);

  void VelocityStep(VectorkSize u, VectorkSize v, VectorkSize u0,
                    VectorkSize v0, double viscosity, double dt);

 private:
  void SetBound(size_t bound, VectorkSize x);

  void LinearSolve(std::size_t bound, VectorkSize x, VectorkSize x0, double a,
                   double c);

  void AddSource(VectorkSize x, VectorkSize s, double dt);

  void AddForce(VectorkSize x, VectorkSize s, double dt);

  void Diffuse(std::size_t bound, VectorkSize x, VectorkSize x0, double diff,
               double dt);

  void AddVection(std::size_t bound, VectorkSize d, VectorkSize d0,
                  VectorkSize u, VectorkSize v, double dt);

  void Project(VectorkSize u, VectorkSize v, VectorkSize p, VectorkSize div);

  std::size_t grid_size_;
};
