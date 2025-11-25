# Minimum CMake version required
cmake_minimum_required(VERSION 3.14)

# Project definition
project(bl LANGUAGES CXX)

# --------------------------------------------------------------
# Build the executable
# --------------------------------------------------------------
add_executable(bl src/bl.cpp)

# expose the install prefix (e.g. /usr/local) to the C++ code
target_compile_definitions(bl PRIVATE
    INSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}"
)

# Require C++17 (or newer) – adjust if you need a different standard
target_compile_features(bl PUBLIC cxx_std_17)

# --------------------------------------------------------------
# Installation rules
# --------------------------------------------------------------
# 1) Install the binary into <CMAKE_INSTALL_PREFIX>/bin
#    (default CMAKE_INSTALL_PREFIX is /usr/local, so this ends up in /usr/local/bin)
install(
    TARGETS bl
    RUNTIME DESTINATION bin
)


# Install the man page with proper section handling
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/man/bl.1
    DESTINATION share/man/man1
    COMPONENT Runtime
)

# --------------------------------------------------------------
# Optional: make the install prefix visible to the user (helpful for packaging)
# --------------------------------------------------------------
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Binary will be installed to: ${CMAKE_INSTALL_PREFIX}/bin")
message(STATUS "Man page will be installed to: ${CMAKE_INSTALL_PREFIX}/share/man/man1")
