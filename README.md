In order to build:
```
cmake -B . build
cmake --build build
```

To run:
```
./build/SLR
```

GRAMMAR: 

E'→ E 

E → E + T | E - T| T

T → T * F | T / F| F

F → (E) | id


> **NOTE:** E'→ E is used to make it easy for automat to recognise when to end