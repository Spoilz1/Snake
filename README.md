# Installation Instructions

## Build Requirements
- CMake (version 3.0 or higher)
- C++ compiler
- Make utility

## Build Steps

1. Create and enter build directory:
   ```bash
   cd build
   ```

2. Generate build files with CMake:
   ```bash
   cmake ..
   ```

3. Compile the project:
   ```bash
   make
   ```

4. Execute the compiled binary:
   ```bash
   ./game
   ```

## Notes
- The build directory should be at the same level as the CMakeLists.txt file
- CMake will generate the necessary Makefiles based on your system configuration
- Compiled binaries will be located in the build directory
