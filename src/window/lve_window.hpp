#pragma once

// this helps at telling glfw that we will be using vulkan
#include <string>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace lve {
class LveWindow {
private:
  GLFWwindow *window;

  const int width;
  const int height;
  const std::string windowName;

  void initWindow();

public:
  LveWindow(int w, int h, std::string name);
  ~LveWindow();

  // disallow copying the class
  // we do not want 2 pointers pointing to the window
  LveWindow(const LveWindow &) = delete;
  LveWindow &operator=(const LveWindow &);

  bool shouldClose();
  void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
};
} // namespace lve
