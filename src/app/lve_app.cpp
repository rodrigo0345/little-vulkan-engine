#include "lve_app.hpp"
#include <GLFW/glfw3.h>

namespace lve {
void FirstApp::run() {
  while (!lveWindow.shouldClose()) {
    // get key structs, mouse, etc..
    glfwPollEvents();
  }
}
} // namespace lve
