#include "lve_app.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace lve {

FirstApp::FirstApp() {
  loadModels();
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
    this->drawFrame();
  }

  // espera que a gpu acabe tudo e só depois acaba o programa
  vkDeviceWaitIdle(lveDevice.device());
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

void FirstApp::createCommandBuffers() {
  commandBuffers.resize(lveSwapChain.imageCount());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = lveDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(lveDevice.device(), &allocInfo,
                               commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("ERROR: Failed to allocate command buffers");
  }

  for (int i = 0; i < commandBuffers.size(); i++) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error(
          "ERROR: Failed to start begin info command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = lveSwapChain.getRenderPass();
    renderPassInfo.framebuffer = lveSwapChain.getFrameBuffer(i);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = lveSwapChain.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};

    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo,
                         VK_SUBPASS_CONTENTS_INLINE);

    lvePipeline->bind(commandBuffers[i]);

    lveModel->bind(commandBuffers[i]);
    lveModel->draw(commandBuffers[i]);

    vkCmdEndRenderPass(commandBuffers[i]);

    if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("ERROR: Failed to record command buffer");
    }
  }
}
void FirstApp::drawFrame() {
  uint32_t imageIndex;

  auto result = lveSwapChain.acquireNextImage(&imageIndex);

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("ERROR: Failed acquire swap chain image");
  }

  // trata de enviar os nossos command buffers para a gpu
  result = lveSwapChain.submitCommandBuffers(&commandBuffers[imageIndex],
                                             &imageIndex);

  if (result != VK_SUCCESS) {
    throw std::runtime_error("ERROR: Failed acquire swap chain image");
  }
}

std::vector<LveModel::Vertex>
sierpinskiTriangles(std::vector<LveModel::Vertex> parentTriangle,
                    int currCount = 0, int limit = 4) {
  if (currCount == limit) {
    return parentTriangle;
  }

  // Calculate the midpoints of each edge of the parent triangle
  LveModel::Vertex midpoint1{
      {(parentTriangle[0].position.x + parentTriangle[1].position.x) / 2.0f,
       (parentTriangle[0].position.y + parentTriangle[1].position.y) / 2.0f}};

  LveModel::Vertex midpoint2{
      {(parentTriangle[1].position.x + parentTriangle[2].position.x) / 2.0f,
       (parentTriangle[1].position.y + parentTriangle[2].position.y) / 2.0f}};

  LveModel::Vertex midpoint3{
      {(parentTriangle[2].position.x + parentTriangle[0].position.x) / 2.0f,
       (parentTriangle[2].position.y + parentTriangle[0].position.y) / 2.0f}};

  // Create the 3 new triangles
  std::vector<LveModel::Vertex> t1 = {parentTriangle[0], midpoint1, midpoint3};
  std::vector<LveModel::Vertex> t2 = {parentTriangle[1], midpoint2, midpoint1};
  std::vector<LveModel::Vertex> t3 = {parentTriangle[2], midpoint3, midpoint2};

  // Recursively divide the new triangles
  auto res1 = sierpinskiTriangles(t1, currCount + 1, limit);
  auto res2 = sierpinskiTriangles(t2, currCount + 1, limit);
  auto res3 = sierpinskiTriangles(t3, currCount + 1, limit);

  // Combine the results
  std::vector<LveModel::Vertex> result;
  result.reserve(res1.size() + res2.size() + res3.size());
  result.insert(result.end(), res1.begin(), res1.end());
  result.insert(result.end(), res2.begin(), res2.end());
  result.insert(result.end(), res3.begin(), res3.end());

  return result;
}

void FirstApp::loadModels() {
  std::vector<LveModel::Vertex> verticesP{
      {{0.0f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}}};

  auto vertices = sierpinskiTriangles(verticesP);

  lveModel = std::make_unique<LveModel>(lveDevice, vertices);
  ;
}

} // namespace lve
