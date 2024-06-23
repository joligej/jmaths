# jmaths

`jmaths` is a maths library for C++ that has support for arbitrary precision integers and fractions, and for other concepts in common fields in mathematics, like linear algebra.

There are 2 ways to install this program. You can either use the default settings, by typing `make` and then `sudo make install`, which assumes your machine's architecture has 64-bit integer support, or you can choose these settings yourself by editing `config/jmaths_aliases.hpp`.

You can enter your own dependencies for the library as well, which might be necessary if you've entered your own custom types in the previous step. Do this by adding the relevant paths to the `config/dependencies.hpp` file.

Regardless of whether you added your own dependencies, type the following to build and install the library:

```
make
sudo make install
```

To build in debug mode type `make debug` instead of `make`. The library is located at `build/libdbgjmaths.a` and the header at `build/dbgjmaths.hpp`.

The default installation directory is `/usr/local/jmaths/`. If you would like to change that, you can pass an `install_dir` parameter to `sudo make install`, like so:

```
sudo make install install_dir="/some/preferred/installation/directory/"
```

Whilst doing this, you will be guided through the configuration process. To rebuild the library from scratch, type `make fresh`, or use `make clean` to restore the folder to its original state.

To uninstall the library, simply type `sudo make uninstall`, and any traces of the library will be removed from your system. If you provided a custom installation directory, please pass the directory as an `install_dir` parameter to the uninstall command as well.

The default compiler is `clang++`. If you wish to specify another compiler, such as `g++`, pass the path to the compiler command as a parameter to the makefile, by specifying, for example, `cc=g++14`, just like with the installation directory. The same goes for changing the extension of pretokenised headers, which by default is `.gch`. Change this by passing `pth_extension=.something` as a parameter. Also, your system's bit architecture can be set via the parameter `bit_architecture=some_value`, like 32 or 64. The default is 64. Be advised that there is no support for bit architectures below 16.

When linking with `libjmaths.a`, it is important to also link with any libraries that it depends on, for example when providing a custom allocator in `config/jmaths_aliases.hpp`, and to do so _after_ linking with `libjmaths.a`.

To do a unit test, run `make test` and then `test/run`. The file `test/logdata.csv` will contain the function profiling data.
