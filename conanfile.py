from conans import ConanFile, CMake, tools
import os
import sys

class SixConan(ConanFile):
    name = "six-library"
    url = "https://github.com/ngageoint/six-library"
    description = "library for reading and writing the Sensor Independent XML (SIX) format"
    settings = "os", "compiler", "build_type", "arch"
    requires = ("coda-oss/master_67d6362bcfcf07e2",
                "nitro/master_496bfba71889054c")
    options = {"shared": [True, False],
               "PYTHON_HOME": "ANY",
               "PYTHON_VERSION": "ANY",
               "ENABLE_PYTHON": [True, False],
               }
    default_options = {"shared": False,
                       "PYTHON_HOME": "",
                       "PYTHON_VERSION": "",
                       "ENABLE_PYTHON": True,
                       }
    exports_sources = ("CMakeLists.txt",
                       "LICENSE",
                       "README.md",
                       "cmake/*",
                       "croppedNitfs/*",
                       "six/*",
                       )
    generators = "cmake_paths"
    license = "GNU LESSER GENERAL PUBLIC LICENSE Version 3"

    # default to short_paths mode (Windows only)
    short_paths = True
    # default the short_paths home to ~/.conan_short
    # this may be overridden by setting the environment variable
    # CONAN_USER_HOME_SHORT or setting user_home_short in ~/.conan/conan.conf
    if sys.platform.startswith('win32') and os.getenv("CONAN_USER_HOME_SHORT") is None:
        os.environ["CONAN_USER_HOME_SHORT"] = os.path.join(
            os.path.expanduser("~"), ".conan_short")

    def set_version(self):
        git = tools.Git(folder=self.recipe_folder)
        self.version = "%s_%s" % (git.get_branch(), git.get_revision()[:16])

    def configure(self):
        # Python-related options are forced to be the same for dependencies
        for dep in ['coda-oss', 'nitro']:
            self.options[dep].PYTHON_HOME = self.options.PYTHON_HOME
            self.options[dep].PYTHON_VERSION = self.options.PYTHON_VERSION
            self.options[dep].ENABLE_PYTHON = self.options.ENABLE_PYTHON

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_id(self):
        # Make any change in our dependencies' version require a new binary
        self.info.requires.full_version_mode()

    def package_info(self):
        self.cpp_info.builddirs = ["lib/cmake"]
