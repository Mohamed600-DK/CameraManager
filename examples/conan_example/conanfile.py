import os

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout,CMakeToolchain

class camera_managerTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("camera_manager/0.1")
        

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)