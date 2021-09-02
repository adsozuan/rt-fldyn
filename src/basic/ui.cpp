#include "ui.h"

#include <algorithm>

#include "renderer.h"

Ui::Ui(int windows_size_x, int windows_size_y, std::size_t grid_size)
    : window_size_x_(windows_size_x),
      window_size_y_(windows_size_y),
      grid_size_(grid_size) {
  SDL_SetMainReady();
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
      0) {
    printf("Error: %s\n", SDL_GetError());
  }
  const char* glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_WindowFlags window_flags = (SDL_WindowFlags)(
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  window_ = SDL_CreateWindow("Real Time Fluid Dynamics", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  context_ = SDL_GL_CreateContext(window_);
  SDL_GL_MakeCurrent(window_, context_);
  SDL_GL_SetSwapInterval(1);  // Enable vsync

  mouse_x_ = {};
  mouse_y_ = {};
  ui_event_ = {};

  // Initialize OpenGL Loader
  // const char* glsl_version = "#version 130";
  bool err = false;
  glbinding::initialize([](const char* name) {
    return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name);
  });
  if (err) {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
  }

  // Setup ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io_ = ImGui::GetIO();
  (void)io_;

  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForOpenGL(window_, context_);
  ImGui_ImplOpenGL3_Init("#version 130");

  bool show_demo_window = true;

  clear_color_ = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

  glViewport(0, 0, (int)io_.DisplaySize.x, (int)io_.DisplaySize.y);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Ui::RenderUi(const Model::VectorkSize& density) {
  // 1. Show the big demo window (Most of the sample code is in
  // ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear
  // ImGui!).

  // 2. Show a simple window that we create ourselves. We use a Begin/End pair
  // to created a named window.

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame(window_);
  ImGui::NewFrame();
  {
    ImGui::Begin("Simulation settings");  // Create a window for parameters
    ImGui::Text("Change settings to see real time changes");

    ImGui::SliderFloat("diffusion rate", &ui_event_.settings.diffusion_rate, 0.0f, 1.0f);
    ImGui::SliderFloat("viscosity", &ui_event_.settings.viscosity, 0.0f, 1.0f);
    ImGui::SliderFloat("force", &ui_event_.settings.force, 0.0f, 0.1f);
    ImGui::SliderFloat("source", &ui_event_.settings.source, 0.0f, 100.0f);

    if (ImGui::Button("Apply")) {
      ui_event_.reset = true;
    }

    ImGui::SameLine();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }

  // Rendering
  ImGui::Render();
  glViewport(0, 0, (int)io_.DisplaySize.x, (int)io_.DisplaySize.y);
  glClear(GL_COLOR_BUFFER_BIT);
  DrawDensity(density);
  glClearColor(clear_color_.x * clear_color_.w, clear_color_.y * clear_color_.w,
               clear_color_.z * clear_color_.w, clear_color_.w);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(window_);
  ui_event_.reset = false;
}
void Ui::DrawDensity(const Model::VectorkSize& density) {
  double h = 1.0 / 32;

  glBegin(GL_QUADS);
  for (int i = 0; i < 32 + 1; i++) {
    double x = (i - 0.5) * h;
    for (int j = 0; j < 32 + 1; j++) {
      double y = (j - 0.5) * h;
      auto d00 = density(i, j);
      auto d01 = density(i, j + 1);
      auto d10 = density(i + 1, j);
      auto d11 = density(i + 1, j + 1);

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

UiEvent Ui::HandleEvent() {
  SDL_Event event;
  std::tuple<int, int> grid_coord{};

  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_c:
            ui_event_.reset = true;
          default:
            break;
        }

        break;
      case SDL_QUIT:
        ui_event_.quit = true;
        break;
      case SDLK_ESCAPE:
        ui_event_.quit = true;
        break;
      case SDL_MOUSEBUTTONDOWN:
        auto b = event.button;
        int x, y;
        SDL_GetMouseState(&x, &y);
        grid_coord = MouseCoordToGridCoord(x, y);
        ui_event_.grid_x = std::get<0>(grid_coord);
        ui_event_.grid_y = std::get<1>(grid_coord);
        ui_event_.dmouse_x = x - mouse_x_;
        ui_event_.dmouse_y = mouse_y_ - y;

        mouse_x_ = x;
        mouse_y_ = y;

        if (b.button == SDL_BUTTON_LEFT) {
          ui_event_.left_click = true;
          ui_event_.right_click = false;
        }
        if (b.button == SDL_BUTTON_RIGHT) {
          ui_event_.left_click = false;
          ui_event_.right_click = true;
        }
        break;
      default:
        break;
    }
  }
  return ui_event_;
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

Ui::~Ui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(context_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}