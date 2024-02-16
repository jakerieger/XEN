# Third Party Dependencies

GLEngine relies on several third-party libraries to handle many tasks like model loading and UI rendering.
Here's a list of all the necessary 3rd party dependencies needed to build GLEngine.

- [GLFW](https://github.com/glfw/glfw) - Cross-platform window management
- [glad](https://glad.dav1d.de/) - Modern OpenGL loader
- [Open**GL** **M**athematics (glm)](https://github.com/g-truc/glm) - Performance-optimized linear algebra library
- [Assimp](https://github.com/assimp/assimp) - 3D model loading
- [EASTL](https://github.com/electronicarts/EASTL) - Performance-optimized **S**tandard **T**emplate **L**ibrary
- [fmtlib](https://github.com/fmtlib/fmt) - Advanced string formatting
- [stb_image](https://github.com/nothings/stb) - Image loading
- [result](https://github.com/bitwizeshift/result) - Modern error-handling used to avoid exceptions
- [msgpack-c](https://github.com/msgpack/msgpack-c) - Binary data serialization/deserialization

All dependencies have been tested and installed with [vcpkg](https://github.com/microsoft/vcpkg). Other package managers
may require manual configuration on your part.

If you have Python installed, you can run the [BootstrapVcpkg.py](Scripts/BootstrapVcpkg.py) script
in [Scripts](Scripts)
to automatically download Vcpkg, the required dependencies, and bootstrap CMake to use them. This will
install vcpkg and the dependencies in the project root directory.

**Glad** and **stb_image** are included in the [Vendor](Vendor) directory. Changes to the targeted OpenGL version (
default is 4.6)
will require generating a new `glad` loader.

OpenGL extension `GL_NVX_gpu_memory_info` is required for
the profiler to be able to query GPU memory data. Support for AMD/Intel devices
is coming soon.