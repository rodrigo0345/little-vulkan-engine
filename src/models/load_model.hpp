#pragma once

#include "lve_model.hpp"
#include <string>
#include <vector>

namespace lve {
class LveLoadModel {

public:
  static std::vector<LveModel::Vertex> loadMesh(const std::string &filePath);

private:
};
} // namespace lve
