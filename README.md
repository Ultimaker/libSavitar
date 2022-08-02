# Savitar

<p align="center">
    <a href="https://github.com/Ultimaker/libSavitar/actions/workflows/conan-package.yml" alt="Conan Package">
        <img src="https://github.com/Ultimaker/libsavitar/actions/workflows/conan-package.yml/badge.svg" /></a>
    <a href="https://github.com/Ultimaker/libSavitar/actions/workflows/unit-test.yml" alt="Unit test">
        <img src="https://github.com/Ultimaker/libsavitar/actions/workflows/unit-test.yml/badge.svg" /></a>
    <a href="https://github.com/Ultimaker/libSavitar/issues" alt="Open Issues">
        <img src="https://img.shields.io/github/issues/ultimaker/libsavitar" /></a>
    <a href="https://github.com/Ultimaker/libSavitar/issues?q=is%3Aissue+is%3Aclosed" alt="Closed Issues">
        <img src="https://img.shields.io/github/issues-closed/ultimaker/libsavitar?color=g" /></a>
    <a href="https://github.com/Ultimaker/libSavitar/pulls" alt="Pull Requests">
        <img src="https://img.shields.io/github/issues-pr/ultimaker/libsavitar" /></a>
    <a href="https://github.com/Ultimaker/libSavitar/graphs/contributors" alt="Contributors">
        <img src="https://img.shields.io/github/contributors/ultimaker/libsavitar" /></a>
    <a href="https://github.com/Ultimaker/libSavitar" alt="Repo Size">
        <img src="https://img.shields.io/github/repo-size/ultimaker/libsavitar?style=flat" /></a>
    <a href="https://github.com/Ultimaker/libSavitar/blob/master/LICENSE" alt="License">
        <img src="https://img.shields.io/github/license/ultimaker/libsavitar?style=flat" /></a>
</p>

This library contains C++ code and Python bindings for loading 3mf files.

## License

![License](https://img.shields.io/github/license/ultimaker/libsavitar?style=flat)  
Savitar is released under terms of the LGPLv3 License. Terms of the license can be found in the LICENSE file. Or at
http://www.gnu.org/licenses/lgpl.html

> But in general it boils down to:  
> **You need to share the source of any Savitar modifications if you make an application with Savitar.**

## How to build

> **Note:**  
> We are currently in the process of switch our builds and pipelines to an approach which uses [Conan](https://conan.io/)
> and pip to manage our dependencies, which are stored on our JFrog Artifactory server and in the pypi.org.
> At the moment not everything is fully ported yet, so bare with us.

If you want to develop Cura with Savitar see the Cura Wiki: [Running Cura from source](https://github.com/Ultimaker/Cura/wiki/Running-Cura-from-Source)

If you have never used [Conan](https://conan.io/) read their [documentation](https://docs.conan.io/en/latest/index.html)
which is quite extensive and well maintained. Conan is a Python program and can be installed using pip

```bash
pip install conan --upgrade
conan config install https://github.com/ultimaker/conan-config.git
conan profile new default --detect
```

**Community developers would have to remove the Conan `cura` repository because that one requires credentials.**
```bash
conan remote remove cura
```

### Building Savitar

The steps above should be enough to get your system in such a state you can start development on Savitar. Executed in the root directory of
Savitar.

#### Release build type

```shell
conan install . --build=missing --update
cd cmake-build-release
cmake --toolchain=conan/conan_toolchain.cmake ..
cmake --build .
```

#### Debug build type

Use the same instructions as above, but pass the `-s build_type=Debug` flag to the `conan install` command.

```shell
conan install . --build=missing --update -s build_type=Debug
cd cmake-build-debug
cmake --toolchain=conan/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## Creating a new Savitar Conan package

To create a new Savitar Conan package such that it can be used in Cura and Uranium, run the following command:

```shell
conan create . savitar/<version>@<username>/<channel> --build=missing --update
```

This package will be stored in the local Conan cache (`~/.conan/data` or `C:\Users\username\.conan\data` ) and can be used in downstream
projects, such as Cura and Uranium by adding it as a requirement in the `conanfile.py` or in `conandata.yml` if that project is set up
in such a way. You can also specify the override at the commandline, to use the newly created package, when you execute the `conan install`
command in the root of the consuming project, with:


```shell
conan install . -build=missing --update --require-override=savitar/<version>@<username>/<channel>
```
