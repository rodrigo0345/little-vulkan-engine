#include "lve_model.hpp"
#include <cstddef>
#include <cstring>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace lve {
// class responsavel por ler os dados de um ficheiro e alocar
// e configurar os vertex buffer da melhor maneira possível

LveModel::LveModel(LveDevice &device, const std::vector<Vertex> &vertices)
    : lveDevice{device} {
  createVertexBuffers(vertices);
}

LveModel::~LveModel() {
  vkDestroyBuffer(this->lveDevice.device(), this->vertexBuffer, nullptr);
  vkFreeMemory(this->lveDevice.device(), this->vertexBufferMemory, nullptr);
}

void LveModel::createVertexBuffers(const std::vector<Vertex> &vertices) {
  this->vertexCount = static_cast<uint32_t>(vertices.size());
  assert(this->vertexCount >= 3 && "Vertex count must be at least 3");

  VkDeviceSize bufferSize = sizeof(vertices[0]) * this->vertexCount;

  // HOST = CPU, DEVICE = GPU
  lveDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                             VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                         this->vertexBuffer, this->vertexBufferMemory);

  // estamos a criar uma zona partilhada entre o cpu e gpu, ent
  // a cpu a colocar os dados nesta zona, a gpu consegue ler automaticamente
  void *data;
  vkMapMemory(this->lveDevice.device(), this->vertexBufferMemory, 0, bufferSize,
              0, &data);

  memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
  vkUnmapMemory(this->lveDevice.device(), this->vertexBufferMemory);
}
void LveModel::draw(VkCommandBuffer commandBuffer) {
  vkCmdDraw(commandBuffer, this->vertexCount, 1, 0, 0);
}

void LveModel::bind(VkCommandBuffer commandBuffer) {
  VkBuffer buffers[] = {this->vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

std::vector<VkVertexInputBindingDescription>
LveModel::Vertex::getBindingDescriptions() {
  std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
  bindingDescriptions[0].binding = 0;
  bindingDescriptions[0].stride = sizeof(Vertex);
  bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription>
LveModel::Vertex::getAttributeDescriptions() {
  std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;

  // isto n tem de ser necessáriamente cores, no nosso caso é position
  attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, position);

  // ---- Color information ----
  attributeDescriptions[1].binding = 0;

  // isto tem de dar match ao que temos no vert shader
  attributeDescriptions[1].location = 1;

  // isto n tem de ser necessáriamente cores, no nosso caso é position
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;

  // não sabia que offsetof existia, isto é top
  // ao que parece faz parte do llvm
  attributeDescriptions[1].offset = offsetof(Vertex, color);

  return attributeDescriptions;
}
} // namespace lve
