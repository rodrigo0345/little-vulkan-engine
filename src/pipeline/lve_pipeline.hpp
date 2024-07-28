#pragma once

#include "../device/lve_device.hpp"
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace lve {

struct PipelineConfigInfo {
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class LvePipeline {
public:
  LvePipeline(LveDevice &device, const std::string &vertFilepath,
              const std::string &fragFilepath, const PipelineConfigInfo config);

  ~LvePipeline(); 

  LvePipeline(const LvePipeline &) = delete;
  void operator=(const LvePipeline &) = delete;

  static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width,
                                                      uint32_t height);

  void bind(VkCommandBuffer commandBuffer);

private:
  LveDevice &lveDevice;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;

  static std::vector<char> readFile(const std::string &filepath);

  void createGraphicsPipeline(const std::string &vertFilepath,
                              const std::string &fragFilepath,
                              const PipelineConfigInfo& configInfo);

  void createShaderModule(const std::vector<char> &code,
                          VkShaderModule *shaderModule);

};
} // namespace lve
