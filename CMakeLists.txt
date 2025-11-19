# --------------------------------------------------------------
# CMakeLists.txt – build and install the “bl” utility
# --------------------------------------------------------------

cmake_minimum_required(VERSION 3.12)   # 3.12 gives good std::filesystem support
project(bl LANGUAGES CXX)

# -----------------------------------------------
# 0) Set where the built executable will be written
# -----------------------------------------------
# By default CMAKE_INSTALL_PREFIX is /usr/local, so this puts the
# binary straight into /usr/local/bin during the normal build step.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/bin")

# --------------------------------------------------------------
# 1) Choose the C++ standard
# --------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)            # or 20 if you prefer
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------------------
# 2) Add the executable
# --------------------------------------------------------------
add_executable(bl bl.cpp)

# --------------------------------------------------------------
# 3) Link the filesystem library (needed on older GCC/Clang)
# --------------------------------------------------------------
# Modern compilers (gcc ≥ 9, clang ≥ 9, MSVC ≥ 2017) link std::filesystem
# automatically. The following lines are safe on all supported versions:
find_package(Threads REQUIRED)        # required by std::filesystem on some platforms
target_link_libraries(bl PRIVATE stdc++fs Threads::Threads)

# --------------------------------------------------------------
# 4) Install rule
# --------------------------------------------------------------
# By default CMAKE_INSTALL_PREFIX is /usr/local, so the binary will go to
# /usr/local/bin. Users can override the prefix with -DCMAKE_INSTALL_PREFIX=…
install(TARGETS bl
        DESTINATION bin)               # installs to ${CMAKE_INSTALL_PREFIX}/bin
