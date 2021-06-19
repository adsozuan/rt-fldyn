#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <iostream>


struct UiEvent {
  bool quit;
  bool left_click;
  bool right_click;
  int grid_x;
  int grid_y;
};


//@class Ui
//@brief handle SDL windows, and implements RAII for resource freeing.
class Ui {
 public:
  Ui(int windows_size_x, int windows_size_y, size_t grid_size);
  UiEvent HandleEvent();
  void SwapWindows();
  ~Ui(); 

 private:
  std::tuple<int, int> MouseCoordToGridCoord(int mouse_x, int mouse_y);

  int window_size_x_;
  int window_size_y_;
  size_t grid_size_; 
  SDL_Window* window_;
};

#endif  // UI_H
