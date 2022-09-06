from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conan.tools import files
from conan import ConanFile
from conans import tools

required_conan_version = ">=1.48.0"


class SavitarConan(ConanFile):
    name = "savitar"
    license = "LGPL-3.0"
    author = "Ultimaker B.V."
    url = "https://github.com/Ultimaker/libSavitar"
    description = "libSavitar is a c++ implementation of 3mf loading with SIP python bindings"
    topics = ("conan", "cura", "3mf", "c++")
    settings = "os", "compiler", "build_type", "arch"
    revision_mode = "scm"
    exports = "LICENSE*"

    python_requires = "umbase/0.1.5@ultimaker/testing"
    python_requires_extend = "umbase.UMBaseConanfile"

    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "enable_testing": [True, False]
    }
    default_options = {
        "shared": True,
        "fPIC": True,
        "enable_testing": False
    }
    scm = {
        "type": "git",
        "subfolder": ".",
        "url": "auto",
        "revision": "auto"
    }
    def build_requirements(self):
        if self.options.enable_testing:
            for req in self._um_data()["build_requirements_testing"]:
                self.test_requires(req)

    def requirements(self):
        for req in self._um_data()["requirements"]:
            self.requires(req)

    def config_options(self):
        if self.options.shared and self.settings.compiler == "Visual Studio":
            del self.options.fPIC

    def configure(self):
        self.options["pugixml"].shared = self.options.shared

    def validate(self):
        if self.settings.compiler.get_safe("cppstd"):
            tools.check_min_cppstd(self, 17)

    def generate(self):
        cmake = CMakeDeps(self)
        cmake.generate()

        tc = CMakeToolchain(self)
        tc.variables["ENABLE_TESTING"] = self.options.enable_testing
        tc.generate()

    def layout(self):
        cmake_layout(self)
        self.cpp.package.libs = ["Savitar"]

        if self.settings.build_type == "Debug":
            self.cpp.package.defines = ["SAVITAR_DEBUG"]

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        packager = files.AutoPackager(self)
        packager.run()
