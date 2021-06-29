// basic.cpp : Defines the entry point for the application.
//
#define GL_GLEXT_PROTOTYPES
#define SDL_MAIN_HANDLED

#include "basic.h"

#include <iostream>
#include <tuple>

#include "renderer.h"
#include "solver.h"
#include "ui.h"

using namespace std;

int main(int argc, char* argv[]) {
  try {
    std::size_t windows_size_x = 512;
    std::size_t windows_size_y = 512;

    Ui ui(windows_size_x, windows_size_y, kGridSize);

    std::cout << "grid size: " << kGridSize << '\n';

    double dt = 0.016;

    double diffusion_rate = 0.1;
    double viscosity = 0.1;
    double force = 5.0;
    double source = 10.0;

    Solver solver(kGridSize, dt, diffusion_rate, viscosity);
    Renderer renderer(windows_size_x, windows_size_y);

    bool running = true;
    UiEvent ui_event{};
    while (running) {
      ui_event = ui.HandleEvent();
      running = !ui_event.quit;

      // Apply UI events to simulation
      if (ui_event.left_click) {
        solver.ApplyForceAtPoint(force, ui_event.grid_x, ui_event.grid_y,
                                 ui_event.dmouse_x, ui_event.dmouse_y);
      }
      if (ui_event.right_click) {
        solver.ApplySourceAtPoint(source, ui_event.grid_x, ui_event.grid_y);
      }

      if (ui_event.reset) {
        solver.Reset();
      }

      // Move simulation one step
      solver.VelocityStep();
      solver.DensityStep();


      // Render everything
      renderer.Display(solver.density(), solver.u_velocity(),
                       solver.v_velocity());

      ui.SwapWindows();
    }

  } catch (const std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
  }

  return 0;
}
