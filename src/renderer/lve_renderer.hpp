#pragma once

#include "../device/lve_device.hpp"
#include "../swap_chain/lve_swap_chain.hpp"
#include "../window/lve_window.hpp"
#include <cassert>
#include <memory>

#include <vector>
#include <vulkan/vulkan_core.h>

namespace lve {
class LveRenderer {
public:
  LveRenderer(LveWindow &window, LveDevice &device);
  ~LveRenderer();

  LveRenderer(const LveRenderer &) = delete;
  LveRenderer &operator=(const LveRenderer &) = delete;

  VkCommandBuffer beginFrame();
  void endFrame();

  void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

  bool isFrameInProgress() const { return isFrameStarted; }

  VkRenderPass getSwapChainRenderPass() const {
    return lveSwapChain->getRenderPass();
  }

  float getAspectRation() const {
    return lveSwapChain->extentAspectRatio();
  }

  VkCommandBuffer getCurrentCommandBuffer() const {

    assert(isFrameStarted &&
           "Cannot get command buffer when frame not in progress");
    return commandBuffers[currentImageIndex];
  }

private:
  LveWindow &lveWindow;

  LveDevice &lveDevice;

  std::unique_ptr<LveSwapChain> lveSwapChain;
  std::vector<VkCommandBuffer> commandBuffers;

  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();

  uint32_t currentImageIndex;
  bool isFrameStarted;
};
} // namespace lve
