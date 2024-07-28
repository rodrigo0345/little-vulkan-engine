#include "lve_pipeline.hpp"
#include "vulkan/vulkan_core.h"
#include <cassert>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
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
LvePipeline::~LvePipeline() {
  vkDestroyShaderModule(lveDevice.device(), vertShaderModule, nullptr);
  vkDestroyShaderModule(lveDevice.device(), fragShaderModule, nullptr);
  vkDestroyPipeline(lveDevice.device(), graphicsPipeline, nullptr);
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

  file.seekg(0);

  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

PipelineConfigInfo LvePipeline::defaultPipelineConfigInfo(uint32_t width,
                                                          uint32_t height) {
  PipelineConfigInfo configInfo{};

  configInfo.inputAssemblyInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

  configInfo.viewport.x = 0.0f;
  configInfo.viewport.y = 0.0f;
  configInfo.viewport.width = static_cast<float>(width);
  configInfo.viewport.height = static_cast<float>(height);
  configInfo.viewport.minDepth = 0.0f;
  configInfo.viewport.maxDepth = 1.0f;

  configInfo.scissor.offset = {0, 0};
  configInfo.scissor.extent = {width, height};

  configInfo.rasterizationInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
  configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
  configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
  configInfo.rasterizationInfo.lineWidth = 1.0f;
  configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
  configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
  configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;

  configInfo.multisampleInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
  configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

  configInfo.colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  configInfo.colorBlendAttachment.blendEnable = VK_FALSE;

  configInfo.colorBlendInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
  configInfo.colorBlendInfo.attachmentCount = 1;
  configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;

  configInfo.depthStencilInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
  configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
  configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
  configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
  configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;

  return configInfo;
}

void LvePipeline::createGraphicsPipeline(const std::string &vertFilepath,
                                         const std::string &fragFilepath,
                                         const PipelineConfigInfo &config) {

  assert(
      config.pipelineLayout != VK_NULL_HANDLE &&
      "Cannot create  graphics pipeline, no pipelineLayout provided in config");
  ;
  assert(
      config.renderPass != VK_NULL_HANDLE &&
      "Cannot create  graphics pipeline, no pipelineLayout provided in config");
  ;
  auto vertCode = readFile(vertFilepath);
  auto fragCode = readFile(fragFilepath);

  createShaderModule(vertCode, &this->vertShaderModule);
  createShaderModule(fragCode, &this->fragShaderModule);

  VkPipelineShaderStageCreateInfo shaderStages[2];

  shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  shaderStages[0].module = vertShaderModule;
  shaderStages[0].pName = "main";
  shaderStages[0].flags = 0;
  shaderStages[0].pNext = nullptr;
  shaderStages[0].pSpecializationInfo = nullptr;

  shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  shaderStages[1].module = fragShaderModule;
  shaderStages[1].pName = "main";
  shaderStages[1].flags = 0;
  shaderStages[1].pNext = nullptr;
  shaderStages[1].pSpecializationInfo = nullptr;

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexAttributeDescriptionCount = 0;
  vertexInputInfo.vertexBindingDescriptionCount = 0;
  vertexInputInfo.pVertexAttributeDescriptions = nullptr;
  vertexInputInfo.pVertexBindingDescriptions = nullptr;

  VkPipelineViewportStateCreateInfo viewportInfo{};
  viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportInfo.viewportCount = 1;
  viewportInfo.pViewports = &config.viewport;
  viewportInfo.scissorCount = 1;
  viewportInfo.pScissors = &config.scissor;

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

  // representa o numero de programas que existem
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = shaderStages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;

  pipelineInfo.pInputAssemblyState = &config.inputAssemblyInfo;
  pipelineInfo.pViewportState = &viewportInfo;
  pipelineInfo.pRasterizationState = &config.rasterizationInfo;
  pipelineInfo.pColorBlendState = &config.colorBlendInfo;
  pipelineInfo.pDepthStencilState = &config.depthStencilInfo;
  pipelineInfo.pMultisampleState = &config.multisampleInfo;
  pipelineInfo.pDynamicState = nullptr;

  pipelineInfo.layout = config.pipelineLayout;
  pipelineInfo.renderPass = config.renderPass;
  pipelineInfo.subpass = config.subpass;

  // otimiza performance pq pode reaproveitar pipelines
  pipelineInfo.basePipelineIndex = -1;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

  if (vkCreateGraphicsPipelines(lveDevice.device(), VK_NULL_HANDLE, 1,
                                &pipelineInfo, nullptr,
                                &graphicsPipeline) != VK_SUCCESS) {
    throw std::runtime_error("ERROR: Failed to create graphics pipeline");
  }
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
