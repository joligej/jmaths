# jmaths
`jmaths` is a maths library for C++ that has support for arbitrary precision integers and fractions, and for other concepts in common fields in mathematics, like linear algebra.

There are 2 ways to install this program. You can either use the default settings, by typing `make` and then `sudo make install`, which deduces your machine's architecture (i.e. does it have 64-bit integer support?), or you can choose all the settings yourself:

```
./preconfigure
make configure
./configure
```

The following command allows you to enter your own dependencies, which might be necessary if you've entered your own custom types in the previous step:

```
./custom_dependencies
```

Regardless of whether you added your own dependencies, type the following to build and install the library:

```
make build
sudo make install
```

Whilst doing this, you will be guided through the configuration process. To rebuild the library from scratch, type `make fresh`, or use `make clean` to restore the folder to its original state.

To uninstall the library, simply type `sudo make uninstall`, and any traces of the library will be removed from your system.
