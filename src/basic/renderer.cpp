
#include "renderer.h"

Renderer::Renderer(std::size_t grid_size, std::size_t windows_size_x, std::size_t windows_size_y)
    : grid_size_(grid_size), windows_size_x_(windows_size_x), windows_size_y_(windows_size_y) {
  glViewport(0, 0, windows_size_x_, windows_size_y_);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 1.0, 0.0, 1.0);
  CheckGLError();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  CheckGLError();

  glClearColor(0.f, 0.f, 0.f, 1.f);
  CheckGLError();

  PreDisplay();
}

void Renderer::Display(const Model::VectorkSize& density,
                       const Model::VectorkSize& u,
                       const Model::VectorkSize& v) {
  PreDisplay();
  DrawDensity(density);
  DrawVelocity(u, v);
  PostDisplay();
}

void Renderer::PreDisplay() {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawDensity(const Model::VectorkSize& density) {
  double h = 1.0 / grid_size_;

  glBegin(GL_QUADS);
  for (int i = 0; i < grid_size_ + 1; i++) {
    double x = (i - 0.5) * h;
    for (int j = 0; j < grid_size_ + 1; j++) {
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

void Renderer::DrawVelocity(const Model::VectorkSize& u,
                            const Model::VectorkSize& v) {
  double h = 1.0 / grid_size_;

  glColor3f(1.0, 0.0, 0.0);
  glLineWidth(1.0);

  glBegin(GL_LINES);
  for (int i = 1; i < grid_size_ + 1; i++) {
    auto x = (i - 0.5) * h;

    for (int j = 1; j < grid_size_ + 1; j++) {
      auto y = (j - 0.5) * h;
      glVertex2f(x, y);
      auto u_i = u(i, j);
      auto v_i = v(i, j);
      glVertex2f(x + u_i, y + v_i);
    }
  }
  glEnd();
}

void Renderer::PostDisplay() { //glFlush(); 
}

void Renderer::CheckGLError() {
  GLenum error = GL_NO_ERROR;
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "Error initializing OpenGL! " << gluErrorString(error)
              << std::endl;
  }
}
