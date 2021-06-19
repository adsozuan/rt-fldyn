#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <iostream>


struct UiEvent {
  bool quit;
  bool left_click;
  bool right_click;
  int mouse_x;
  int mouse_y;
};


//@class Ui
//@brief handle SDL windows, and implements RAII for resource freeing.
class Ui {
 public:
  Ui(int windows_size_x, int windows_size_y);
  UiEvent HandleEvent();
  void SwapWindows();
  ~Ui(); 

 private:
  int windows_size_x_;
  int windows_size_y_;
  SDL_Window* window_;
};

#endif  // UI_H
