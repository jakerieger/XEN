# Third Party Dependencies

## ⚠️ Necessary

XEN relies on several third-party libraries to handle many tasks like model loading and UI rendering.
Here's a list of all the necessary 3rd party dependencies needed to build XEN.

- [GLFW](https://github.com/glfw/glfw) - Cross-platform window management
- [glad](https://glad.dav1d.de/) - Modern OpenGL loader
- [Open**GL** **M**athematics (glm)](https://github.com/g-truc/glm) - Performance-optimized linear algebra library
- [Assimp](https://github.com/assimp/assimp) - 3D model loading
- [EASTL](https://github.com/electronicarts/EASTL) - Performance-optimized **S**tandard **T**emplate **L**ibrary.
  *Entirely replaces the std STL in XEN.* See [Using EASTL](#-using-eastl) for more information.
- [fmtlib](https://github.com/fmtlib/fmt) - Advanced string formatting
- [stb_image](https://github.com/nothings/stb) - Image loading
- [result](https://github.com/bitwizeshift/result) - Modern error-handling used to avoid exceptions
- [msgpack-c](https://github.com/msgpack/msgpack-c) - Binary data serialization/deserialization
- [Dear ImGui](https://github.com/ocornut/imgui) - Debug and tool UI

## ❔In consideration

These libraries are being considered for integration into XEN.

- [RmlUI](https://github.com/mikke89/RmlUi) - HTML/CSS UI framework for game UI's
- [stduuid](https://github.com/mariusbancila/stduuid) - UUID generation library for C++17
- [Boost.Asio](https://www.boost.org/doc/libs/1_76_0/doc/html/boost_asio.html) - Networking IO library
- [FMOD](https://www.fmod.com/) - Proprietary audio library
- [Bullet](https://github.com/bulletphysics/bullet3) - Open-source physics library
- [DirectInput](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416842(v=vs.85)) - First-party
  input framework for Windows, **not** cross-platform.

### 🛈 Notes

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
the profiler to be able to query GPU memory data on Nvidia hardware. Support for AMD/Intel devices
is coming soon.

### 🛈 Using EASTL

XEN uses EASTL in place of the standard C++ STL. Type names and methods are all identical to the standard STL,
so code like:

```c++
using namespace std;

vector<int> nums = {1,2,3,4};
size_t numsLen = nums.size();

function<void()> lambdaFunc;
lambdaFunc();

... etc
```

is identical to:

```c++
using namespace eastl;

vector<int> nums = {1,2,3,4};
size_t numsLen = nums.size();

function<void()> lambdaFunc;
lambdaFunc();

... etc
```

EA provides a very handy PDF that outlines everything included in
EASTL [here](https://github.com/electronicarts/EASTL/blob/master/doc/quick-reference.pdf).
Adopting EASTL for you game project should require little to no extra effort.