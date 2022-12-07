#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <random>

#include "abcgOpenGL.hpp"

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 textCoord{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Model {
public:
  void loadTexture(std::string_view path);
  void loadObj(bool standardize = true);
  void render(int numTriangles = -1) const;
  void setupVAO(GLuint program);
  void destroy() const;
  void randomZ();
  void createBuffers();
  void standardize();

  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  GLuint m_texture{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  std::default_random_engine m_randomEngine;

  bool m_hasNormals{false};
  bool m_hasTextCoords{false};

  void computeNormal();
};

#endif