#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <sstream>
#include <string>
#include <fstream>

#include "graphics/graphics.h"

void render(
  graphics::scene::Node* root,
  graphics::ogl::MeshBuffer& mb,
  glm::mat4 start,
  GLuint matrixId)
{
  if(root == nullptr)
  {
    return;
  }

  auto meshes = root->getComponents(
    graphics::scene::component::Type::Mesh);

  glm::mat4 mvp = start * root->getTransform().getMatrix();
  glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

  for(std::size_t i = 0; i < meshes.size(); ++i)
  {
    mb.draw(((graphics::scene::MeshComponent*)meshes[i])->getId());
  }

  for(std::size_t i = 0; i < root->numChildren(); ++i)
  {
    render(root->getChild(i), mb, mvp, matrixId);
  }
}

void destroy(graphics::scene::Node* root)
{
  for(std::size_t i = 0; i < root->numChildren(); ++i)
  {
    destroy(root->getChild(i));
  }

  for(std::size_t i = 0; i < root->numComponents(); ++i)
  {
    delete root->getComponent(i);
  }

  delete root;
}

int main() {
  glewExperimental = true;
  if(!glfwInit())
  {
    fprintf(stderr, "Failed to initialize GLFW\n");
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window;
  window = glfwCreateWindow(1024,768, "opengl", NULL, NULL);
  if(window == nullptr)
  {
    fprintf(stderr, "Failed to open GLFW window.");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glewExperimental=true;
  if(glewInit() != GLEW_OK)
  {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  auto quad1 = new graphics::data::Mesh();
  quad1->vertices = std::vector<float>
    ({ -1.0f, -1.0f, 0.0f,
				 1.0f, -1.0f, 0.0f,
				 -1.0f, 1.0f, 0.0f,
				 1.0f, 1.0f, 0.0f});
  quad1->indices = std::vector<unsigned short>(
    {0,1,2,1,3,2});

  auto quad2 = new graphics::data::Mesh();
  quad2->vertices = std::vector<float>
    ({ -2.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f});
  quad2->indices = std::vector<unsigned short>(
    {0,1,2,1,3,2});

  std::vector<graphics::data::Mesh*> meshes;
  meshes.push_back(quad1);
  meshes.push_back(quad2);

  graphics::ogl::MeshBuffer mb;
  mb.generate(meshes);

  std::vector<GLuint> shaderIds;
  shaderIds.push_back
    (graphics::ogl::shader::compile("data/shaders/simple.vs", GL_VERTEX_SHADER));
  shaderIds.push_back
    (graphics::ogl::shader::compile("data/shaders/simple.fs", GL_FRAGMENT_SHADER));

  GLuint programId = graphics::ogl::shader::link(shaderIds);
  GLuint matrixId = glGetUniformLocation(programId, "uniform_mvp");

  auto cameraNode = new graphics::scene::Node();
  auto camera = new graphics::scene::CameraComponent();

  cameraNode->addComponent(camera);
  cameraNode->getTransform().setTranslation(glm::vec3(4, 3, 3));
  camera->setTarget(glm::vec3(0,0,0));

  auto rootNode = new graphics::scene::Node();

  auto mn1 = new graphics::scene::Node();
  auto mc1 = new graphics::scene::MeshComponent();

  rootNode->addChild(mn1);
  mn1->getTransform().setTranslation(glm::vec3(0, -1, 0));
  mn1->addComponent(mc1);
  mc1->setId(1);

  auto mn2 = new graphics::scene::Node();
  auto mc2 = new graphics::scene::MeshComponent();

  rootNode->addChild(mn2);
  mn2->getTransform().setTranslation(glm::vec3(0, 1, 0));
  mn2->addComponent(mc2);
  mc2->setId(0);

  do{
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programId);

    glm::mat4 mv =
      camera->getProjectionMatrix() *
      camera->getViewMatrix() *
      glm::mat4(1.0f);

    mb.predraw();
    render(rootNode, mb, mv, matrixId);
    mb.postdraw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

  mb.destroy();
  destroy(rootNode);

  delete quad1;
  delete quad2;
}
