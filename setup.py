from setuptools import setup, Extension, find_packages
import pybind11

ext_modules = [ 
    Extension(
        "IterativeJackKnife.main",
        ["IterativeJackKnife/main.cpp", "IterativeJackKnife/mainFunctions.c"],
        include_dirs=[pybind11.get_include()],
        libraries=['gsl', 'gslcblas'],
        language="c++"
    ),
]

setup(
    name="IterativeJackKnife",
    version="0.0.1",
    author="David Olejniczak",
    author_email="david.olejniczak@icloud.com",
    description="JackKnife Iterative technique used to calculate Z score.",
    long_description="long description", #open("README.md").read(),
    long_description_content_type="text/markdown",
    ext_modules=ext_modules,
    install_requires=["pybind11"],
    python_requires=">=3.6",
)
