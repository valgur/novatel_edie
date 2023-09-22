import os

from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, cmake_layout, CMakeToolchain, CMakeDeps
from conan.tools.files import copy, rmdir, move_folder_contents, mkdir

required_conan_version = ">=2.0"


class NovatelEdieConan(ConanFile):
    name = "novatel_edie"
    version = "3.2.24"
    description = ("The EDIE (Encode-Decode Interface Engine) SDK allows interfacing and decoding "
                   "data output from NovAtel's OEM7 receivers.")
    url = "https://github.com/novatel/novatel_edie"
    license = "MIT"
    package_type = "library"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_dynamic_libs": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "build_dynamic_libs": True,
    }
    options_description = {
        "shared": "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)",
        "fPIC": "Build with -fPIC",
        "build_dynamic_libs": "Build additional C-style *_dynamic_library versions of the libraries",
    }

    exports_sources = ["cmake/*", "database/*", "src/*", "LICENSE", "!doc", "!test", "CMakelists.txt"]

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("nlohmann_json/3.11.2", transitive_headers=True)
        self.requires("spdlog/1.12.0", transitive_headers=True)

    def validate(self):
        if self.settings.compiler.cppstd:
            check_min_cppstd(self, 17)

    def build_requirements(self):
        self.test_requires("gtest/1.14.0")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["BUILD_DYNAMIC_LIBS"] = self.options.build_dynamic_libs
        tc.cache_variables["BUILD_TESTS"] = False
        tc.cache_variables["BUILD_EXAMPLES"] = False
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        copy(self, "LICENSE",
             src=self.source_folder,
             dst=os.path.join(self.package_folder, "licenses"))
        mkdir(self, os.path.join(self.package_folder, "res"))
        move_folder_contents(self, os.path.join(self.package_folder, "share", "novatel", "edie"),
                                   os.path.join(self.package_folder, "res"))
        rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "EDIE")
        self.cpp_info.set_property("cmake_target_name", "EDIE::EDIE")
        self.cpp_info.includedirs.append(os.path.join("include", "novatel", "edie"))
        self.cpp_info.resdirs = ["res"]
        self.cpp_info.libs = ["novatel", "stream_interface"]
        if self.options.build_dynamic_libs:
            self.cpp_info.libs.extend(["decoders_dynamic_library", "hwinterface_dynamic_library"])
