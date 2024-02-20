import os
import subprocess
import sys

vcpkg_dir = os.getcwd().replace("Scripts", "vcpkg")
cfg_type = sys.argv[1]


def config_debug():
    subprocess.check_call(["cmake", "-B", "build/Debug", "-DCMAKE_BUILD_TYPE=Debug", "-S", ".",
                           "-DCMAKE_TOOLCHAIN_FILE={}".format(
                               os.path.join(vcpkg_dir, "scripts", "buildsystems", "vcpkg.cmake"))])


def config_release():
    subprocess.check_call(["cmake", "-B", "build/Release", "-DCMAKE_BUILD_TYPE=Release", "-S", ".",
                           "-DCMAKE_TOOLCHAIN_FILE={}".format(
                               os.path.join(vcpkg_dir, "scripts", "buildsystems", "vcpkg.cmake"))])


if cfg_type == "":
    cfg_type = "All"

if cfg_type == "Debug" or cfg_type == "debug":
    config_debug()
elif cfg_type == "Release" or cfg_type == "release":
    config_release()
elif cfg_type == "All":
    config_debug()
    config_release()
