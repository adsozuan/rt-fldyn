#ifndef UI_H
#define UI_H

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <glbinding/glbinding.h>
#include <glbinding/gl/gl.h>


struct UiEvent {
  bool quit;
  bool left_click;
  bool right_click;
  bool reset;
  int grid_x;
  int grid_y;
  int dmouse_x;
  int dmouse_y;
};


//@class Ui
//@brief handle SDL windows, and implements RAII for resource freeing.
class Ui {
 public:
  Ui(int windows_size_x, int windows_size_y, size_t grid_size);
  void RenderUi();
  UiEvent HandleEvent();
  void SwapWindows();
  ~Ui(); 

 private:
  std::tuple<int, int> MouseCoordToGridCoord(int mouse_x, int mouse_y);

  int window_size_x_;
  int window_size_y_;
  size_t grid_size_; 
  SDL_Window* window_;
  SDL_GLContext context_;
  int mouse_x_;
  int mouse_y_;
};

#endif  // UI_H
