#include "lve_pipeline.hpp"
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace lve {
std::vector<char> LvePipeline::readFile(const std::string &filepath) {
  // ate is to get the end imediatlly
  std::ifstream file{filepath, std::ios::ate | std::ios::binary};

  if (!file.is_open()) {
    throw std::runtime_error("ERROR: Failed to open file: " + filepath);
  }

  // tellg gets the current position, and bc we are at the end of the file
  // this easily tells us the file size
  size_t fileSize = static_cast<size_t>(file.tellg());

  std::vector<char> buffer(fileSize);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}
void LvePipeline::createGraphicsPipeline(const std::string &vertFilepath,
                                         const std::string &fragFilepath) {

  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilepath);

  std::cout << "Vertex Shader Code Size: " << vertCode.size() << "\n";
  std::cout << "Fragment Shader Code Size: " << fragCode.size() << "\n";
}
LvePipeline::LvePipeline(const std::string &vertFilepath,
                         const std::string &fragFilepath) {
  this->createGraphicsPipeline(vertFilepath, fragFilepath);
}
} // namespace lve
