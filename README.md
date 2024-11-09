## nps::range - Simple and Flexible Range Iterator for C++
nps::range is a lightweight, header-only library that brings Python-style range iteration to C++. Designed with simplicity and flexibility in mind, it allows developers to iterate over numeric ranges with custom start, end, and step values, or allows it to progress in a pattern. Ideal for concise and readable loops, nps::range integrates seamlessly with modern C++ code.

### Key Features
- Simple Iteration: Use for loops to easily iterate over a sequence.
- Flexible Parameters: Supports custom start, end, and step values.
- Supports All Numeric Types: Works with integers and floating-point types.

## Table of Contents
1. [Installation](#installation)
2. [Usage](#usage)
3. [Classes](#classes)
    - [range class](#range-class)
    - [circular_range class](#circular_range-class)
    - [patterned_range class](#patterned_range-class)
4. [Assert Handling](#assert-handling)
5. [License](#license)

## Installation
To include this library in your project, add the nps_range.h file to your project and start by including it:
```cpp
#include "nps_range.h"
```

## Usage

#### Example - 1
```cpp
#include <iostream>
#include "nps_range.h"

for (int i : nps::range(5))
    std::cout << i;  // Output: 0 1 2 3 4
```

#### Exampe - 2
```cpp
int main() {
    // Iteration from 0 to 10 with increments of 2
    for (int i : nps::range(0, 10).step(2))
        std::cout << i << " ";  // Output: 0 2 4 6 8
}
```

### Example - 3
```cpp
int main() {
    // Inverse iteration from 10 to 0.
    for (int i : nps::range(0, 10).reverse())
        std::cout << i << " ";  // Output: 9 8 7 6 5 4 3 2 1 0
}

```

### Example - 4
```cpp
int main() {
    // Iteration from 10 to 0.3, decreasing by 2.5.
    for (float i : nps::range(10.0f, 0.3f).step(-2.5f)) 
        std::cout << i << " ";  // Output: 10 7.5 5 2.5
}
```

## Classes
### range class
The range class provides a basic range iterator that iterates through numbers between a given start and end with a specified step value.

#### Constructors
- **`range()`**  
Default constructor. Initializes a range with no start or end values.

- **`range(_Ty end)`**  
Creates a range starting from 0 to the specified end value with a step of 1.

- **range`(_Ty start, _Ty end, step_type step = 1)`**  
Creates a range starting from start to end with a specified step.

 #### Member Functions
- **`reset(_Ty start, _Ty end, step_type step = 1)`**  
Resets the range with a new start, end, and step value.

- **`step(step_type new_step) const noexcept`**  
Returns a new range with the specified step value.

- **`reverse() const noexcept`**  
Returns a new range in reverse order (from end to start).

- **`scale(_Ty factor) const noexcept`**  
Returns a new range scaled by the specified factor.

- **`intersection(const range& other) const noexcept`**  
Returns the intersection of two ranges.

- **`circular(long long count = 0) const noexcept`**  
Returns a circular range that loops over the values infinitely.

- **`patterned(_Ty(*pattern_fun)(_Ty)) const noexcept`**  
Returns a patterned range with a custom pattern function that modifies the values as they iterate.

- **`slice(size_type start_index, size_type end_index) const noexcept`**  
Returns a sliced version of the range between the specified indices.

- **`odd() const noexcept`**  
Returns a range containing only odd numbers.

- **`even() const noexcept`**  
Returns a range containing only even numbers.

- **`nth_step(size_type n) const noexcept`**  
Returns the nth step value of the range.

- **`size() const noexcept`**  
Returns the total number of elements in the range.

and more...

### circular_range class 
The circular_range class provides a range that wraps around, looping back to the start value after reaching the end value.

#### Constructors
- **`circular_range()`**  
Default constructor.

- **`circular_range(_Ty end, long long count = 0)`**  
Creates a circular range starting from 0 to the specified end value.

- **`circular_range(_Ty start, _Ty end, step_type step, long long count = 0)`**  
Creates a circular range starting from start to end with a specified step value.

#### Member Functions
- **`reset(_Ty start, _Ty end, step_type step = 1, long long count = 0) noexcept`**  
Resets the circular range with the specified start, end, step, and count values.

- **`begin() const noexcept`**  
Returns an iterator pointing to the start of the range.

- **`end() const noexcept`**  
Returns an iterator pointing to the end of the range.

### patterned_range class 
The patterned_range class allows iteration through a range where the values follow a custom pattern defined by a user-provided function.

#### Constructors
- **`patterned_range()`**  
Default constructor.

- **`patterned_range(_Ty end, _Ty(*pattern_fun)(_Ty) = [](_Ty v)->_Ty { return v++; })`**  
Creates a patterned range from 0 to end, with a default pattern that increments the value by 1.

- **`patterned_range(_Ty start, _Ty end, _Ty(*pattern_fun)(_Ty) = [](_Ty v)->_Ty { return v++; })`**  
Creates a patterned range from start to end, with a default pattern function that increments the value by 1.

#### Member Functions
- **`reset(_Ty start, _Ty end, _Ty(*pattern_fun)(_Ty) = [](_Ty v)->_Ty { return v++; }) noexcept`**  
Resets the patterned range with the specified start, end, and pattern function.

- **`begin() const noexcept`**  
Returns an iterator pointing to the start of the range.

- **`end() const noexcept`**  
Returns an iterator pointing to the end of the range.

## Assert Handling
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

### **License**  
`nps::range` is licensed under the MIT License, which allows for broad usage in both personal and commercial projects. 

Developed by [**tynes0**](https://github.com/tynes0). 

For inquiries, contact: cihanbilgihan@gmail.com.
