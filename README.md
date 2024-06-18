# jmaths
`jmaths` is a maths library for C++ that has support for arbitrary precision integers and fractions, and for other concepts in common fields in mathematics, like linear algebra.

There are 2 ways to install this program. You can either use the default settings, by typing `make` and then `sudo make install`, which assumes your machine's architecture has 64-bit integer support, or you can choose these settings yourself by editing `config/jmaths_aliases.hpp`.

The following command shall be run to configure the library's dependencies, allowing you to enter your own dependencies as well, which might be necessary if you've entered your own custom types in the previous step:

```
./dependencies
```

Regardless of whether you added your own dependencies, type the following to build and install the library:

```
make build
sudo make install
```

The default installation directory is `/usr/local/jmaths/`. If you would like to change that, you can pass an `install_dir` parameter to `sudo make install`, like so:

```
sudo make install install_dir=/some/preferred/installation/directory/
```

Whilst doing this, you will be guided through the configuration process. To rebuild the library from scratch, type `make fresh`, or use `make clean` to restore the folder to its original state.

To uninstall the library, simply type `sudo make uninstall`, and any traces of the library will be removed from your system.

When linking with `libjmaths.a`, it is important to also link with any libraries that it depends on, for example when providing a custom allocator in `config/jmaths_aliases.hpp`, and to do so _after_ linking with `libjmaths.a`.
