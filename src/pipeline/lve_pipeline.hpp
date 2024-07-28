#pragma once

#include "../device/lve_device.hpp"
#include <vector>
#include <vulkan/vulkan_core.h>


namespace lve {

struct PipelineConfigInfo {
  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;

  std::vector<VkDynamicState> dynamicStateEnables;
  VkPipelineDynamicStateCreateInfo dynamicStateInfo;

  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class LvePipeline {
public:
  LvePipeline(LveDevice &device, const std::string &vertFilepath,
              const std::string &fragFilepath, const PipelineConfigInfo& configInfo);

  ~LvePipeline();

  static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

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
