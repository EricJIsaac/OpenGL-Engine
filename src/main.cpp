#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cstdlib>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <chrono>
#include <memory>
#include <iostream>
#include <exception>

#include "opengl_engine.h"

using namespace graphics::scene;
using namespace engine::scripts;

const std::string current_date_time() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

void screenshot_callback(int width, int height) {
    std::string file_name = current_date_time() + ".ppm";

    void* pixels = malloc(4 * width * height);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    io::image::save_ppm(file_name.c_str(), width, height, 4, pixels);
    free(pixels);
}

engine::input::InputMap inputMap;
auto input_callback(GLFWwindow* window, int key, int s, int action, int m)
{
  inputMap.key_callback(window,key,s,action,m);

  if(key == GLFW_KEY_P && action == GLFW_RELEASE) {
      int width, height;
      glfwGetWindowSize(window, &width, &height);
      screenshot_callback(width, height);
  }
};

void window_size_callback(GLFWwindow* window, int width, int height)
{
    float aspect = 0.75; // h/w
    float diff_h = 0;
    float diff_w = 0;

    if(height / width > aspect) {
      // 'too tall'
      diff_w = height / aspect - width;
      diff_w = diff_w / 2;
    } else if (height / width < aspect){
      // 'too wide'
      diff_h = width * aspect - height;
      diff_h = diff_h / 2;
    }

    glViewport(-diff_w, -diff_h, width + diff_w, height + diff_h);
}

namespace graphics {
  class GlException: public std::exception
  {
    virtual const char* what() const throw()
    {
      return "An OpenGL error occured";
    }
  } glException;
} // namespace graphics;

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

  if(type == GL_DEBUG_TYPE_ERROR) {
    throw graphics::glException;
  }
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
  glfwSetWindowSizeCallback(window, window_size_callback);

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

      glm::vec4 eye =
        //cameraNode->getTransform().getAbsoluteMatrix() *
        glm::vec4(cameraNode->getTransform().getTranslation(), 0);

      glUseProgram(mesh_shader.get());
      mb.predraw();
      glUniform3fv(mesh_shader.uniform("eye"), 1, &eye[0]);
      graphics::ogl::render(scene.getRoot(), mb, mv, mesh_shader);
      mb.postdraw();

      glUseProgram(smesh_shader.get());
      smb.predraw();
      glUniform3fv(smesh_shader.uniform("eye"), 1, &eye[0]);
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
