from conan import ConanFile
from conan.tools.cmake import cmake_layout


class Cubescape(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("spdlog/1.15.1")
        self.requires("glfw/3.4")
        self.requires("glad/0.1.36")
        self.requires("glm/1.0.1")
        self.requires("entt/3.14.0")

    def configure(self):
        self.options["spdlog"].no_exceptions = True

        self.options["glad"].gl_profile = "core"
        self.options["glad"].gl_version = "4.6"

    def layout(self):
        cmake_layout(self)
