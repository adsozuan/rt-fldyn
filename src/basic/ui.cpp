#include "ui.h"

#include <algorithm>

Ui::Ui(int windows_size_x, int windows_size_y, std::size_t grid_size)
    : window_size_x_(windows_size_x),
      window_size_y_(windows_size_y),
      grid_size_(grid_size) {
  SDL_SetMainReady();
  SDL_Init(SDL_INIT_VIDEO);
  window_ =
      SDL_CreateWindow("Real Time Fluid Dynamics", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, windows_size_x, windows_size_y,
                       SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  context_ = SDL_GL_CreateContext(window_);
  if (!context_) {
    std::cout << "OpenGL could not be created. SDL ERROR: " << SDL_GetError()
              << std::endl;
  }

  SDL_GL_SetSwapInterval(1);
  mouse_x_ = {};
  mouse_y_ = {};
}

UiEvent Ui::HandleEvent() {
  SDL_Event event;
  UiEvent ui_event{};
  std::tuple<int, int> grid_coord{};

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
        grid_coord = MouseCoordToGridCoord(x, y);
        ui_event.grid_x = std::get<0>(grid_coord);
        ui_event.grid_y = std::get<1>(grid_coord);
        ui_event.dmouse_x = x - mouse_x_;
        ui_event.dmouse_y = mouse_y_ - y;

        mouse_x_ = x;
        mouse_y_ = y;

        if (b.button == SDL_BUTTON_LEFT) {
          ui_event.left_click = true;
          ui_event.right_click = false;
        }
        if (b.button == SDL_BUTTON_RIGHT) {
          ui_event.left_click = false;
          ui_event.right_click = true;
        }
        break;
      default:
        break;
    }
  }
  return ui_event;
}

void Ui::SwapWindows() { SDL_GL_SwapWindow(window_); }

std::tuple<int, int> Ui::MouseCoordToGridCoord(int mouse_x, int mouse_y) {
  auto grid_x = static_cast<int>(
      (mouse_x / static_cast<float>(window_size_x_)) * grid_size_ + 1);
  auto grid_y =
      static_cast<int>(((window_size_y_ - static_cast<float>(mouse_y)) /
                        static_cast<float>(window_size_y_)) *
                           static_cast<float>(grid_size_) +
                       1.0);
  grid_x = std::clamp(grid_x, 1, static_cast<int>(grid_size_));
  grid_y = std::clamp(grid_y, 1, static_cast<int>(grid_size_));
  std::cout << "grid_y :" << grid_x << ", grid_y: " << grid_y << "\n";

  return std::tuple<int, int>{grid_x, grid_y};
}

Ui::~Ui() { SDL_Quit(); }