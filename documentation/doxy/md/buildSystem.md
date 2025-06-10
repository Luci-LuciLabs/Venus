
# Venus Build-System

  Venus uses CMake as its build-system. Everything in the "CMakePresets.json" file is configured to use the Ninja generator.

  - CMake version: 3.28 ***Currently the only version tested and used in the project.***
  - Ninja version: 1.11.1 ***Currently the only version tested and used in the project.***
  - Make version 4.3 ***Not preferred due to be slower and requiring a bit more setup than Ninja***
  - Any C++23 compiler: ***Clang is preferred, gcc/g++ has moderate support, msvc is untested, no others have been considered.***

  All alternative generators such as visual-studio-solutions are entirely untested at this time. 

## A walk through the Venus build-system

- **Section 1: Project Root Directory**

  The project root CMakeLists.txt controls the configuration of project versions, files, options, and subdirectories.
  
  - **CMakeLists.txt**

    - **Version**

      The project version is only to be updated when releasing a new update. 
      
      The most common and least-restrictive version variable would be "PROJECT_VERSION_PATCH", you may bump this up by '1' when releasing fixes, and minour updates, with no
      possible conflictions with current versions.

      The second most common version variable is "PROJECT_VERSION_MINOR", you may only bump this up by '1' during a full update release. 
      If this version is updated then the patch version must be reset to '0', all future patches on this version will start at '1' and tick up as normal. 
      This version is permitted to have minour-moderate conflictions with previous versions, however it should always be kept to a minimum where possible.

      The final and least common version variable is "PROJECT_VERSION_MAJOR", you may only bump this by '1' during a full update release that breaks large amounts of previous compatibility to older versions. 
      Whereas a minour version would denote added features or re-working current features, a majour version would denote large swathes of changes to the core components of Venus. 
      This will likely contain new features, bug-fixes, and more, but the key difference between majour and minour version is specifically compatibility to versions in-use today.
      If you update this version variable you should reset both the minour and patch variables to '0', continue by following the established rules for updating versions.

      Each version variable is required as they are used within "version.hpp" to supply the current version to the codebase upon build configuration and compilation.

    - **Options**

      1. SANITIZE:

          When this option is enabled cmake will include the "sanitize.cmake.in" helper module to configure
          project-global sanitization type and the necessary flags for each. There are currently three sanitization types to choose from:
            
          - Thread Safety
          - Memory Safety
          - Security
          
          It should be noted that these sanitization builds are merely supplemental to development and are not entirely accurate.
          For example: Address-Sanitization, *part of the memory sanitization type*, will consistently report memory leaks with
          libdbus.so because it cannot fully track it. It's quite likely that there is in fact no leak in cases such as this, with that said it
          would still behoove you to run more in-depth diagnostics tests.

          Valgrind falls victim to similar issues as ASAN, however it seems to also have issues with vulkan in general. While still useful valgrind is
          not very trust-worthy in graphics-programming. Again its better to find a more suitable tool to run for in-depth diagnostics.

  - **CMake directory**

    Found in the project root; the cmake directory contains our necessary cmake modules. These modules may set global configuration, or supply logic and utility throughout the build-system.

    - ***x86_64clang.cmake***:

      This toolchain file is exactly what it sounds like, it sets your compiler to clang and targets the x86_64 architecture. This also involves setting required flags for clang.
      All clang options and configuration should be edited here, if you want to add another target architecture please create a new toolchain file and new cmake-preset to make use of it.

    - ***x86_64gcc-g++.cmake***:

      This toolchain file is exactly what it sounds like, it sets your compiler to gcc/g++ and targets the x86_64 architecture. This also invloves setting required flags for gcc/g++.
      All gcc/g++ options and configuration should be edited here, if you want to add another target architecture please create a new toolchain file and new cmake-preset to make use of it.

    - ***x86_64msvc.cmake***:

      This toolchain file is exactly what it sounds like, it sets your compiler to MSVC and targets the x86_64 architecture. This also involves setting required flags for MSVC.
      All msvc options and configuration should be edited here, if you want to add another target architecture please create a new toolchain file and new cmake-preset to make use of it.

    - ***sanitize.cmake.in***:

      This module contains the per-compiler and per-sanitization-type configuration of sanitization-flags used across the build-system during project sanitization.
      If you want to adjust sanitization in any manner you should adjust it in this file, do not rename the variable "SANITIZE_FLAGS" as its used throughout the build-system.

    - ***version.hpp.in***:

      This file should never be changed, cmake uses it to generate "version.hpp" and place it without our project sources.

  - **CMakePresets.json**

    The provided presets within "CMakePresets.json" are the encouraged way to handle cmake configuration for builds.
    It's highly encouraged that you browse the provided presets and use what is most applicable for your needs.
    If you don't see anything that suits your needs then, so-long as it does not break anything currently provided, you should feel free to add the necessary preset and toolchain file.

    Example usage:

    ```Bash
      rm -rf build; mkdir build; cmake --preset=clangDebug -S . -B build
    ```
    There are many presets to allow for easy selection of supported configurations.

    - clang debug and release.
    - clang sanitization types, ***a subset of debug***.

    - gcc/g++ debug and release.
    - gcc/g++ sanitization types, ***a subset of debug***.

    - msvc debug and release.
    - ***sanitization is currently not configured for msvc***


- **Section 2: Project Source Directory**:


