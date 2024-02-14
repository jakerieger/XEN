# Third Party Dependencies

This is a list of all the necessary 3rd party dependencies needed to build GLEngine.

- [GLFW](https://github.com/glfw/glfw)
- [glad](https://glad.dav1d.de/)
- [Open**GL** **M**athematics (glm)](https://github.com/g-truc/glm)
- [Assimp](https://github.com/assimp/assimp)
- [EASTL](https://github.com/electronicarts/EASTL)
- [fmtlib](https://github.com/fmtlib/fmt)
- [stb_image](https://github.com/nothings/stb)
- [result](https://github.com/bitwizeshift/result)
- [msgpack-c](https://github.com/msgpack/msgpack-c)

The [CMakeLists.txt](CMakeLists.txt) assumes these were installed with [vcpkg](https://github.com/microsoft/vcpkg).
Other package managers will require some manual
reconfiguration on your part.

**Glad** and **stb_image** are included in the [Vendor](Vendor) directory. Changes to the targeted OpenGL version (
default is 4.6)
will require generating a new `glad` loader.