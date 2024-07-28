#pragma once

#include "../pipeline/lve_pipeline.hpp"
#include "../window/lve_window.hpp"
#include "../swap_chain/lve_swap_chain.hpp"
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace lve {
class FirstApp {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();

  FirstApp();
  ~FirstApp();

  FirstApp(const FirstApp&) = delete;
  FirstApp &operator=(const FirstApp&) = delete;

private:
  LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan"};

  LveDevice lveDevice{lveWindow};

  LveSwapChain lveSwapChain{lveDevice, lveWindow.getExtent()};

  std::unique_ptr<LvePipeline> lvePipeline;

  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;

  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();
};
} // namespace lve
