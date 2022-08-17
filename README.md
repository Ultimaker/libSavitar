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

## System Requirements

### Windows
- Python 3.6 or higher
- Ninja 1.10 or higher
- VS2022 or higher
- CMake 3.23 or higher
- nmake

### MacOs
- Python 3.6 or higher
- Ninja 1.10 or higher
- apply clang 11 or higher
- CMake 3.23 or higher
- make

### Linux
- Python 3.6 or higher
- Ninja 1.10 or higher
- gcc 12 or higher
- CMake 3.23 or higher
- make


## How To Build

> **Note:**  
> We are currently in the process of switch our builds and pipelines to an approach which uses [Conan](https://conan.io/)
> and pip to manage our dependencies, which are stored on our JFrog Artifactory server and in the pypi.org.
> At the moment not everything is fully ported yet, so bare with us.

If you want to develop Cura with Savitar see the Cura Wiki: [Running Cura from source](https://github.com/Ultimaker/Cura/wiki/Running-Cura-from-Source)

If you have never used [Conan](https://conan.io/) read their [documentation](https://docs.conan.io/en/latest/index.html)
which is quite extensive and well maintained. Conan is a Python program and can be installed using pip

### 1. Configure Conan

```bash
pip install conan --upgrade
conan config install https://github.com/ultimaker/conan-config.git
conan profile new default --detect --force
```

Community developers would have to remove the Conan cura repository because it requires credentials. 

Ultimaker developers need to request an account for our JFrog Artifactory server at IT
```bash
conan remote remove cura
```

### 2. Clone libSavitar
```bash
git clone https://github.com/Ultimaker/libSavitar.git
cd libSavitar
```

### 3. Install & Build libSavitar (Release OR Debug)

#### Release
```bash
conan install . --build=missing --update
# optional for a specific version: conan install . pysavitar/<version>@<user>/<channel> --build=missing --update
cmake --preset release
cmake --build --preset release
```

#### Debug

```bash
conan install . --build=missing --update build_type=Debug
cmake --preset debug
cmake --build --preset debug
```

## Creating a new Savitar Conan package

To create a new Savitar Conan package such that it can be used in Cura and Uranium, run the following command:

```shell
conan create . savitar/<version>@<username>/<channel> --build=missing --update
```

This package will be stored in the local Conan cache (`~/.conan/data` or `C:\Users\username\.conan\data` ) and can be used in downstream
projects, such as Cura and Uranium by adding it as a requirement in the `conanfile.py` or in `conandata.yml`.

Note: Make sure that the used `<version>` is present in the conandata.yml in the pySavitar root

You can also specify the override at the commandline, to use the newly created package, when you execute the `conan install`
command in the root of the consuming project, with:


```shell
conan install . -build=missing --update --require-override=savitar/<version>@<username>/<channel>
```

## Developing libSavitar In Editable Mode

You can use your local development repository downsteam by adding it as an editable mode package.
This means you can test this in a consuming project without creating a new package for this project every time.

```bash
    conan editable add . savitar/<version>@<username>/<channel>
```

Then in your downsteam projects (Cura) root directory override the package with your editable mode package.  

```shell
conan install . -build=missing --update --require-override=savitar/<version>@<username>/<channel>
```
