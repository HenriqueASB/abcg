#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBOPositions{};
  GLuint m_VBOColors{};
  GLuint m_program{};


  abcg::Timer m_timer;

  std::default_random_engine m_randomEngine;

  std::array<glm::vec4, 3> m_colors{};

  int round{0}; 

  void setupModel();

  std::array<glm::vec2, 3> arrayPosition();
  std::array<glm::vec4, 3> arrayColors();

};

#endif