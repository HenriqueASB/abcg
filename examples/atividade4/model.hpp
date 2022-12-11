#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <random>

#include "abcgOpenGL.hpp"

struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Model {
public:
  void loadDiffuseTexture(std::string_view path);
  void createGeometry(bool standardize = true);
  void render(int numTriangles = -1) const;
  void setupVAO(GLuint program);
  void destroy();
  void randomZ();

  int number{10};

  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

  [[nodiscard]] glm::vec4 getKa() const { return m_Ka; }
  [[nodiscard]] glm::vec4 getKd() const { return m_Kd; }
  [[nodiscard]] glm::vec4 getKs() const { return m_Ks; }
  [[nodiscard]] float getShininess() const { return m_shininess; }

  [[nodiscard]] bool isUVMapped() const { return m_hasTexCoords; }

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  glm::vec4 m_Ka = {0.1f, 0.1f, 0.1f, 1.0f};
  glm::vec4 m_Kd = {0.7f, 0.7f, 0.7f, 1.0f};
  glm::vec4 m_Ks = {1.0f, 1.0f, 1.0f, 1.0f};
  float m_shininess = 25.0f;
  GLuint m_diffuseTexture{};

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;

  bool m_hasNormals{false};
  bool m_hasTexCoords{false};

  void computeNormals();
  void createBuffers();
  void standardize();

  std::default_random_engine m_randomEngine{};
};

#endif