# quadric engine

OpenGL-based engine for creative programming in C++17

<p align="center">
  <img width="400" height="300" src="demos/demo_cubeworld.gif" alt="Demo quadric engine">
</p>

## Dependencies
Following packages need to be installed:
* libglvnd-dev
* GLFW3 (libglfw3-dev)
* GLM (libglm-dev)
* eigen3 (libeigen3-dev)
* glad (python3-glad)
* gcc 7 (or greater)

Optional:
* glslang-tools (for GLSL code linting)

<video width="320" height="240" controls>
  <source src="vid/demo_cubeworld.mp4" type="video/mp4">
Your browser does not support the video tag.
</video>

## Building and running
```bash
git clone https://github.com/thomasbbrunner/quadric-engine.git
cd quadric-engine
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
    - compile as a static library (no main function, wrap in namespace qe/quad/quadric)
    - more control and flexibility to the developer
    - less time for implementation of new features
* Remove deprecated makefile
* Improve shader support
    - add debugging shader (fall back option, only geometry)
    - add support for shader hot reloading (automatic reload on file save, see C++17 std::filesystem)
* Clean up git history (tags)
