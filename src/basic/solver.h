#include <xtensor/xarray.hpp>
#include <xtensor/xfixed.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xtensor.hpp>

#include "basic.h"

constexpr size_t kSize = (grid_size + 2) * (grid_size + 2);

using VectorkSize =
    xt::xtensor_fixed<double, xt::xshape<grid_size + 2, grid_size + 2>>;

void SetBound(std::size_t grid_size, size_t bound, VectorkSize x);

void LinearSolve(std::size_t grid_size, std::size_t bound, VectorkSize x,
                 VectorkSize x0, double a, double c);

void AddSource(std::size_t grid_size, VectorkSize x, VectorkSize s, double dt);

void AddForce(std::size_t grid_size, VectorkSize x, VectorkSize s, double dt);

void Diffuse(std::size_t grid_size, std::size_t bound, VectorkSize x,
             VectorkSize x0, double diff, double dt);

void AddVection(std::size_t grid_size, std::size_t bound, VectorkSize d,
                VectorkSize d0, VectorkSize u, VectorkSize v, double dt);

void Project(std::size_t grid_size, VectorkSize u, VectorkSize v, VectorkSize p,
             VectorkSize div);

void DensityStep(std::size_t grid_size, VectorkSize x, VectorkSize x0,
                 VectorkSize u, VectorkSize v, double diff, double dt);

void VelocityStep(std::size_t grid_size, VectorkSize u, VectorkSize v,
                  VectorkSize u0, VectorkSize v0, double viscosity, double dt);
