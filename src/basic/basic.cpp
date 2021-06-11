// basic.cpp : Defines the entry point for the application.
//
#define GL_GLEXT_PROTOTYPES
#define SDL_MAIN_HANDLED

#include "basic.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <iostream>

#include "solver.h"

using namespace std;

void DrawDensity(VectorkSize& dens) {
  double h = 1.0 / grid_size;

  glBegin(GL_QUADS);
  for (int i = 0; i < grid_size + 1; i++) {
    double x = (i - 0.5) * h;
    for (int j = 0; j < grid_size + 1; j++) {
      double y = (j - 0.5) * h;
      auto d00 = dens(i, j);
      auto d01 = dens(i, j + 1);
      auto d10 = dens(i + 1, j);
      auto d11 = dens(i + 1, j + 1);

      glColor3f(d00, d00, d00);
      glVertex2f(x, y);
      glColor3f(d10, d10, d10);
      glVertex2f(x + h, y);
      glColor3f(d11, d11, d11);
      glVertex2f(x + h, y + h);
      glColor3f(d01, d01, d01);
      glVertex2f(x, y + h);
    }
  }
  glEnd();
}

void DrawVelocity() { double h = 1.0 / grid_size; }

void PreDisplay() {
  glViewport(0, 0, 512, 512);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 1.0, 0.0, 1.0);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}
void PostDisplay() {}

void Display(VectorkSize& density, VectorkSize& velocity) {
  PreDisplay();
  DrawDensity(density);
  DrawVelocity();
  PostDisplay();
}

int main(int argc, char* argv[]) {
  try {
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    std::cout << "size: " << kSize << '\n';
    std::cout << "grid size: " << grid_size << '\n';

    VectorkSize u = {};
    VectorkSize v = {};
    VectorkSize u_previous = {};
    VectorkSize v_previous = {};
    VectorkSize density = {};
    VectorkSize density_previous = {};

    double dt = 0.1;
    double diff = 0.0;
    double viscosity = 0.0;
    double force = 5.0;
    double source = 100.0;

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
      std::cout << u << '\n';
      std::cout << v << '\n';
      std::cout << density << '\n';
      glViewport(0, 0, windows_size_x, windows_size_y);
      glClearColor(1.f, 0.f, 1.f, 0.f);
      glClear(GL_COLOR_BUFFER_BIT);
      // VelocityStep(grid_size, u, v, u_previous, v_previous, viscosity, dt);
      DensityStep(grid_size, density, density_previous, u, v, diff, dt);
      Display(density, v);

      SDL_GL_SwapWindow(window);
    }
  } catch (const std::exception& e) {
    std::cout <<"ERROR: " << e.what() << std::endl;
  }

  SDL_Quit();

  return 0;
}
