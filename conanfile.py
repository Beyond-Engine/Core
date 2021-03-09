from conans import ConanFile, CMake


class BeyondCoreConan(ConanFile):
    name = "beyond_core"
    version = "0.0.2"
    license = "MIT"
    author = "Lesley Lai lesley@lesleylai.info"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of BeyondCore here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "cmake"
    exports_sources = ["cmake/*", "include/*", "src/*", "third-party/*", "CMakeLists.txt"]
    requires = ["fmt/7.1.3", "backward-cpp/1.5"]
    build_requires = ["catch2/2.13.4"]

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".")
        cmake.build()

    def package(self):
        self.copy("*.hpp", dst="include", src="include")
        self.copy("*.h", dst="include", src="include")
        self.copy("*hello.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["core"]
