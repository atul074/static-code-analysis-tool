1. cd jargon-master
2. touch CMakeLists.txt
## Create a CMakeLists.txt 

```
cmake_minimum_required(VERSION 3.13)
project(JargonMaster)

set(CMAKE_CXX_STANDARD 17)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CLANG_INCLUDE_DIRS})

add_definitions(${LLVM_DEFINITIONS})

file(GLOB_RECURSE SOURCES
    core/*.cpp
    rules/*.cpp
    main.cpp
)

add_executable(jargon-master ${SOURCES})

target_link_libraries(jargon-master
    clangTooling
    clangBasic
    clangASTMatchers
)
```

3. mkdir build
4. cd build

## Build

1. cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
2. ninja

## Test

./jargon-master ../tests/testFile.cpp -- -std=c++17

## Test Folder

1. Add tests for each rule in the test folder

## Versions

```
clang --version
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

```
llvm-config --version
18.1.3
```

## Packages and Libraries used

```
# Core build tools
sudo apt install -y build-essential cmake ninja-build git

# Clang + LLVM development libraries (IMPORTANT)
sudo apt install -y llvm-dev clang libclang-dev clang-tools

# Faster build
sudo apt install ccache
```

### RULES AND PROGRESS - GOOGLE DOCS
```
https://docs.google.com/document/d/1G1QU9SAc30Ln2xjnRvo3pRjKRV3J0R3-S8BAYjIJUHI/edit?tab=t.fwe032of2ziy
```

## System Issue

1. If build is slow, use ninja -j4 or ninja -j2 or ninja -j8 as per the number of cores your cpu has
2. Build using ccache

## Ccache commands

```
cmake -G Ninja \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  ..
```

## Verify ccache
```
ccache -s
```
# through checks
# build
mkdir build && cd build
cmake -G Ninja ..
ninja

# run 
./jargon-master ../tests/VirtualSpecifierCheckTest.cpp -- -std=c++17 