## nps::range - Simple and Flexible Range Iterator for C++
nps::range is a lightweight, header-only library that brings Python-style range iteration to C++. Designed with simplicity and flexibility in mind, it allows developers to iterate over numeric ranges with custom start, end, and step values. Ideal for concise and readable loops, nps::range integrates seamlessly with modern C++ code.

### Key Features
- Simple Iteration: Use for loops to easily iterate over a sequence.
- Flexible Parameters: Supports custom start, end, and step values.
- Supports Numeric Types: Works with integers and floating-point types.

### Example Usages
#### Example - 1
```cpp
#include <iostream>
#include "nps_range.h"

for (int i : nps::range(5)) {
    std::cout << i;  // Output: 0 1 2 3 4
}
```

#### Exampe - 2
```cpp
#include <iostream>
#include "nps_range.h"

int main() {
    // Iteration from 0 to 10 with increments of 2
    for (int i : nps::range(0, 10).step(2)) {
        std::cout << i << " ";  // Output: 0 2 4 6 8
    }
    std::cout << std::endl;
    return 0;
}

```

### Example - 3
```cpp
#include <iostream>
#include "nps_range.h"

int main() {
    // Inverse iteration from 10 to 0.
    for (int i : nps::range(0, 10).reverse()) {
        std::cout << i << " ";  // Output: 9 8 7 6 5 4 3 2 1 0
    }
    std::cout << std::endl;
    return 0;
}

```

### Example - 4
```cpp
#include <iostream>
#include "nps_range.h"

int main() {
    // Iteration from 10 to 0, decreasing by 3.
    for (int i : nps::range(10, 0).step(-3)) {
        std::cout << i << " ";  // Output: 10 7 4 1
    }
    std::cout << std::endl;
    return 0;
}

```

### Example - 5
```cpp
#include <iostream>
#include "nps_range.h"

int main() {
    nps::range<int> r(0, 20, 3);  // Range from 0 to 20 in increments of 3

    std::cout << "nth_step(1): " << r.nth_step(1) << std::endl;  // Output: 0
    std::cout << "nth_step(2): " << r.nth_step(2) << std::endl;  // Output: 3
    std::cout << "nth_step(3): " << r.nth_step(3) << std::endl;  // Output: 6
    std::cout << "nth_step(4): " << r.nth_step(4) << std::endl;  // Output: 9
    std::cout << "nth_step(5): " << r.nth_step(5) << std::endl;  // Output: 12
    std::cout << "nth_step(6): " << r.nth_step(6) << std::endl;  // Output: 15
    std::cout << "nth_step(7): " << r.nth_step(7) << std::endl;  // Output: 18

    return 0;
}

```

### Exampe - 6
```cpp
#include <iostream>
#include "nps_range.h"

int main() {
    nps::range<int> r1(0, 10);  //Range from 0 to 10
    nps::range<double> r2(0, 20, 6.3);  // Range from 0 to 20 in increments of 6.3

    std::cout << "r1 size: " << r1.size() << std::endl;  // Output: 10
    std::cout << "r2 size: " << r2.size() << std::endl;  // Output: 4

    return 0;
}

```

## Why nps::range?
Avoid cumbersome loop setup and make your code more readable with intuitive range expressions. Whether for basic loops or more complex numeric sequences, nps::range helps you iterate with ease.
  
### Assert Handling
You can define and use your own assert to handle conditions. This assertion checks the condition and provides a message if the condition is false.
```cpp
#if !defined(_NPS_ASSERT)
#if defined(_DEBUG) 
#define _NPS_ASSERT(cond, message) assert(cond && message) // You can define and use your own assert to handle conditions.
#else // _DEBUG
#define _NPS_ASSERT(cond, message)
#endif // _DEBUG
#endif //  !defined(_NPS_ASSERT)
```


**License**  
`nps::range` is licensed under the MIT License, which allows for broad usage in both personal and commercial projects. 

Developed by [**tynes0**](https://github.com/tynes0). 

For inquiries, contact: cihanbilgihan@gmail.com.
