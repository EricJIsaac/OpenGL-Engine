#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <sstream>
#include <string>
#include <fstream>

#include "engine/engine.h"
#include "graphics/graphics.h"

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

void startup(graphics::scene::Node* root)
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

void update(graphics::scene::Node* root, float dt)
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
    update(root->getChild(i), dt);
  }
}

void shutdown(graphics::scene::Node* root)
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
    shutdown(root->getChild(i));
  }
}

void update_anim_time(graphics::scene::Node* root, float dt)
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
    update_anim_time(root->getChild(i), dt);
  }
}

void render(
  graphics::scene::Node* root,
  graphics::ogl::MeshBuffer& mb,
  glm::mat4 start,
  const graphics::ogl::Shader& shader)
{
  if(root == nullptr)
  {
    return;
  }

  auto meshes = root->getComponents(
    graphics::scene::component::Type::Mesh);

  glm::mat4 mvp = start * root->getTransform().getMatrix();
  glUniformMatrix4fv(shader.uniform("mvp"), 1, GL_FALSE, &mvp[0][0]);

  for(std::size_t i = 0; i < meshes.size(); ++i)
  {
    auto mc = (graphics::scene::MeshComponent*)meshes[i];
    mb.draw(mc->getId());
  }

  for(std::size_t i = 0; i < root->numChildren(); ++i)
  {
    render(root->getChild(i), mb, mvp, shader);
  }
}

void render(
  graphics::scene::Node* root,
  graphics::ogl::SkeletonMeshBuffer& mb,
  glm::mat4 start,
  const graphics::ogl::Shader& shader)
{
  if(root == nullptr)
  {
    return;
  }

  auto meshes = root->getComponents(
    graphics::scene::component::Type::SkeletonMesh);

  glm::mat4 mvp = start * root->getTransform().getMatrix();
  glUniformMatrix4fv(shader.uniform("mvp"), 1, GL_FALSE, &mvp[0][0]);

  for(std::size_t i = 0; i < meshes.size(); ++i)
  {
    auto smc = (graphics::scene::SkeletonMeshComponent*)meshes[i];
    if(!smc->isAnimating())
    {
      continue;
    }

    std::vector<glm::mat4x4> bpos;
    mb.getBonePositions(smc->getId(),
			smc->getAnimName(),
			smc->getAnimTime(),
			bpos);

    glUniformMatrix4fv(shader.uniform("bpos"), 50, GL_FALSE, (const GLfloat*)(&bpos[0]));
    mb.draw(smc->getId());
  }

  for(std::size_t i = 0; i < root->numChildren(); ++i)
  {
    render(root->getChild(i), mb, mvp, shader);
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

  inputMap.addAxisKey(GLFW_KEY_W, AXIS_VERTICAL, 1);
  inputMap.addAxisKey(GLFW_KEY_S, AXIS_VERTICAL, -1);
  inputMap.addAxisKey(GLFW_KEY_D, AXIS_HORIZONTAL, 1);
  inputMap.addAxisKey(GLFW_KEY_A, AXIS_HORIZONTAL, -1);
  inputMap.addAxisKey(GLFW_KEY_R, AXIS_SKYWARD, 1);
  inputMap.addAxisKey(GLFW_KEY_F, AXIS_SKYWARD, -1);
  inputMap.addAxisKey(GLFW_KEY_Q, AXIS_ROTATIONAL, 1);
  inputMap.addAxisKey(GLFW_KEY_E, AXIS_ROTATIONAL, -1);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  graphics::data::Mesh* teapot = nullptr;
  if(!graphics::data::mesh::load("data/meshes/teapot.obj", teapot))
  {
    printf("Failed to load teapot\n");
    return -1;
  }

  graphics::data::SkeletonMesh* simple_cube = nullptr;
  if(!graphics::data::mesh::load("data/anim/simple_cube_1.dae", simple_cube))
  {
    printf("Failed to load animated cube\n");
    return -1;
  }

  graphics::data::SkeletonMesh* simple_cube_2 = nullptr;
  if(!graphics::data::mesh::load("data/anim/simple_cube_2.dae", simple_cube_2))
  {
    printf("Failed to load animated cube 2\n");
    return -1;
  }

  std::vector<graphics::data::Mesh*> meshes;
  meshes.push_back(teapot);

  graphics::ogl::MeshBuffer mb;
  mb.generate(meshes);

  std::vector<graphics::data::SkeletonMesh*> smeshes;
  smeshes.push_back(simple_cube);
  smeshes.push_back(simple_cube_2);

  graphics::ogl::SkeletonMeshBuffer smb;
  smb.generate(smeshes);

  std::vector<GLuint> shader_ids;
  shader_ids.push_back
    (graphics::ogl::shader::compile("data/shaders/mesh.vs", GL_VERTEX_SHADER));
  shader_ids.push_back
    (graphics::ogl::shader::compile("data/shaders/mesh.fs", GL_FRAGMENT_SHADER));

  GLuint mesh_program_id = graphics::ogl::shader::link(shader_ids);
  auto mesh_shader = graphics::ogl::Shader(mesh_program_id);

  shader_ids.clear();
  shader_ids.push_back
    (graphics::ogl::shader::compile("data/shaders/skeleton_mesh.vs", GL_VERTEX_SHADER));
  shader_ids.push_back
    (graphics::ogl::shader::compile("data/shaders/skeleton_mesh.fs", GL_FRAGMENT_SHADER));

  GLuint smesh_program_id = graphics::ogl::shader::link(shader_ids);
  auto smesh_shader = graphics::ogl::Shader(smesh_program_id);

  glfwSetKeyCallback(window, input_callback);

  auto rootNode = new graphics::scene::Node();

  auto cameraNode = new graphics::scene::Node();
  auto camera = new graphics::scene::CameraComponent();
  auto cameraScript = new engine::scripts::CameraScript();
  cameraScript->setInputMap(&inputMap);

  rootNode->addChild(cameraNode);
  cameraNode->addComponent(camera);
  cameraNode->addComponent(cameraScript);
  cameraNode->getTransform().setTranslation(glm::vec3(-10, 0, 0));

  auto mn1 = new graphics::scene::Node();
  auto mc1 = new graphics::scene::SkeletonMeshComponent();

  rootNode->addChild(mn1);
  mn1->getTransform().setTranslation(glm::vec3(-1, 0, 0));
  mn1->getTransform().setScale(glm::vec3(1,1,1));
  mn1->getTransform().setRotation(glm::vec3(0, 0, 0));
  mn1->addComponent(mc1);
  mc1->setId(0);
  mc1->setAnimName("default_0");
  mc1->setAnimTime(0.1f);

  auto mn2 = new graphics::scene::Node();
  auto mc2 = new graphics::scene::MeshComponent();

  rootNode->addChild(mn2);
  mn2->getTransform().setTranslation(glm::vec3(-15, -1, -15));
  mn2->getTransform().setScale(glm::vec3(0.3,0.3,0.3));
  mn2->getTransform().setRotation(glm::vec3(0, 0, 0));
  mn2->addComponent(mc2);
  mc2->setId(0);

  auto mn3 = new graphics::scene::Node();
  auto mc3 = new graphics::scene::SkeletonMeshComponent();

  rootNode->addChild(mn3);
  mn3->getTransform().setTranslation(glm::vec3(-1, -5, -5));
  mn3->addComponent(mc3);
  mc3->setAnimName("default_0");
  mc3->setId(1);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
  glEnable(GL_DEPTH_TEST);

  startup(rootNode);

  do{
    float dt = 0.016f;
    update(rootNode, dt);
    update_anim_time(rootNode, dt);

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 mv =
      camera->getProjectionMatrix() *
      camera->getViewMatrix() *
      glm::mat4(1.0f);

    glUseProgram(mesh_shader.get());
    mb.predraw();
    render(rootNode, mb, mv, mesh_shader);
    mb.postdraw();

    glUseProgram(smesh_shader.get());
    smb.predraw();
    render(rootNode, smb, mv, smesh_shader);
    smb.postdraw();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

  mb.destroy();
  smb.destroy();

  shutdown(rootNode);
  destroy(rootNode);
}
