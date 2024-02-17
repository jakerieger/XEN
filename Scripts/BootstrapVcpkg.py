import os
import subprocess

print("This script will automate the following:")
print("  - Install vcpkg")
print("  - Bootstrap vcpkg")
print("  - Install vcpkg dependencies")
print("  - Configure CMake")
proceed = input("Continue? (Y/n) ")

if proceed == "n":
    print("Exiting...")
    exit(0)

vcpkg_repo = "https://github.com/microsoft/vcpkg"
install_dir = os.getcwd().replace("Scripts", "vcpkg")

subprocess.check_call(["git", "clone", vcpkg_repo, install_dir])

os.chdir(install_dir)
subprocess.check_call(["git", "checkout", "master"])
subprocess.check_call(["git", "pull"])

if os.name == "nt":
    subprocess.check_call(["./bootstrap-vcpkg.bat"])
else:
    subprocess.check_call(["./bootstrap-vcpkg.sh"])

subprocess.check_call(["./vcpkg", "update"])

packages = [
    'glfw3',
    'glm',
    'assimp',
    'eastl',
    'fmt',
    'msgpack',
    'catch2'
]

for pkg in packages:
    subprocess.check_call(["./vcpkg", "install", pkg])

subprocess.check_call(["./vcpkg", "update"])

os.chdir(os.path.join(install_dir, "../"))
subprocess.check_call(["cmake", "-B", "build/Debug", "-DCMAKE_BUILD_TYPE=Debug", "-S", ".",
                       "-DCMAKE_TOOLCHAIN_FILE={}".format(
                           os.path.join(install_dir, "scripts", "buildsystems", "vcpkg.cmake"))])
