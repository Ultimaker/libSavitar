from conan import ConanFile
from conan.tools.build import can_run
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
from conan.tools.env import VirtualRunEnv
from conan.tools.files import copy


class SavitarTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "VirtualRunEnv"
    test_type = "explicit"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def generate(self):
        cmake = CMakeDeps(self)
        cmake.generate()

        venv = VirtualRunEnv(self)
        venv.generate()

        tc = CMakeToolchain(self)
        tc.generate()

        for dep in self.dependencies.values():
            for bin_dir in dep.cpp_info.bindirs:
                copy(self, "*.dll", src = bin_dir, dst = self.build_folder)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()


    def test(self):
        if can_run(self):
            ext = ".exe" if self.settings.os == "Windows" else ""
            prefix_path = "" if self.settings.os == "Windows" else "./"
            self.run(f"{prefix_path}test{ext}", env = "conanrun")
