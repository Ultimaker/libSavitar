import os

from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
from conan.tools import files
from conan import ConanFile
from conans import tools
from conans.errors import ConanException

required_conan_version = ">=1.46.2"


class SavitarConan(ConanFile):
    name = "savitar"
    license = "LGPL-3.0"
    author = "Ultimaker B.V."
    url = "https://github.com/Ultimaker/libSavitar"
    description = "libSavitar is a c++ implementation of 3mf loading with SIP python bindings"
    topics = ("conan", "python", "binding", "sip", "cura", "3mf", "c++")
    settings = "os", "compiler", "build_type", "arch"
    revision_mode = "scm"
    exports = "LICENSE*"

    python_requires = "umbase/0.1.4@ultimaker/testing", "sipbuildtool/0.1@ultimaker/testing"
    python_requires_extend = "umbase.UMBaseConanfile"

    options = {
        "build_python": [True, False],
        "shared": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "build_python": True,
        "shared": True,
        "fPIC": True,
    }
    scm = {
        "type": "git",
        "subfolder": ".",
        "url": "auto",
        "revision": "auto"
    }

    def requirements(self):
        channel = "" if not self.channel else self.channel
        for req in self._um_data(self.version, channel)["requirements"]:
            self.requires(req)
        if self.options.build_python:
            for req in self._um_data(self.version, channel)["requirements_pysavitar"]:
                self.requires(req)

    def config_options(self):
        if self.options.shared and self.settings.compiler == "Visual Studio":
            del self.options.fPIC

    def configure(self):
        self.options["pugixml"].shared = self.options.shared
        self.options["cpython"].shared = True

    def validate(self):
        if self.settings.compiler.get_safe("cppstd"):
            tools.check_min_cppstd(self, 17)

    def generate(self):
        cmake = CMakeDeps(self)
        cmake.generate()

        tc = CMakeToolchain(self)

        if self.settings.compiler == "Visual Studio":
            tc.blocks["generic_system"].values["generator_platform"] = None
            tc.blocks["generic_system"].values["toolset"] = None

        tc.variables["ALLOW_IN_SOURCE_BUILD"] = True
        tc.variables["BUILD_PYTHON"] = self.options.build_python
        if self.options.build_python:
            sip = self.python_requires["sipbuildtool"].module.SipBuildTool(self)
            sip.configure()
            sip.generate("pySavitar")

            tc.variables["Python_EXECUTABLE"] = self.deps_user_info["cpython"].python
            tc.variables["Python_USE_STATIC_LIBS"] = not self.options["cpython"].shared
            tc.variables["Python_ROOT_DIR"] = self.deps_cpp_info["cpython"].rootpath
            tc.variables["Python_FIND_FRAMEWORK"] = "NEVER"
            tc.variables["Python_FIND_REGISTRY"] = "NEVER"
            tc.variables["Python_FIND_IMPLEMENTATIONS"] = "CPython"
            tc.variables["Python_FIND_STRATEGY"] = "LOCATION"

            if self.options.shared and self.settings.os == "Windows":
                tc.variables["Python_SITELIB_LOCAL"] = self.cpp.build.bindirs[0]
            else:
                tc.variables["Python_SITELIB_LOCAL"] = self.cpp.build.libdirs[0]

        tc.generate()

    def layout(self):
        self.folders.source = "."
        try:
            build_type = str(self.settings.build_type)
        except ConanException:
            raise ConanException("'build_type' setting not defined, it is necessary for cmake_layout()")
        self.folders.build = f"cmake-build-{build_type.lower()}"
        self.folders.generators = os.path.join(self.folders.build, "conan")
        self.cpp.build.libdirs = ["."]
        self.cpp.build.bindirs = ["."]

        # TODO: Should we split Savitar up in two repo's? That would at least allow us to get rid of the components,
        # we would then only need to recompile pyArcus for each python version. Now we also compile Arcus again

        # libsavitar component
        self.cpp.source.components["libsavitar"].includedirs = ["include"]

        self.cpp.build.components["libsavitar"].libs = ["Savitar"]
        self.cpp.build.components["libsavitar"].libdirs = ["."]
        self.cpp.build.components["libsavitar"].bindirs = ["."]

        self.cpp.package.components["libsavitar"].libs = ["Savitar"]
        self.cpp.package.components["libsavitar"].libdirs = ["lib"]
        self.cpp.package.components["libsavitar"].bindirs = ["bin"]
        self.cpp.package.components["libsavitar"].includedirs = ["include"]
        self.cpp.package.components["libsavitar"].requires = ["pugixml::pugixml"]

        # pyarcus component
        if self.options.build_python:
            self.cpp.build.components["pysavitar"].libdirs = [".", os.path.join("pySavitar", "pySavitar")]

            self.cpp.package.components["pysavitar"].libdirs = ["site-packages"]
            self.cpp.package.components["pysavitar"].requires = ["libsavitar", "cpython::cpython"]

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        packager = files.AutoPackager(self)
        packager.patterns.build.lib = ["*.so", "*.so.*", "*.a", "*.lib", "*.dylib", "*.pyd", "*.pyi"]
        packager.run()

        # Workaround for AutoPackager not playing nice with components
        files.rmdir(self, os.path.join(self.package_folder, self.cpp.package.components["libsavitar"].libdirs[0], "CMakeFiles"))
        tools.remove_files_by_mask(os.path.join(self.package_folder, self.cpp.package.components["libsavitar"].bindirs[0]), "*.exe")
        files.rmdir(self, os.path.join(self.package_folder, self.cpp.package.components["libsavitar"].bindirs[0], "CMakeFiles"))
        tools.remove_files_by_mask(os.path.join(self.package_folder, self.cpp.package.components["libsavitar"].libdirs[0]), "pySavitar.*")
        files.rmdir(self, os.path.join(self.package_folder, self.cpp.package.components["libsavitar"].libdirs[0], "pySavitar"))
        if self.options.build_python:
            files.rmdir(self, os.path.join(self.package_folder, self.cpp.package.components["pysavitar"].libdirs[0], "CMakeFiles"))
            tools.remove_files_by_mask(os.path.join(self.package_folder, self.cpp.package.components["pysavitar"].libdirs[0], "pySavitar"), "pySavitar.*")
            files.rmdir(self, os.path.join(self.package_folder, self.cpp.package.components["pysavitar"].libdirs[0], "pySavitar"))
            tools.remove_files_by_mask(os.path.join(self.package_folder, self.cpp.package.components["pysavitar"].libdirs[0]), "Savitar.*")
            tools.remove_files_by_mask(os.path.join(self.package_folder, self.cpp.package.components["pysavitar"].libdirs[0]), "libSavitar.*")

    def package_info(self):
        if self.options.build_python:
            if self.in_local_cache:
                self.runenv_info.append_path("PYTHONPATH", self.cpp_info.components["pysavitar"].lib_paths[0])
            else:
                self.runenv_info.append_path("PYTHONPATH", self.build_folder)
