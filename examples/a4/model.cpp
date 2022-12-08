#include "model.hpp"

#include <filesystem>
#include <unordered_map>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    auto const h2{std::hash<glm::vec3>()(vertex.normal)};
    auto const h3{std::hash<glm::vec2>()(vertex.texCoord)};
    return abcg::hashCombine(h1, h2, h3);
  }
};

void Model::computeNormals() {
  // Clear previous vertex normals
  for (auto &vertex : m_vertices) {
    vertex.normal = glm::vec3(0.0f);
  }

  // Compute face normals
  for (auto const offset : iter::range<int>(0, m_indices.size(), 3)) {
    // Get face vertices
    auto &a{m_vertices.at(m_indices.at(offset + 0))};
    auto &b{m_vertices.at(m_indices.at(offset + 1))};
    auto &c{m_vertices.at(m_indices.at(offset + 2))};

    // Compute normal
    auto const edge1{b.position - a.position};
    auto const edge2{c.position - b.position};
    auto const normal{glm::cross(edge1, edge2)};

    // Accumulate on vertices
    a.normal += normal;
    b.normal += normal;
    c.normal += normal;
  }

  // Normalize
  for (auto &vertex : m_vertices) {
    vertex.normal = glm::normalize(vertex.normal);
  }

  m_hasNormals = true;
}

void Model::createBuffers() {
  // Delete previous buffers
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);

  // VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices.at(0)) * m_indices.size(),
                     m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::loadDiffuseTexture(std::string_view path) {
  if (!std::filesystem::exists(path))
    return;

  abcg::glDeleteTextures(1, &m_diffuseTexture);
  m_diffuseTexture = abcg::loadOpenGLTexture({.path = path});
}

void Model::loadObj(std::string_view path, bool standardize) {

  m_vertices.clear();
  m_indices.clear();

  m_hasNormals = false;
  m_hasTexCoords = false;

  int n = 10;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {

      glm::vec3 position{(i / (float)(n - 1)), (j / (float)(n - 1)), 0.0f};
      glm::vec3 normal{0.0f, 0.0f, 0.0f};
      glm::vec2 texCoord{(i / (float)(n - 1)), (j / (float)(n - 1))};
      Vertex const vertex{
          .position = position, .normal = normal, .texCoord = texCoord};
      m_vertices.push_back(vertex);
    }
  }
  m_hasTexCoords = true;

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - 1; j++) {
      m_indices.push_back(i * n + j);
      m_indices.push_back((i + 1) * n + j);
      m_indices.push_back((i + 1) * n + (j + 1));
      m_indices.push_back(i * n + j);
      m_indices.push_back((i + 1) * n + (j + 1));
      m_indices.push_back(i * n + (j + 1));
    }
  }

  /*
  for (int k = 0; k < ((n) * (n + 1)); k++) {
    if (((k + 1) % (n + 1))) {
      m_indices.push_back(k);
      m_indices.push_back(k + n + 2);
      m_indices.push_back(k + n + 1);
      m_indices.push_back(k);
      m_indices.push_back(k + 1);
      m_indices.push_back(k + n + 2);
    }
  }
  */
  // Default values
  m_Ka = {0.1f, 0.1f, 0.1f, 1.0f};
  m_Kd = {0.7f, 0.7f, 0.7f, 1.0f};
  m_Ks = {1.0f, 1.0f, 1.0f, 1.0f};
  m_shininess = 25.0f;

  auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  if (standardize) {
    Model::standardize();
  }

  if (!m_hasNormals) {
    computeNormals();
  }

  // randomZ();

  createBuffers();
}

void Model::randomZ() {
  std::uniform_real_distribution rd(0.0f, 1.0f);

  for (auto &vertex : m_vertices) {
    vertex.position = {vertex.position.x, vertex.position.y,
                       rd(m_randomEngine)};
  }
}

void Model::render(int numTriangles) const {
  abcg::glBindVertexArray(m_VAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);

  // Set minification and magnification parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set texture wrapping parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  auto const numIndices{(numTriangles < 0) ? m_indices.size()
                                           : numTriangles * 3};

  abcg::glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
}

void Model::setupVAO(GLuint program) {
  // Release previous VAO
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);

  // Bind EBO and VBO
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  // Bind vertex attributes
  auto const positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};
  if (positionAttribute >= 0) {
    abcg::glEnableVertexAttribArray(positionAttribute);
    abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex), nullptr);
  }

  auto const normalAttribute{abcg::glGetAttribLocation(program, "inNormal")};
  if (normalAttribute >= 0) {
    abcg::glEnableVertexAttribArray(normalAttribute);
    auto const offset{offsetof(Vertex, normal)};
    abcg::glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  auto const texCoordAttribute{
      abcg::glGetAttribLocation(program, "inTexCoord")};
  if (texCoordAttribute >= 0) {
    abcg::glEnableVertexAttribArray(texCoordAttribute);
    auto const offset{offsetof(Vertex, texCoord)};
    abcg::glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE,
                                sizeof(Vertex),
                                reinterpret_cast<void *>(offset));
  }

  // End of binding
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);
}

void Model::standardize() {
  // Center to origin and normalize largest bound to [-1, 1]

  // Get bounds
  glm::vec3 max(std::numeric_limits<float>::lowest());
  glm::vec3 min(std::numeric_limits<float>::max());
  for (auto const &vertex : m_vertices) {
    max = glm::max(max, vertex.position);
    min = glm::min(min, vertex.position);
  }

  // Center and scale
  auto const center{(min + max) / 2.0f};
  auto const scaling{2.0f / glm::length(max - min)};
  for (auto &vertex : m_vertices) {
    vertex.position = (vertex.position - center) * scaling;
  }
}

void Model::destroy() {
  abcg::glDeleteTextures(1, &m_diffuseTexture);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}