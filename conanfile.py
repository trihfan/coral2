from conans import ConanFile
import multiprocessing
import os

class coral(ConanFile):
    # Parameters
    name = "coral"
    version = "0.0.1"
    url = "https://github.com/trihfan/coral2"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_find_package"
    requires = [
        "glm/0.9.9.8",
        "assimp/5.1.0",
        "vulkan-headers/1.3.204.1",
        "freetype/2.11.1"
    ]

