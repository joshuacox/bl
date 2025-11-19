# bl

`bl` is a simple backlight utility for Linux systems that use the sysfs backlight interface.

## Overview

The program can set the screen brightness to an absolute percentage or adjust it relative to the current level.

## Build

The project uses CMake. From the repository root:

```sh
# Create a build directory (out‑of‑source build)
mkdir -p build && cd build

# Configure the project (the install prefix defaults to /usr/local)
cmake ..   # you can override with -DCMAKE_INSTALL_PREFIX=/some/other/path

# Build the executable – it will be written directly to /usr/local/bin
make        # or: cmake --build .

# (Optional) Install system‑wide
# sudo make install   # installs to /usr/local/bin (same place as the build output)
```

The resulting binary `bl` will appear in **/usr/local/bin** after the `make` step, so you can run it immediately without a separate install step.

