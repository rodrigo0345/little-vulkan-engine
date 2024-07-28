
#pragma once

#include "../models/lve_model.hpp"

#include <glm/ext/vector_float3.hpp>
#include <memory>
#include <sys/types.h>
namespace lve {

struct Transform2DComponent {
  glm::vec2 translation{};
  glm::vec2 scale{1.f, 1.f};
  float rotationAngle;

  glm::mat2 mat2() {
    const float sin = glm::sin(rotationAngle);
    const float cos = glm::cos(rotationAngle);

    glm::mat2 rotMatrix{{cos, sin}, {-sin, cos}};
    glm::mat2 scaleMat{{scale.x, 0}, {.0f, scale.y}};

    return rotMatrix * scaleMat;
  }
};

class LveGameObject {

public:
  using id_t = unsigned int;

  static LveGameObject createGameObject() {
    static id_t currentId = 0;
    return LveGameObject{currentId++};
  }

  LveGameObject(const LveGameObject &) = delete;
  LveGameObject &operator=(const LveGameObject &) = delete;
  LveGameObject(LveGameObject &&) = default;
  LveGameObject &operator=(LveGameObject &&) = default;

  id_t getId() { return id; }

  std::shared_ptr<LveModel> model{};
  glm::vec3 color{};
  Transform2DComponent transform2d;

private:
  LveGameObject(id_t objId) : id{objId} {}
  id_t id;
};
} // namespace lve
