#pragma once

#include "../pipeline/lve_pipeline.hpp"
#include "../window/lve_window.hpp"

namespace lve {
class FirstApp {
private:
  LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan"};
  LvePipeline lvePipeline{"shaders/build/simple_shader.vert.spv",
                          "shaders/build/simple_shader.frag.spv"};

public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  void run();
};
} // namespace lve
