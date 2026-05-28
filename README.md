## Build Instructions

### Build

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --target unit_tests --parallel
```

### Run Tests
```bash
./build/tests/unit_tests
```

