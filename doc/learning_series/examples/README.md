# Learning Series Examples

This directory contains complete, runnable examples for each part of the RTTR learning series.

## Structure

```
examples/
├── part03_fundamentals/
│   ├── CMakeLists.txt
│   ├── basic_types.cpp
│   ├── variant_demo.cpp
│   └── simple_class/
├── part04_registration/
│   ├── CMakeLists.txt
│   ├── player_example/
│   ├── shapes_example/
│   └── factory_example/
└── shared/
    ├── CMakeLists.txt
    └── utils.h
```

## Building Examples

Each example directory has its own CMakeLists.txt file. To build:

```bash
cd examples/part03_fundamentals
mkdir build && cd build
cmake ..
make
```

## Running Examples

Each example produces an executable that demonstrates the concepts:

```bash
# Run basic types demo
./basic_types_demo

# Run variant demo  
./variant_demo

# Run simple class demo
./simple_class_demo
```

## Example Descriptions

### Part 3 Examples
- **basic_types.cpp**: Demonstrates type queries and basic RTTR operations
- **variant_demo.cpp**: Shows variant creation, conversion, and type safety
- **simple_class/**: Complete class registration and usage example

### Part 4 Examples  
- **player_example/**: Game character with properties and methods
- **shapes_example/**: Inheritance hierarchy with virtual methods
- **factory_example/**: Factory pattern with custom constructors

## Next Steps

After running these examples, continue with the next part of the learning series. Each example builds on previous concepts while introducing new ones.