from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout,CMakeDeps,CMakeToolchain
import conan.tools


opencv_opts = {
    "highgui": True,
    "imgproc": True,
    "imgcodecs": True,
    "videoio": True,
    "dnn": False,
    "objdetect": False,
    "photo": False,
    "ml": False,
    "flann": False,
    "gapi": False,
    "calib3d": False,
    "stitching": False,
    "video": False,
    "features2d": False,
#------------------------------------------------------------------------#
    "parallel": False,
    "with_ipp": False,
    "with_eigen": False,
    "neon": False,
    "with_opencl": False,
    "with_cuda": False,
    "with_cublas": False,
    "with_cufft": False,
    "with_cudnn": False,
    "cuda_arch_bin": None,
    "cpu_baseline": None,
    "cpu_dispatch": None,
    "world": True,
    "nonfree": False,
    # DNN module not required
    "with_flatbuffers": False,
    "with_protobuf": False,
    "with_vulkan": False,
    "with_openvino": False,
    "dnn_cuda": False,
    # HIGHGUI module is needed for cv::imshow
    "with_gtk": True,       # For Linux (X11/Wayland support)
    "with_qt": False,
    "with_wayland": False,   # If Wayland is used
    "with_msmf": True,      # Needed for Windows Media Foundation support
    "with_msmf_dxva": False,
    # Image codecs needed for imencode(".jpg")
    "with_avif": False,
    "with_jpeg": "libjpeg-turbo",  # For cv::imencode(".jpg", ...)
    "with_png": True,
    "with_tiff": False,
    "with_jpeg2000": False,
    "with_openexr": False,
    "with_webp": False,
    "with_gdal": False,
    "with_gdcm": False,
    "with_imgcodec_hdr": False,
    "with_imgcodec_pfm": False,
    "with_imgcodec_pxm": False,
    "with_imgcodec_sunraster": False,
    # objdetect not required
    "with_quirc": False,
    # Required video I/O options
    "with_ffmpeg": True,  # Best general backend for video I/O
    # Tesseract not required
    "with_tesseract": False,
        }
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
        if self.settings.os == "Linux":
            self.options["opencv"].with_v4l = True
        # Enable only selected features for OpenCV and disable the rest
        for opt, val in opencv_opts.items():
            self.options["opencv"].__setattr__(opt, val)



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
