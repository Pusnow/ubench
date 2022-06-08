import sys

from pybind11 import get_cmake_dir

# Available at setup time due to pyproject.toml
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

__version__ = "0.0.1"


ext_modules = [
    Pybind11Extension(
        "ubench",
        ["lib.cpp"],
        define_macros=[("VERSION_INFO", __version__)],
    ),
]

setup(
    name="ubench",
    version=__version__,
    author="Wonsup Yoon",
    author_email="wonsup@pusnow.com",
    url="https://github.com/Pusnow/ubench",
    description="A header-only simple microbenchmark library for Linux user-space, Linux kernel module, macOS, and Windows.",
    long_description="",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.6",
)
