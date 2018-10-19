import os
from setuptools import setup

from setuptools.extension import Extension
from Cython.Build import cythonize

setup(
    name="example_bridge",
    version="0.0.3",
    description=("example of using np-bidge"),
    license="prop.",
    ext_modules=cythonize(Extension(
        "example_bridge.ext", include_dirs = ["ext"],sources=[
            "ext/stuff.cxx","ext/ext.pyx"],language="c++",
        extra_compile_args=["-std=c++14","-g"], extra_link_args=["-std=c++14","-g"]
        ))
)
