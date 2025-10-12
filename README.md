# jmaths

`jmaths` is a maths library for C++ that has support for arbitrary precision integers and fractions, and for other concepts in common fields in mathematics, like linear algebra.

There are 2 ways to install this program. You can either use the default settings, or you can choose these settings yourself by editing `src/headers/constants_and_types.hpp.in`.

You can add your own dependencies for the library as well, which might be necessary if you've entered your own custom types in the previous step. Do this by adding the relevant includes to the aforementioned file.

Regardless of whether you added your own dependencies, type the following to build and install the library:

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j8
sudo cmake --install .
```

Or use the simpler shortcuts for POSIX shells and Windows CMD respectively:

```
./build_install_posix.sh
```

```
./build_install_windows.cmd
```

To build in debug mode, type `-DCMAKE_BUILD_TYPE=Debug` instead of `=Release`. The library is located at `/usr/local/lib/libjmaths[d].a` (the `d` is appended in debug builds) and the headers at `/usr/local/include/jmaths/*.hpp`. An elaborate documentation, licensing information and sample executables are located in `/usr/local/lib/jmaths/`.

The default installation directory is `/usr/local/`. If you would like to change that, you can pass a parameter `-DCMAKE_INSTALL_PREFIX=/some/install/dir/`, like so:

```
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/some/preferred/installation/directory/
```

When linking with `libjmaths.a`, it is important to also link with any libraries that it depends on, for example when providing a custom allocator in `src/headers/constants_and_types.hpp`, and to do so _after_ linking with `libjmaths.a`. Or you could of course alter the `CMakeLists.txt` file a little so all that will be done automatically.


