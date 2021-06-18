#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>


//@class Ui
//@brief handle SDL windows, and implements RAII for resource freeing.
class Ui {
 public:
  Ui(int windows_size_x, int windows_size_y);
  bool HandleEvent(bool running);
  void SwapWindows();
  ~Ui(); 

 private:
  int windows_size_x_;
  int windows_size_y_;
  SDL_Window* window_;
};

#endif  // UI_H
