#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <chrono>
#include <memory>

#include "engine/engine.h"
#include "graphics/graphics.h"

using namespace graphics::scene;
using namespace engine::scripts;

engine::input::InputMap inputMap;
auto input_callback(GLFWwindow* w, int k, int s, int a, int m)
{
  inputMap.key_callback(w,k,s,a,m);
};

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

void glfw_error_callback(int error_code, const char* description)
{
  printf("GLFW Error: %s\n", description);
}

int main() {
  glfwSetErrorCallback(glfw_error_callback);

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
    fprintf(stderr, "Failed to open GLFW window.\n");
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

  inputMap.addAxisKey(GLFW_KEY_W, AXIS_VERTICAL, 1);
  inputMap.addAxisKey(GLFW_KEY_S, AXIS_VERTICAL, -1);
  inputMap.addAxisKey(GLFW_KEY_D, AXIS_HORIZONTAL, 1);
  inputMap.addAxisKey(GLFW_KEY_A, AXIS_HORIZONTAL, -1);
  inputMap.addAxisKey(GLFW_KEY_R, AXIS_SKYWARD, 1);
  inputMap.addAxisKey(GLFW_KEY_F, AXIS_SKYWARD, -1);
  inputMap.addAxisKey(GLFW_KEY_Q, AXIS_ROTATIONAL, 1);
  inputMap.addAxisKey(GLFW_KEY_E, AXIS_ROTATIONAL, -1);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  graphics::data::MeshDB meshDB;
  meshDB.load("data/geometry/meshdb.json");

  graphics::ogl::MeshBuffer mb;
  mb.generate(meshDB.getMeshes());

  graphics::ogl::SkeletonMeshBuffer smb;
  smb.generate(meshDB.getSkeletonMeshes());

  std::vector<GLuint> shader_ids;
  shader_ids.push_back
    (graphics::ogl::shader::compile("data/shaders/mesh.vs", GL_VERTEX_SHADER));
  shader_ids.push_back
    (graphics::ogl::shader::compile("data/shaders/mesh.fs", GL_FRAGMENT_SHADER));

  GLuint mesh_program_id = graphics::ogl::shader::link(shader_ids);
  auto mesh_shader = graphics::ogl::Shader(mesh_program_id);

  shader_ids.clear();
  shader_ids.push_back
    (graphics::ogl::shader::compile(
      "data/shaders/skeleton_mesh.vs",
      GL_VERTEX_SHADER));
  shader_ids.push_back
    (graphics::ogl::shader::compile(
      "data/shaders/skeleton_mesh.fs",
      GL_FRAGMENT_SHADER));

  GLuint smesh_program_id = graphics::ogl::shader::link(shader_ids);
  auto smesh_shader = graphics::ogl::Shader(smesh_program_id);

  glfwSetKeyCallback(window, input_callback);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
  glEnable(GL_DEPTH_TEST);

  graphics::scene::Scene scene;
  scene.load("data/scenes/simple.json");

  auto cameraNode = std::unique_ptr<Node>(new Node());
  auto camera = std::unique_ptr<CameraComponent>(new CameraComponent());
  auto cameraScript = std::unique_ptr<CameraScript>(new CameraScript());
  cameraScript->setInputMap(&inputMap);

  scene.getRoot()->addChild(cameraNode.get());
  cameraNode->addComponent(camera.get());
  cameraNode->addComponent(cameraScript.get());
  cameraNode->getTransform().setTranslation(glm::vec3(-10, 0, 0));

  scene.startup();

  typedef std::chrono::high_resolution_clock Time;
  typedef std::chrono::milliseconds ms;
  typedef std::chrono::duration<float> fsec;
  auto last_tick_time = Time::now();

  do{
    fsec ft = Time::now() - last_tick_time;
    ms mst = std::chrono::duration_cast<ms>(ft);
    float dt = ((float) mst.count()) / 1000.f;

    if(dt >= 0.016f)
    {
      last_tick_time = Time::now();

      scene.update(dt);

      glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glm::mat4 mv =
      	camera->getProjectionMatrix() *
      	camera->getViewMatrix() *
      	glm::mat4(1.0f);

      glUseProgram(mesh_shader.get());
      mb.predraw();
      graphics::ogl::render(scene.getRoot(), mb, mv, mesh_shader);
      mb.postdraw();

      glUseProgram(smesh_shader.get());
      smb.predraw();
      graphics::ogl::render(scene.getRoot(), smb, mv, smesh_shader);
      smb.postdraw();

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }
  while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

  mb.destroy();
  smb.destroy();

  scene.shutdown();

  glfwDestroyWindow(window);
  glfwTerminate();
}
