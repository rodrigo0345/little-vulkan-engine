#include "lve_app.hpp"

// std
#include <array>
#include <cassert>
#include <glm/gtc/constants.hpp>
#include <stdexcept>
#include <utility>
#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace lve {

struct SimplePushConstantData {
  glm::mat2 transform{1.0f};
  glm::vec2 offset;
  // isto existe??? Ao que parece
  // é preciso ter muita atenção a isto quando
  // se usa constants
  alignas(16) glm::vec3 color;
};

FirstApp::FirstApp() {
  loadGameObjects();
  createPipelineLayout();
  createPipeline();
}

FirstApp::~FirstApp() {
  vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
}

void FirstApp::run() {
  while (!lveWindow.shouldClose()) {
    glfwPollEvents();
    if (auto commandBuffer = lveRenderer.beginFrame()) {
      lveRenderer.beginSwapChainRenderPass(commandBuffer);
      renderGameObjects(commandBuffer);
      lveRenderer.endSwapChainRenderPass(commandBuffer);
      lveRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(lveDevice.device());
}

void FirstApp::loadGameObjects() {
  std::vector<LveModel::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                         {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                         {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

  auto triangle = LveGameObject::createGameObject();

  triangle.model = lveModel;
  triangle.color = {.1f, .8f, .1f};
  triangle.transform2d.translation.x = .2f;
  triangle.transform2d.scale = {2.f, .5f};
  triangle.transform2d.rotationAngle = 0.25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
}

void FirstApp::createPipelineLayout() {

  VkPushConstantRange pushConstantRange;
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr,
                             &pipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void FirstApp::createPipeline() {
  assert(pipelineLayout != nullptr &&
         "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = lveRenderer.getSwapChainRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  lvePipeline = std::make_unique<LvePipeline>(
      lveDevice, "shaders/build/simple_shader_vert.spv",
      "shaders/build/simple_shader_frag.spv", pipelineConfig);
}

void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer) {
  lvePipeline->bind(commandBuffer);

  for (auto &obj : gameObjects) {

    obj.transform2d.rotationAngle =
        glm::mod(obj.transform2d.rotationAngle + 0.0001f, glm::two_pi<float>());
    // vamos desenhar 4 copias do nosso modelo
    SimplePushConstantData push{};
    push.offset = obj.transform2d.translation;
    push.color = obj.color;
    push.transform = obj.transform2d.mat2();

    vkCmdPushConstants(commandBuffer, pipelineLayout,
                       VK_SHADER_STAGE_VERTEX_BIT |
                           VK_SHADER_STAGE_FRAGMENT_BIT,
                       0, sizeof(SimplePushConstantData), &push);

    obj.model->bind(commandBuffer);
    obj.model->draw(commandBuffer);
  }
}
} // namespace lve
