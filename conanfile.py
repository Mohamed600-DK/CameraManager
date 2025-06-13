from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout,CMakeDeps,CMakeToolchain
import conan.tools

class CameraManagerConan(ConanFile):
    name = "camera_manager"
    version = "0.1"
    license = "MIT"
    author = "MohamedAhmedAbdelaal,mohamedahmed_dk@hotmail.com"
    url = "https://github.com/mohamed600-dk/CameraManager"
    description = "A C++ camera management library using OpenCV"
    topics = ("camera", "opencv", "image-processing")
    
    settings = "os", "compiler", "build_type", "arch"
    requires = "opencv/4.8.1"
    exports_sources = "CMakeLists.txt", "include/*", "src/*"
    package_type = "library"
    build_policy = "missing"
    options = {
        "shared": [True, False],
    }
    default_options = {
        "shared": False,
    }

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")
    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
            self.tools
    def layout(self):
        cmake_layout(self)
    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["CAMERA_MANAGER_SKIP_CONFIG"] = True
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    def package(self):
        cmake = CMake(self)
        cmake.install()
    def package_info(self):
        self.cpp_info.libs = ["camera_manager"]
