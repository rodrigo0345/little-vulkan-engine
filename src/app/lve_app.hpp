#pragma once

#include "../game_object/lve_game_object.hpp"
#include "../models/lve_model.hpp"
#include "../pipeline/lve_pipeline.hpp"
#include "../renderer/lve_renderer.hpp"
#include "../swap_chain/lve_swap_chain.hpp"
#include "../window/lve_window.hpp"
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

  FirstApp(const FirstApp &) = delete;
  FirstApp &operator=(const FirstApp &) = delete;

private:
  void loadGameObjects();

  LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan"};

  LveDevice lveDevice{lveWindow};
  LveRenderer lveRenderer{lveWindow, lveDevice};

  std::unique_ptr<LvePipeline> lvePipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<LveGameObject> gameObjects;

  void createPipelineLayout();
  void createPipeline();
  void renderGameObjects(VkCommandBuffer commandBuffer);
};
} // namespace lve
