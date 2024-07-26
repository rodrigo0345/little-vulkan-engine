#include "lve_window.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace lve {
LveWindow::LveWindow(int w, int h, std::string name)
    : width(w), height(h), windowName(name) {
  initWindow();
}

void LveWindow::initWindow() {
  glfwInit();

  // disable opengl mode
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  // window resizing is treated in a special way
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  // the last parameter is related to opengl
  window =
      glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

LveWindow::~LveWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

bool LveWindow::shouldClose() { return glfwWindowShouldClose(this->window); }

void LveWindow::createWindowSurface(VkInstance instance,
                                    VkSurfaceKHR *surface) {
  if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
    throw std::runtime_error("ERROR: Failed to create window surface");
  }
} // namespace lve
} // namespace lve
