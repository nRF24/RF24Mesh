#!/usr/bin/env python
import os
from setuptools import setup, Extension
from sys import version_info

BOOST_LIB = "boost_python" + (
    "" if version_info < (3,) else "%d%d" % (version_info.major, version_info.minor)
)

# NOTE can't access "../../library.properties" from working dir because
# it's relative. Brute force absolute path dynamically.
git_dir = os.path.split(os.path.abspath(os.getcwd()))[0]

# get LIB_VERSION from library.properties file for Arduino IDE
version = "1.0"
with open(os.path.join(git_dir, "library.properties"), "r") as f:
    for line in f.read().splitlines():
        if line.startswith("version"):
            version = line.split("=")[1]

setup(
    version=version,
    ext_modules=[
        Extension(
            "RF24Mesh",
            sources=["pyRF24Mesh.cpp"],
            libraries=["rf24mesh", "rf24network", "rf24", BOOST_LIB],
        )
    ],
)
