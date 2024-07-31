#pragma once

#include "../camera/lve_camera.hpp"
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
class SimpleRenderSystem {
public:
  void run();

  SimpleRenderSystem(LveDevice &device, VkRenderPass renderPass);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem &) = delete;
  SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

  void renderGameObjects(VkCommandBuffer commandBuffer,
                         std::vector<LveGameObject> &gameObjects,
                         const LveCamera &camera);

private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

  LveDevice &lveDevice;

  std::unique_ptr<LvePipeline> lvePipeline;
  VkPipelineLayout pipelineLayout;
};
} // namespace lve
