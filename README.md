# quadric engine

OpenGL-based engine for creative programming in C++17.

<p align="center">
  <img width="400" height="300" src="demos/demo_cubeworld.gif" alt="Demo quadric engine">
</p>

## Dependencies
Following packages need to be installed:
* libglvnd-dev
* GLFW3 (libglfw3-dev and libglfw3) **not** the wayland version!
* GLM (libglm-dev)
* eigen3 (libeigen3-dev)
* glad (python3-glad)
* gcc

Optional:
* glslang-tools (for GLSL code linting)

## Building and running
```bash
git clone https://github.com/thomasbbrunner/quadric-engine.git
cd quadric-engine
mkdir build
cd build
cmake ..
make
cd ../
./bin/quadric-engine
```

## TODOs
* Remove singletons and global variables
* Add right version to glad loader
* Formatting according to a standard (google style guide)
* Port geometries to templated classes and make generation of vertices run at compile time
* Make entire interface more like a library and less like a framework
    - compile as a static library (no main function, wrap in namespace quadricengine/qe/quad/quadric)
    - more control and flexibility to the developer
    - less time for implementation of new features
* Improve shader support
    - add debugging shader (fall back option, only geometry)
    - add support for shader hot reloading (automatic reload on file save, see C++17 std::filesystem)
* Clean up git history (tags)
* Improve Scene object:
    - Remove automatic merging of geometries, and save each geometry in a vector/array.
    - Instead, allow the creation of a new geometry from merged geometries, which could then be used in a scene.
* Re-add OpenGL ES support
* Do proper C++ casting
* Look into WebGPU
* Don't append all geometries together, scenes should be composed of different geometries. Appending geometries should be done by creating another geometry.
* Make code work properly under wayland
