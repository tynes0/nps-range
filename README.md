## nps::range - Simple and Flexible Range Iterator for C++
nps::range is a lightweight, header-only library that brings Python-style range iteration to C++. Designed with simplicity and flexibility in mind, it allows developers to iterate over numeric ranges with custom start, end, and step values. Ideal for concise and readable loops, nps::range integrates seamlessly with modern C++ code.

### Key Features
- Simple Iteration: Use for loops to easily iterate over a sequence.
- Flexible Parameters: Supports custom start, end, and step values.
- Supports Numeric Types: Works with integers and floating-point types.
```cpp

#include "nps_range.h"

for (int i : nps::range(5)) {
    std::cout << i;  // Output: 0 1 2 3 4
}
```
## Why nps::range?
Avoid cumbersome loop setup and make your code more readable with intuitive range expressions. Whether for basic loops or more complex numeric sequences, nps::range helps you iterate with ease.

**License**  
`nps::range` is licensed under the MIT License, which allows for broad usage in both personal and commercial projects. 

Developed by [**tynes0**](https://github.com/tynes0). 

For inquiries, contact: cihanbilgihan@gmail.com.
