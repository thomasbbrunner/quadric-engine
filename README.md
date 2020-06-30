# quadric works

OpenGL based engine for creative programming in C++17

## Dependencies
Following packages need to be installed:
* GLFW3 (libglfw3-dev)
* GLM (libglm-dev)
* eigen3 (libeigen3-dev)
* glad (python3-glad)
* gcc 7 (or greater)

Optional:
* glslang-tools (for GLSL code linting, requires VS Code extension)

## Building and running
```bash
git clone https://github.com/thomasbbrunner/quadricworks.git
cd quadricworks
mkdir build
cd build
cmake ..
make
cd ../
./bin/quadricworks
```

## TODOs
* Substitute #defines for enums or constexpr
* Remove singletons and global variables
* Add right version to glad loader
* Formatting according to a standard (google style guide)
* Port geometries to templated classes and make generation of vertices run at compile time
* Make entire interface more like a library and less like a framework
    - more control and flexibility to the developer
    - less time for implementation of new features
* Remove deprecated makefile
