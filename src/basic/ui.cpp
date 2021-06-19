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

UiEvent Ui::HandleEvent() {
  SDL_Event event;
  UiEvent ui_event{};

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      
      case SDL_KEYDOWN:
        /* Handle key presses. */
        break;
      case SDL_QUIT:
        ui_event.quit = true;
        break;
      case SDLK_ESCAPE:
        ui_event.quit = true;
        break;
      case SDL_MOUSEBUTTONDOWN:
        auto b = event.button;
        int x, y;
        SDL_GetMouseState(&ui_event.mouse_x, &ui_event.mouse_y);
        std::cout << "mouse x: " << ui_event.mouse_x
                  << ", y: " << ui_event.mouse_y << '\n';
        if (b.button == SDL_BUTTON_LEFT) {
          ui_event.left_click = true;
        }
        if (b.button == SDL_BUTTON_RIGHT) {
          ui_event.left_click = false;
        }
        break;
    }
  }
  return ui_event;
}

void Ui::SwapWindows() { SDL_GL_SwapWindow(window_); }

Ui::~Ui() { SDL_Quit(); }