
#include "renderer.h"

#include <SDL2/SDL_opengl.h>
#include <gl/GL.h>
#include <gl/GLU.h>

void Renderer::Display(const Model::VectorkSize& density,
                       const Model::VectorkSize& u,
                       const Model::VectorkSize& v) {
  PreDisplay();
  DrawDensity(density);
  DrawVelocity(u, v);
  PostDisplay();
}

void Renderer::DrawDensity(const Model::VectorkSize& density) {
  double h = 1.0 / kGridSize;

  glBegin(GL_QUADS);
  for (int i = 0; i < kGridSize + 1; i++) {
    double x = (i - 0.5) * h;
    for (int j = 0; j < kGridSize + 1; j++) {
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

void Renderer::DrawVelocity(const Model::VectorkSize& u, const Model::VectorkSize& v) {
  double h = 1.0 / kGridSize;
  glColor3f(1.0, 1.0, 1.0);
  glLineWidth(1.0);

  glBegin(GL_LINES);
  for (int i = 1; i < kGridSize + 1; i++) {
    auto x = (i - 0.5) * h;

    for (int j = 1; j < kGridSize + 1; j++) {
      auto y = (j - 0.5) * h;
      glColor3f(1.0, 0.0, 0.0);
      glVertex2f(x, y);
      glVertex2f(x + u(i, j), y + v(i, j));
    }
  }
  glEnd();
}

void Renderer::PreDisplay() {
  glViewport(0, 0, 512, 512);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 1.0, 0.0, 1.0);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::PostDisplay() { glFlush(); }
