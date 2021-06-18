#include "ui.h"

Ui::Ui(int windows_size_x, int windows_size_y)
    : windows_size_x_(windows_size_x), windows_size_y_(windows_size_y) {
  SDL_SetMainReady();
  SDL_Init(SDL_INIT_VIDEO);
  window_ = SDL_CreateWindow("Real Time Fluid Dynamics", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, windows_size_x,
                             windows_size_y, SDL_WINDOW_OPENGL);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  SDL_GLContext context = SDL_GL_CreateContext(window_);
}

bool Ui::HandleEvent(bool running) {
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
  return running;
}

void Ui::SwapWindows() { SDL_GL_SwapWindow(window_); }

Ui::~Ui() { SDL_Quit(); }