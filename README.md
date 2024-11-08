# Hello Professor An, see the instructions below:

## Build Requirements
- CMake (version 3.0 or higher)
- C++ compiler
- Make utility
- Homebrew package manager (macOS)

## Setup Steps (in projects root path)

1. Install required packages from requirements.txt:
   ```bash
   xargs brew install < requirements.txt
   ```

2. Create and enter build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Generate build files with CMake:
   ```bash
   cmake ..
   ```

4. Compile the project:
   ```bash
   make
   ```

5. Execute the compiled binary:
   ```bash
   ./game
   ```

## Notes
- Requirements.txt contains necessary Homebrew packages
- The build directory should be at the same level as the CMakeLists.txt file
- CMake will generate the necessary Makefiles based on your system configuration
- Compiled binaries will be located in the build directory
