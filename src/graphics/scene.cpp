#include "graphics/scene/scene.h"
#include "graphics/scene/node.h"
#include "graphics/scene/transform.h"

// Components: Add new components here
#include "graphics/scene/component.h"
#include "graphics/scene/script_component.h"
#include "graphics/scene/skeleton_mesh_component.h"
#include "graphics/scene/mesh_component.h"

#include "json/json.h"
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

namespace graphics {
namespace scene {

void parseJsonNode(
  const Json::Value& root,
  Node* rootNode,
  std::vector<std::unique_ptr<Node>>& nodesArr,
  std::vector<std::unique_ptr<Component>>& componentsArr
)
{
  if(root == Json::nullValue || rootNode == nullptr)
  {
    return;
  }

  const Json::Value& transform = root["transform"];
  if(transform != Json::nullValue)
  {
    const Json::Value& location = transform["location"];
    if(location != Json::nullValue)
    {
      float v0 = location[0].asFloat();
      float v1 = location[1].asFloat();
      float v2 = location[2].asFloat();

      glm::vec3 vec(v0, v1, v2);
      rootNode->getTransform().setTranslation(vec);
    }

    const Json::Value& rotation = transform["rotation"];
    if(rotation != Json::nullValue)
    {
      float v0 = rotation[0].asFloat();
      float v1 = rotation[1].asFloat();
      float v2 = rotation[2].asFloat();

      rootNode->getTransform().setRotation(
        glm::vec3(v0, v1, v2)
      );
    }

    const Json::Value& scale = transform["scale"];
    if(scale != Json::nullValue)
    {
      float v0 = scale[0].asFloat();
      float v1 = scale[1].asFloat();
      float v2 = scale[2].asFloat();

      rootNode->getTransform().setScale(
        glm::vec3(v0, v1, v2)
      );
    }
  }

  const Json::Value& components = root["components"];
  for(int i = 0; i < components.size(); ++i)
  {
    const Json::Value& component = components[i];
    std::string type = component["type"].asString();

    if(type == "Mesh")
    {
      componentsArr.push_back(
        std::make_unique<MeshComponent>()
      );

      MeshComponent* mc =
        static_cast<MeshComponent*>(
          componentsArr.back().get()
        );

      mc->setId(component["id"].asInt());
    }
    else if(type == "SkeletonMesh")
    {
      componentsArr.push_back(
        std::make_unique<SkeletonMeshComponent>()
      );

      SkeletonMeshComponent* smc =
        static_cast<SkeletonMeshComponent*>(
          componentsArr.back().get()
        );

        smc->setId(component["id"].asInt());
        smc->setAnimName(component["animName"].asString());
        smc->setAnimTime(component["animTime"].asFloat());
    }

    Component* componentPtr = componentsArr.back().get();
    rootNode->addComponent(componentPtr);
  }

  const Json::Value& children = root["children"];
  for(int i = 0; i < children.size(); ++i)
  {
    nodesArr.push_back(std::make_unique<Node>());
    Node* childNode = nodesArr.back().get();

    parseJsonNode(
      children[i],
      childNode,
      nodesArr,
      componentsArr
    );

    rootNode->addChild(childNode);
  }

}

Scene::Scene()
{
  this->m_rootNode = std::make_unique<Node>();
}

bool Scene::load(std::string filePath)
{
  std::ifstream ifs(filePath);
  Json::Value root;
  ifs >> root;

  parseJsonNode(
    root,
    this->m_rootNode.get(),
    this->m_nodes,
    this->m_components
  );

  ifs.close();
  return true;
}

void Scene::startup(){
  this->startup(this->m_rootNode.get());
}

void Scene::update(float dt){
  this->update(this->m_rootNode.get(), dt);
  this->updateAnimTime(this->m_rootNode.get(), dt);
}

void Scene::shutdown(){
  this->shutdown(this->m_rootNode.get());
}

Node* Scene::getRoot()
{
  return this->m_rootNode.get();
}

void Scene::startup(Node* root)
{
  auto scripts = root->getComponents(
    graphics::scene::component::Type::Script);

  for(std::size_t i = 0; i < scripts.size(); ++i)
  {
    auto script = (graphics::scene::ScriptComponent*)scripts[i];
    script->startup();
  }

  for(std::size_t i = 0; i < root->numChildren(); ++i)
  {
    startup(root->getChild(i));
  }
}

void Scene::update(Node* root, float dt)
{
  auto scripts = root->getComponents(
    graphics::scene::component::Type::Script);

  for(std::size_t i = 0; i < scripts.size(); ++i)
  {
    auto script = (graphics::scene::ScriptComponent*)scripts[i];
    script->update(dt);
  }

  for(std::size_t i = 0; i < root->numChildren(); ++i)
  {
    this->update(root->getChild(i), dt);
  }
}

void Scene::updateAnimTime(Node* root, float dt)
{
  auto meshes = root->getComponents(
    graphics::scene::component::Type::SkeletonMesh);

  for(std::size_t i = 0; i < meshes.size(); ++i)
  {
    auto mesh = (graphics::scene::SkeletonMeshComponent*)meshes[i];
    mesh->updateAnimTime(dt);
  }

  for(std::size_t i = 0; i < root->numChildren(); ++i)
  {
    this->updateAnimTime(root->getChild(i), dt);
  }
}

void Scene::shutdown(Node* root)
{
  auto scripts = root->getComponents(
    graphics::scene::component::Type::Script);

  for(std::size_t i = 0; i < scripts.size(); ++i)
  {
    auto script = (graphics::scene::ScriptComponent*)scripts[i];
    script->shutdown();
  }

  for(std::size_t i = 0; i < root->numChildren(); ++i)
  {
    this->shutdown(root->getChild(i));
  }
}

} // namespace scene
} // namespace graphics
