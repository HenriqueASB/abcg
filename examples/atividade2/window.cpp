#include "window.hpp"

void Window::onCreate() {
  auto const *vertexShader{R"gl(#version 300 es

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec4 inColor;

out vec4 fragColor;

void main() { 
  gl_Position = vec4(inPosition, 0, 1);
  fragColor = inColor;
}
  )gl"};

  auto const *fragmentShader{R"gl(#version 300 es
                                     
    precision mediump float;

    in vec4 fragColor;

    out vec4 outColor;

    void main() { outColor = fragColor; }
  )gl"};

  m_program = abcg::createOpenGLProgram(
      {{.source = vertexShader, .stage = abcg::ShaderStage::Vertex},
       {.source = fragmentShader, .stage = abcg::ShaderStage::Fragment}});

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);
}

void Window::onPaint() {
  // Check whether to render the next triangle
  if (m_timer.elapsed() < 1.0 / 6)
    return;
  m_timer.restart();

  setupModel();

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);
  abcg::glBindVertexArray(m_VAO);

  abcg::glDrawArrays(GL_TRIANGLES, 0, 3);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const widgetSize{ImVec2(250, 90)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5,
                                   m_viewportSize.y - widgetSize.y - 5));
    ImGui::SetNextWindowSize(widgetSize);
    auto windowFlags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin(" ", nullptr, windowFlags);

    // Edit vertex colors
    auto colorEditFlags{ImGuiColorEditFlags_NoTooltip |
                        ImGuiColorEditFlags_NoPicker};
    ImGui::PushItemWidth(215);                           
    ImGui::ColorEdit3("v0", &m_colors.at(0).x, colorEditFlags);
    ImGui::ColorEdit3("v1", &m_colors.at(1).x, colorEditFlags);
    ImGui::ColorEdit3("v2", &m_colors.at(2).x, colorEditFlags);
    ImGui::PopItemWidth();

    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

std::array<glm::vec2, 3> Window::arrayPosition(){
    if(round == 0){
        std::array<glm::vec2, 3> const positions ={
      {{0.0f,0.0f},
       {-0.5f,-1.0f},
       {0.5f,-1.0f}}};

    return positions;
  } if(round == 1){
        std::array<glm::vec2, 3> const positions ={
      {{0.0f,0.0f},
       {0.5f,-1.0f},
       {1.0f,0.0f}}};

    return positions;
  } if(round == 2){
        std::array<glm::vec2, 3> const positions ={
      {{0.0f,0.0f},
       {1.0f,0.0f},
       {0.5f,1.0f}}};

    return positions;
  } if(round == 3){
        std::array<glm::vec2, 3> const positions ={
      {{0.0f,0.0f},
       {0.5f,1.0f},
       {-0.5f,1.0f}}};

    return positions;
  }
  if(round == 4){
        std::array<glm::vec2, 3> const positions ={
      {{0.0f,0.0f},
       {-0.5f,1.0f},
       {-1.0f,0.0f}}};

    return positions;
  }
  if(round == 5){
        std::array<glm::vec2, 3> const positions ={
      {{0.0f,0.0f},
       {-1.0f,0.0f},
       {-0.5f,-1.0f}}};

    return positions;
  }

    std::array<glm::vec2, 3> const positions ={
      {{0.0f,0.0f},
       {0.0f,0.0f},
       {0.0f,0.0f}}};

    return positions;

}

std::array<glm::vec4, 3> Window::arrayColors(){
    if(round%2==0){
        std::uniform_real_distribution rd(0.0f, 1.0f);
        glm::vec4 color = {rd(m_randomEngine), rd(m_randomEngine), rd(m_randomEngine), 1};
        std::array<glm::vec4, 3> colors{color,
                                        color,
                                        color};
        return colors;
    }
  std::uniform_real_distribution rd(0.0f, 1.0f);
  std::array<glm::vec4, 3> colors{{{rd(m_randomEngine), rd(m_randomEngine), rd(m_randomEngine), 1},
                                     {rd(m_randomEngine), rd(m_randomEngine),rd(m_randomEngine), 1},
                                     {rd(m_randomEngine), rd(m_randomEngine), rd(m_randomEngine), 1}}};
  return colors;

}

void Window::setupModel() {

  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Create array of random vertex positions
  std::array<glm::vec2, 3> const positions = arrayPosition();
  round ++;
  if(round == 6){
    round = 0;
  }

  // Generate VBO of positions
  abcg::glGenBuffers(1, &m_VBOPositions);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate VBO of colors
  m_colors = arrayColors();
  abcg::glGenBuffers(1, &m_VBOColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  auto const colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}
