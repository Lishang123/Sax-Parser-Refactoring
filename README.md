## Build Instructions

### Build

```bash
cmake -S . -B build -G Ninja
cmake --build build --target unit_tests --parallel
```

### Run Tests
```bash
cd build 
ctest -R ^SaxParser\\.
```

