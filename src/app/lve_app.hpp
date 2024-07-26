#pragma once

#include "../pipeline/lve_pipeline.hpp"
#include "../window/lve_window.hpp"

namespace lve {
class FirstApp {
public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();

private:
  LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan"};

  LveDevice lveDevice{lveWindow};

  LvePipeline lvePipeline{
      lveDevice, "shaders/build/simple_shader.vert.spv",
      "shaders/build/simple_shader.frag.spv",
      LvePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};

};
} // namespace lve
