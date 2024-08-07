#pragma once

#include "../device/lve_device.hpp"
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace lve {
// class responsavel por ler os dados de um ficheiro e alocar
// e configurar os vertex buffer da melhor maneira possível
class LveModel {

public:

  struct Vertex {
    glm::vec3 position;
    glm::vec3 color;

    static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
  };

  LveModel(LveDevice& device, const std::vector<Vertex> &vertices);
  ~LveModel();

  LveModel(const LveModel &) = delete;
  LveModel &operator=(const LveModel &) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

private:
  LveDevice& lveDevice;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;

  void createVertexBuffers(const std::vector<Vertex> &vertices);
};
} // namespace lve
