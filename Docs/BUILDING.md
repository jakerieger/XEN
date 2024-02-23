![](img/XENEngineBanner.png)

# Building XEN

This guide will give you a quick rundown of all the steps required to build and
start using XEN.

First, these are all the tools you'll need before cloning the source:

1. C++ Compiler (I'd recommend Clang)
2. CMake 3.25 or newer
3. Python 3 (optional, needed to run [scripts](../Scripts))

## 1. Clone the repo

```bash
$ git clone https://github.com/jakerieger/XEN.git
```

## 2. Install 3rd party dependencies

See [DEPENDENCIES](../DEPENDENCIES.md) for instructions and a list of required dependencies.

## 3. Configure CMake

From the project root, run:

```bash
$ cmake -B build/Debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake
```

## 4. Compile XEN

If you're on Linux, you can simply run `make` from the build directory:

```bash
$ cd build/Debug
$ sudo make
```

If you're on Windows, you can open the generated Visual Studio solution and compile, or use Ninja
if you're not using MSVC.