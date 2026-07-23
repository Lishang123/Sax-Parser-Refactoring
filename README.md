## Build Instructions

### Build

```bash
cmake -S . -B build
cmake --build build --target unit_tests --parallel
```

### Run Tests
```bash
ctest -R "^SaxParser\." --test-dir build
```
