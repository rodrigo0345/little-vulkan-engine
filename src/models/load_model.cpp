
#include "load_model.hpp"
#include "lve_model.hpp"
#include <cstring>
#include <vector>

namespace lve {

std::vector<LveModel::Vertex> LveLoadModel::loadMesh(const std::string &filePath) {
  std::vector<unsigned int> indices{// Top
                                    2, 6, 7, 2, 3, 7,

                                    // Bottom
                                    0, 4, 5, 0, 1, 5,

                                    // Left
                                    0, 2, 6, 0, 4, 6,

                                    // Right
                                    1, 3, 7, 1, 5, 7,

                                    // Front
                                    0, 2, 3, 0, 1, 3,

                                    // Back
                                    4, 6, 7, 4, 5, 7};

  std::vector<float> vertices{
      -1, -1, 1,  // 0
      1,  -1, 1,  // 1
      -1, 1,  1,  // 2
      1,  1,  1,  // 3
      -1, -1, -1, // 4
      1,  -1, -1, // 5
      -1, 1,  -1, // 6
      1,  1,  -1  // 7
  };

  std::vector<LveModel::Vertex> verticesResult;
  verticesResult.reserve(indices.size());

  for (size_t i = 0; i < indices.size(); ++i) {
    unsigned int index = indices[i];
    LveModel::Vertex vertex{};
    vertex.position = glm::vec4(vertices[3 * index], vertices[3 * index + 1],
                                vertices[3 * index + 2], 1.0f);
    verticesResult.push_back(vertex);
  }

  return verticesResult;
}
} // namespace lve
