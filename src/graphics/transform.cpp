#include "graphics/scene/transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

namespace graphics {
namespace scene {
  Transform::Transform()
  {
    this->m_parentMatrix = glm::mat4(1.0f);
    this->m_scale = glm::vec3(1,1,1);
    this->m_rotation = glm::vec3(0,0,0);
    this->m_translation = glm::vec3(0,0,0);
  }

  void Transform::translate(const glm::vec3& translation)
  {
    this->m_translation += translation;
  }

  void Transform::rotate(const glm::vec3& rotation)
  {
    this->m_rotation += rotation;
    this->m_rotation.x = fmod(this->m_rotation.x, 360);
    this->m_rotation.y = fmod(this->m_rotation.y, 360);
    this->m_rotation.z = fmod(this->m_rotation.z, 360);
  }

  void Transform::scale(const glm::vec3& scale)
  {
    this->m_scale.x = this->m_scale.x * scale.x;
    this->m_scale.y = this->m_scale.y * scale.y;
    this->m_scale.z = this->m_scale.z * scale.z;
  }

  void Transform::setTranslation(const glm::vec3& translation)
  {
    this->m_translation = translation;
  }

  void Transform::setRotation(const glm::vec3& rotation)
  {
    this->m_rotation = rotation;
  }

  void Transform::setScale(const glm::vec3& scale)
  {
    this->m_scale = scale;
  }


  void Transform::setParentMatrix(glm::mat4 parent)
  {
    this->m_parentMatrix = parent;
  }

  const glm::vec3 Transform::getTranslation() const
  {
    return this->m_translation;
  }

  const glm::vec3 Transform::getRotation() const
  {
    return this->m_rotation;
  }

  const glm::vec3 Transform::getScale() const
  {
    return this->m_scale;
  }

  const glm::mat4 Transform::getMatrix() const
  {
    glm::mat4 out  = glm::mat4(1.0f);
    out = glm::scale(out, this->m_scale);
    out = glm::translate(out, this->m_translation);
    out = glm::rotate( out, this->m_rotation.x, glm::vec3(1,0,0));
    out = glm::rotate( out, this->m_rotation.y, glm::vec3(0,1,0));
    out = glm::rotate( out, this->m_rotation.z, glm::vec3(0,0,1));

    return out;
  }

  const glm::mat4 Transform::getParentMatrix() const
  {
    return this->m_parentMatrix;
  }

  const glm::mat4 Transform::getAbsoluteMatrix() const
  {
    return this->m_parentMatrix * this->getMatrix();
  }

} // namespace scene
} // namespace graphics
