// basic.cpp : Defines the entry point for the application.
//
#define GL_GLEXT_PROTOTYPES
#define SDL_MAIN_HANDLED

#include "basic.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>

#include "solver.h"
#include "renderer.h"

using namespace std;
using VectorkSize = Solver::VectorkSize;




int main(int argc, char* argv[]) {
  try {
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);


    std::cout << "grid size: " << kGridSize << '\n';

    double dt = 0.1;

    double diffusion_rate = 0.0;
    double viscosity = 0.0;
    double force = 5.0;
    double source = 100.0;

    Solver solver(kGridSize, dt, diffusion_rate, viscosity);

    std::size_t windows_size_x = 512;
    std::size_t windows_size_y = 512;

    SDL_Window* window =
        SDL_CreateWindow("Real Time Fluid Dynamics", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, windows_size_x, windows_size_y,
                         SDL_WINDOW_OPENGL);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GLContext context = SDL_GL_CreateContext(window);

    Renderer renderer;

    bool running = true;
    while (running) {
      SDL_Event event;

      while (SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_KEYDOWN:
            /* Handle key presses. */
            break;
          case SDL_QUIT:
            running = false;
            break;
          case SDLK_ESCAPE:
            running = false;
            break;
        }
      }
      //std::cout << u << '\n';
      //std::cout << v << '\n';
      //std::cout << density << '\n';

      solver.VelocityStep();
      solver.DensityStep();

      renderer.Display(solver.density(), solver.u_velocity(), solver.v_velocity());

      SDL_GL_SwapWindow(window);
    }
  } catch (const std::exception& e) {
    std::cout << "ERROR: " << e.what() << std::endl;
  }

  SDL_Quit();

  return 0;
}
