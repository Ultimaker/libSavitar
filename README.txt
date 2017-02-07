Introduction

libSavitar is a c++ implementation of 3mf loading with SIP python bindings.

Build instructions Windows with MSVC.

Make sure you have run something like:

“c:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64

mkdir build
mkdir install_dir
cd build
cmake -DCMAKE_INSTALL_PREFIX=../install_dir -DBUILD_STATIC=ON -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles" ..
nmake
nmake install

Now copy the site-packages contents from the install_dir into your site-packages dir.