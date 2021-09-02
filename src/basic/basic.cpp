// basic.cpp : Defines the entry point for the application.
//
//#define GL_GLEXT_PROTOTYPES
//#define SDL_MAIN_HANDLED

#include <iostream>
#include <tuple>

#include "renderer.h"
#include "solver.h"
#include "ui.h"

using namespace std;

int main(int argc, char* argv[]) {
  try {
    std::size_t windows_size_x = 1024;
    std::size_t windows_size_y = 1024;

    double dt = 0.016;

    double diffusion_rate = 0.1;
    double viscosity = 0.1;
    double force = 0.005;
    double source = 100.0;

    Solver solver(dt, diffusion_rate, viscosity);
    Ui ui(windows_size_x, windows_size_y, solver.grid_size());
    Renderer renderer(solver.grid_size(), windows_size_x, windows_size_y);

    bool running = true;
    UiEvent ui_event{};
    while (running) {
      ui_event = ui.HandleEvent();
      //ui.RenderUi();
      running = !ui_event.quit;

      // Apply UI events to simulation
      if (ui_event.left_click) {
         solver.ApplyForceAtPoint(ui_event.settings.force, ui_event.grid_x, ui_event.grid_y,
                                 ui_event.dmouse_x, ui_event.dmouse_y);
        //solver.AddVelocity(force, force, ui_event.grid_x, ui_event.grid_y);
      }
      if (ui_event.right_click) {
        solver.ApplySourceAtPoint(ui_event.settings.source, ui_event.grid_x, ui_event.grid_y);
      }

      if (ui_event.reset) {
        solver.Reset();
        solver.SetDiffustionRate(ui_event.settings.diffusion_rate);
        solver.SetViscosity(ui_event.settings.viscosity);
        std::cout << "diff: " << ui_event.settings.diffusion_rate << "\n";
      }

      // Move simulation one step
      solver.ResetPreviousVelocity();
      solver.VelocityStep();
      solver.DensityStep();
      //solver.OneStep();

      // Render everything
      //renderer.Display(solver.density(), solver.u_velocity(),
      //                 solver.v_velocity());

      ui.RenderUi(solver.density());

    }

  } catch (const std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
  }

  return 0;
}
