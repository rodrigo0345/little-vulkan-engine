#pragma once

// this helps at telling glfw that we will be using vulkan
#include <cstdint>
#include <string>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace lve {
class LveWindow {
private:
  GLFWwindow *window;

  const uint32_t width;
  const uint32_t height;
  const std::string windowName;

public:
  LveWindow(uint32_t w, uint32_t h, std::string name);
  ~LveWindow();

  // disallow copying the class
  // we do not want 2 pointers pointing to the window
  LveWindow(const LveWindow &) = delete;
  LveWindow &operator=(const LveWindow &);

  VkExtent2D getExtent() { return {this->width, this->height};}

  bool shouldClose();
  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
  void initWindow();

};
} // namespace lve
