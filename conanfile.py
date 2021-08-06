import os
import pathlib

from conans import ConanFile, tools
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake

class SavitarConan(ConanFile):
    name = "Savitar"
    version = "4.10.0"
    license = "LGPL-3.0"
    author = "Ultimaker B.V."
    url = "https://github.com/Ultimaker/libSavitar"
    description = "libSavitar is a c++ implementation of 3mf loading with SIP python bindings"
    topics = ("conan", "python", "binding", "sip", "cura", "3mf", "c++")
    settings = "os", "compiler", "build_type", "arch"
    exports = "LICENSE"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "tests": [True, False],
        "python_version": "ANY"
    }
    default_options = {
        "shared": True,
        "fPIC": True,
        "tests": False,
        "python_version": "3.8"
    }
    scm = {
        "type": "git",
        "subfolder": ".",
        "url": "auto",
        "revision": "auto"
    }

    # TODO: Move lib naming logic to python_requires project
    _ext = None

    @property
    def ext(self):
        if self._ext:
            return self._ext
        self._ext = "lib" if self.settings.os == "Windows" else "a"
        if self.options.shared:
            if self.settings.os == "Windows":
                self._ext = "dll"
            elif self.settings.os == "Macos":
                self._ext = "dylib"
            else:
                self._ext = "so"
        return self._ext

    @property
    def lib_name(self):
        return self.name

    def configure(self):
        self.options["SIP"].shared = self.options.shared
        self.options["SIP"].python_version = self.options.python_version
        self.options["pugixml"].shared = self.options.shared
        if self.options.shared or self.settings.compiler == "Visual Studio":
            del self.options.fPIC

    def build_requirements(self):
        self.build_requires("cmake/[>=3.16.2]")
        if self.options.tests:
            self.build_requires("gtest/[>=1.10.0]", force_host_context=True)

    def requirements(self):
        self.requires("SIP/[>=4.19.24]@riverbankcomputing/testing")
        self.requires("pugixml/1.8.1@bincrafters/stable")

    def validate(self):
        if self.settings.compiler.get_safe("cppstd"):
            tools.check_min_cppstd(self, 17)

    def generate(self):
        cmake = CMakeDeps(self)
        cmake.generate()

        tc = CMakeToolchain(self)

        # FIXME: This shouldn't be necessary (maybe a bug in Conan????)
        if self.settings.compiler == "Visual Studio":
            tc.blocks["generic_system"].values["generator_platform"] = None
            tc.blocks["generic_system"].values["toolset"] = None

        tc.variables["SIP_MODULE_SITE_PATH"] = "site-packages"
        tc.variables["USE_SHIPPED_PUGIXML"] = False
        tc.variables["Python_VERSION"] = self.options.python_version
        tc.variables["BUILD_TESTS"] = self.options.tests
        tc.generate()

    _cmake = None

    def configure_cmake(self):
        if self._cmake:
            return self._cmake
        self._cmake = CMake(self)
        self._cmake.configure()
        return self._cmake

    def build(self):
        cmake = self.configure_cmake()
        cmake.build()

    def package(self):
        cmake = self.configure_cmake()
        cmake.install()
        self.copy("*", src = os.path.join("package", "include"), dst = "include")
        self.copy("*", src = os.path.join("package", "lib"), dst = "lib", excludes = "*.cmake")
        self.copy("*", src = os.path.join("package", "site-packages"), dst = "site-packages")
        self.copy("*.pyi", src = ".", dst = "site-packages")

    def package_info(self):
        self.cpp_info.libs = [self.lib_name]
        if self.in_local_cache:
            self.runenv_info.prepend_path("PYTHONPATH", os.path.join(self.package_folder, "site-packages"))
            self.cpp_info.includedirs = ["include"]
            self.cpp_info.libdirs = ["lib"]
        else:
            local_build_path = os.path.join(str(pathlib.Path(__file__).parent.absolute())),\
                               f"cmake-build{self.settings.build_type}".lower()
            self.runenv_info.prepend_path("PYTHONPATH", local_build_path)
            self.cpp_info.includedirs = ["src"]
            self.cpp_info.libdirs = [local_build_path]
        self.cpp_info.names["cmake_find_package"] = self.name
        self.cpp_info.names["cmake_find_package_multi"] = self.name
        if self.settings.build_type == "Debug":
            self.cpp_info.defines.append("SAVITAR_DEBUG")
        if self.settings.os in ["Linux", "FreeBSD", "Macos"]:
            self.cpp_info.system_libs.append("pthread")