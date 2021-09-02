#ifndef UI_H
#define UI_H

#include "renderer.h"
#include "solver.h"

#include <SDL.h>
#include <stdio.h>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load
//  OpenGL function pointers. Helper libraries are often used for this purpose!
//  Here we are supporting a few common ones (gl3w, glew, glad). You may use
//  another loader/header of your choice (glext, glLoadGen, etc.), or chose to
//  manually implement your own.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>  // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>  // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
#include <glad/gl.h>  // Initialize with gladLoadGL(...) or gladLoaderLoadGL()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#define GLFW_INCLUDE_NONE  // GLFW including OpenGL headers causes ambiguity or
                           // multiple definition errors.
#include <glbinding/Binding.h>  // Initialize with glbinding::Binding::initialize()
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#define GLFW_INCLUDE_NONE  // GLFW including OpenGL headers causes ambiguity or
                           // multiple definition errors.
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>  // Initialize with glbinding::initialize()
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

struct SimulationSettings {
    float diffusion_rate = 0.1;
    float viscosity = 0.1;
    float force = 0.005;
    float source = 100.0;
};

struct UiEvent {
  bool quit;
  bool left_click;
  bool right_click;
  bool reset;
  int grid_x;
  int grid_y;
  int dmouse_x;
  int dmouse_y;
  SimulationSettings settings;
};

//@class Ui
//@brief handle SDL windows, and implements RAII for resource freeing.
class Ui {
 public:
  Ui(int windows_size_x, int windows_size_y, size_t grid_size);
  void RenderUi(const Model::VectorkSize& density);
  void DrawDensity(const Model::VectorkSize& density);
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
  ImVec4 clear_color_;
  ImGuiIO io_;
  UiEvent ui_event_;
};

#endif  // UI_H
