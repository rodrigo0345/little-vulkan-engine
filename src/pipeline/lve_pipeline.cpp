#include "lve_pipeline.hpp"
#include "vulkan/vulkan_core.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace lve {
LvePipeline::LvePipeline(LveDevice &device, const std::string &vertFilepath,
                         const std::string &fragFilepath,
                         const PipelineConfigInfo config)
    : lveDevice(device) {
  this->createGraphicsPipeline(vertFilepath, fragFilepath, config);
}

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

PipelineConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width,
                                                          uint32_t height) {
  PipelineConfigInfo configInfo{};

  configInfo.inputAssemblyInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  // estamos a dizer para os pontos que passamos serem organizados em triangulos
  configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

  return configInfo;
}

void LvePipeline::createGraphicsPipeline(const std::string &vertFilepath,
                                         const std::string &fragFilepath,
                                         const PipelineConfigInfo &config) {

  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilepath);

  std::cout << "Vertex Shader Code Size: " << vertCode.size() << "\n";
  std::cout << "Fragment Shader Code Size: " << fragCode.size() << "\n";
}

// takes the shader code and creates a module
void LvePipeline::createShaderModule(const std::vector<char> &code,
                                     VkShaderModule *shaderModule) {
  VkShaderModuleCreateInfo createInfo{};

  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr,
                           shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("ERROR: Failed to create shader module");
  }
}
} // namespace lve
