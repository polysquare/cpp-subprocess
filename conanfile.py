from contextlib import contextmanager
from conans import ConanFile, CMake
from conans.tools import download, unzip
import os

VERSION = "0.0.1"


@contextmanager
def in_dir(directory):
    last_dir = os.getcwd()
    try:
        os.makedirs(directory)
    except OSError:
        pass

    try:
        os.chdir(directory)
        yield directory
    finally:
        os.chdir(last_dir)


class CPPSubprocessCMakeConan(ConanFile):
    name = "cpp-subprocess"
    version = os.environ.get("CONAN_VERSION_OVERRIDE", VERSION)
    generators = "cmake"
    requires = (
        "common-universal-cmake/master@smspillaz/common-universal-cmake",
        "Boost/1.60.0@lasote/stable"
    )
    url = "http://github.com/polysquare/cpp-subprocess"
    license = "MIT"
    settings = "os", "compiler", "build_type", "arch"

    def source(self):
        zip_name = "cpp-subprocess.zip"
        download("https://github.com/polysquare/"
                 "cpp-subprocess/archive/{version}.zip"
                 "".format(version="v" + VERSION),
                 zip_name)
        unzip(zip_name)
        os.unlink(zip_name)

    def build(self):
        cmake = CMake(self.settings)
        with in_dir("build"):
            self.run("cmake '{src}' {cmd}".format(src=self.conanfile_directory,
                                                  cmd=cmake.command_line))
            self.run("cmake --build . {cfg}".format(cfg=cmake.build_config))

    def package(self):
        self.copy(pattern="*.cmake",
                  dst="cmake/cpp-subprocess",
                  src="cpp-subprocess-" + VERSION,
                  keep_path=True)
