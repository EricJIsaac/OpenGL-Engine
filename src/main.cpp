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

  GLuint VertexArrayId;
  glGenVertexArrays(1, &VertexArrayId);
  glBindVertexArray(VertexArrayId);

  std::size_t s1 = quad1->vertices.size() * sizeof(float);
  std::size_t s2 = quad2->vertices.size() * sizeof(float);

  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, s1 + s2, 0, GL_STATIC_DRAW);

  glBufferSubData(GL_ARRAY_BUFFER, 0, s1, &quad1->vertices[0]);
  glBufferSubData(GL_ARRAY_BUFFER, s1, s2, &quad2->vertices[0]);

  s1 = quad1->indices.size() * sizeof(unsigned short);
  s2 = quad2->indices.size() * sizeof(unsigned short);

  GLuint elementbuffer;
  glGenBuffers(1, &elementbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, s1 + s2, 0, GL_STATIC_DRAW);

  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, s1, &quad1->indices[0]);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, s1, s2, &quad2->indices[0]);

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

  do{
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programId);

    glm::mat4 mvp =
      camera->getProjectionMatrix() *
      camera->getViewMatrix() *
      glm::mat4(1.0f);

    glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    char* offset = (char*)0 + 6 * sizeof(unsigned short);
    glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)offset, 4);

    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

  glDeleteBuffers(1, &vertexbuffer);
  glDeleteBuffers(1, &elementbuffer);

  delete camera;
  delete cameraNode;
}
