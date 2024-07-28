#pragma once

#include "../pipeline/lve_pipeline.hpp"
#include "../window/lve_window.hpp"
#include "../swap_chain/lve_swap_chain.hpp"
#include "../models/lve_model.hpp"
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
  void loadModels();

  LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan"};

  LveDevice lveDevice{lveWindow};

  std::unique_ptr<LveSwapChain> lveSwapChain;

  std::unique_ptr<LvePipeline> lvePipeline;

  VkPipelineLayout pipelineLayout;
  std::vector<VkCommandBuffer> commandBuffers;
  std::unique_ptr<LveModel> lveModel;

  void createPipelineLayout();
  void createPipeline();
  void createCommandBuffers();
  void drawFrame();
  void recreateSwapChain();
  void recordCommandBuffer(int imageIndex);
};
} // namespace lve
