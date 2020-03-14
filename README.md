# OpenGL Engine
An OpenGL based rendering engine under development.

## Building
### Windows
1. Install [vcpkg](https://github.com/microsoft/vcpkg) and [cmake](https://cmake.org/install/)
2. Use vcpkg to install glfw3, assimp, glew, and glm - `vcpkg install glfw3 assimp glew glm`
3. Clone and navigate to this repository in a command line
4. Make a build directory and navigate to it
5. Run `cmake [path to source] -DCMAKE_TOOLCHAIN_FILE="[vcpkg repo location]/scripts/buildsystems/vcpkg.cmake"`
6. Open the solution and build

### Linux
1. Install cmake, glfw, glew, glm, and assimp using your distro's package manager
2. Build using CMake & Make
