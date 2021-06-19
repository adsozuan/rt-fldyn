#include "ui.h"

Ui::Ui(int windows_size_x, int windows_size_y, std::size_t grid_size)
    : window_size_x_(windows_size_x),
      window_size_y_(windows_size_y),
      grid_size_(grid_size) {
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
        SDL_GetMouseState(&x, &y);
        auto grid_coord = MouseCoordToGridCoord(x, y);
        ui_event.grid_x = std::get<0>(grid_coord);
        ui_event.grid_y = std::get<1>(grid_coord);

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

std::tuple<int, int> Ui::MouseCoordToGridCoord(int mouse_x, int mouse_y) {
  auto grid_x = static_cast<int>(
      mouse_x / static_cast<float>(window_size_x_) * grid_size_ + 1);
  auto grid_y = static_cast<int>(
      (window_size_y_ - static_cast<float>(mouse_y)) /
          static_cast<float>(window_size_y_) * static_cast<float>(grid_size_) +
      1.0);
  if (grid_x < 1) {
    grid_x = 1;
  }
  if (grid_x > grid_size_) {
    grid_x = grid_size_;
  }
  if (grid_y < 1) {
    grid_y = 1;
  }
  if (grid_y > grid_size_) {
    grid_y = grid_size_;
  }
  return std::tuple<int, int>{grid_x, grid_y};
}
Ui::~Ui() { SDL_Quit(); }