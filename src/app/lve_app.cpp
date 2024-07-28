#include "lve_app.hpp"
#include <GLFW/glfw3.h>
#include <exception>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace lve {

FirstApp::FirstApp() {
  createPipelineLayout();
  createPipeline();
  createCommandBuffers();
}
FirstApp::~FirstApp() {
  vkDestroyPipelineLayout(lveDevice.device(), this->pipelineLayout, nullptr);
}

void FirstApp::run() {
  while (!lveWindow.shouldClose()) {
    // get key structs, mouse, etc..
    glfwPollEvents();
  }
}

void FirstApp::createPipelineLayout() {
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

  // injeta diretamente nos shaders info adicional (layouts)
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("ERROR: Failed to create pipeline layout");
  }
}
void FirstApp::createPipeline() {
  // não podemos ir buscar os pixeis diretamente às variaveis
  // de width e height de FirstApp porque nos apple retina displays,
  // 1px não é o mesmo que 1px nos outros monitores
  auto pipelineConfig = LvePipeline::defaultPipelineConfigInfo(
      lveSwapChain.width(), lveSwapChain.height());
  pipelineConfig.renderPass = lveSwapChain.getRenderPass();
  pipelineConfig.pipelineLayout = this->pipelineLayout;

  // debug
  std::cout << "Current path is " << std::filesystem::current_path() << '\n';
  lvePipeline = std::make_unique<LvePipeline>(
      lveDevice, "./shaders/build/simple_shader_vert.spv",
      "./shaders/build/simple_shader_frag.spv", pipelineConfig);
}
void FirstApp::createCommandBuffers() {}
void FirstApp::drawFrame() {}
} // namespace lve
