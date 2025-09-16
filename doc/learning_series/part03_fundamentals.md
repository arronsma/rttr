# Part 3: RTTR Fundamentals and Type System

> **Learning Objective**: Understand the core concepts of RTTR, the type system, and how reflection works at runtime.

## Overview

In this part, we'll dive deep into the fundamental concepts that make RTTR work. By the end, you'll understand:

- What reflection is and why it's useful in C++
- How RTTR's type system works
- The relationship between types, instances, and variants
- Core RTTR classes and their purposes
- The philosophy behind RTTR's design

## What is Reflection?

Reflection is the ability of a program to examine and modify its own structure and behavior at runtime. In languages like Java or C#, reflection is built-in. C++ doesn't have native reflection, but RTTR provides this capability.

### Why Do We Need Reflection?

Consider these scenarios:

```cpp
// Without reflection - you need to know types at compile time
void save_to_json(const Person& person) {
    json["name"] = person.get_name();
    json["age"] = person.get_age();
    // ... hardcoded for each type
}

// With reflection - works with any registered type
void save_to_json(const variant& obj) {
    type t = obj.get_type();
    for (auto& prop : t.get_properties()) {
        json[prop.get_name()] = prop.get_value(obj);
    }
}
```

**Common Use Cases:**
- **Serialization**: Save/load objects to/from JSON, XML, databases
- **Scripting**: Expose C++ objects to Lua, Python, JavaScript
- **Object Editors**: Build property grids for game editors
- **Plugin Systems**: Load code dynamically and discover its types
- **Network Protocols**: Handle unknown message types
- **Testing**: Create mock objects and verify behavior

## RTTR Type System

RTTR creates a parallel type system that operates at runtime alongside C++'s compile-time type system.

### The rttr::type Class

The `rttr::type` class is the cornerstone of RTTR. It represents a C++ type at runtime:

```cpp
#include <rttr/type>
#include <iostream>
#include <string>

int main() {
    using namespace rttr;
    
    // Get type information for built-in types
    type int_type = type::get<int>();
    type string_type = type::get<std::string>();
    
    std::cout << "int type name: " << int_type.get_name() << std::endl;
    std::cout << "string type name: " << string_type.get_name() << std::endl;
    
    // Type comparison
    if (int_type == type::get<int>()) {
        std::cout << "Types are equal!" << std::endl;
    }
    
    // Type queries
    std::cout << "int is arithmetic: " << int_type.is_arithmetic() << std::endl;
    std::cout << "string is class: " << string_type.is_class() << std::endl;
    
    return 0;
}
```

**Key Type Operations:**
- `type::get<T>()` - Get type info for T
- `type::get_by_name("name")` - Get type by string name
- `type::get(object)` - Get type from object instance
- `t.get_name()` - Get type name as string
- `t.is_class()`, `t.is_arithmetic()` - Type queries
- `t == other_type` - Type comparison

### Understanding Variants

The `rttr::variant` class is RTTR's type-erased value container. Think of it as a type-safe `void*` that remembers what type it contains:

```cpp
#include <rttr/type>
#include <iostream>

int main() {
    using namespace rttr;
    
    // Create variants with different types
    variant int_var = 42;
    variant string_var = std::string("Hello RTTR");
    variant double_var = 3.14159;
    
    // Variants remember their types
    std::cout << "int_var type: " << int_var.get_type().get_name() << std::endl;
    std::cout << "string_var type: " << string_var.get_type().get_name() << std::endl;
    
    // Extract values (with type checking)
    if (int_var.can_convert<int>()) {
        int value = int_var.get_value<int>();
        std::cout << "int value: " << value << std::endl;
    }
    
    // Convert between types when possible
    if (int_var.can_convert<double>()) {
        double converted = int_var.to_double();
        std::cout << "int as double: " << converted << std::endl;
    }
    
    // Check if variant is valid
    variant empty_var;
    std::cout << "empty variant valid: " << empty_var.is_valid() << std::endl;
    
    return 0;
}
```

**Variant Features:**
- **Type Safety**: Can't extract wrong type without explicit conversion
- **Automatic Conversions**: Built-in conversions between compatible types
- **Value Semantics**: Variants store copies (by default)
- **Null State**: Invalid variants represent "no value"

### Working with Objects

Let's create a simple class and see how RTTR handles it:

```cpp
// simple_class.h
#include <rttr/type>
#include <string>

class Person {
public:
    Person() = default;
    Person(const std::string& name, int age) : name_(name), age_(age) {}
    
    const std::string& get_name() const { return name_; }
    void set_name(const std::string& name) { name_ = name; }
    
    int get_age() const { return age_; }
    void set_age(int age) { age_ = age; }
    
private:
    std::string name_;
    int age_ = 0;
    
    RTTR_ENABLE() // Enable RTTR for this class
};
```

```cpp
// simple_class.cpp
#include "simple_class.h"
#include <rttr/registration>

RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<Person>("Person")
        .constructor<>()
        .constructor<const std::string&, int>()
        .property("name", &Person::get_name, &Person::set_name)
        .property("age", &Person::get_age, &Person::set_age)
        ;
}
```

```cpp
// main.cpp
#include <rttr/type>
#include <iostream>
#include "simple_class.h"

int main() {
    using namespace rttr;
    
    // Get type information
    type person_type = type::get<Person>();
    std::cout << "Type name: " << person_type.get_name() << std::endl;
    
    // Create instances using reflection
    variant person_var = person_type.create({"Alice", 30});
    std::cout << "Created: " << person_var.get_type().get_name() << std::endl;
    
    // Work with the instance
    Person& person = person_var.get_value<Person>();
    std::cout << "Name: " << person.get_name() << std::endl;
    std::cout << "Age: " << person.get_age() << std::endl;
    
    return 0;
}
```

## Core RTTR Classes

### 1. rttr::type
The primary interface for type information:

```cpp
type t = type::get<MyClass>();

// Basic information
std::string name = t.get_name();
bool is_class = t.is_class();
bool is_pointer = t.is_pointer();

// Reflection objects
auto props = t.get_properties();
auto methods = t.get_methods();
auto ctors = t.get_constructors();

// Object creation
variant obj = t.create({arg1, arg2});
```

### 2. rttr::variant
Type-erased value container:

```cpp
variant v = 42;
variant v2 = std::string("hello");

// Type queries
type t = v.get_type();
bool valid = v.is_valid();

// Value extraction
int value = v.get_value<int>();
double d = v.to_double();

// Conversion checks
bool can_convert = v.can_convert<std::string>();
```

### 3. rttr::property
Represents class properties (member variables or getter/setter pairs):

```cpp
type t = type::get<Person>();
property name_prop = t.get_property("name");

// Property information
std::string prop_name = name_prop.get_name();
type prop_type = name_prop.get_type();
bool readable = name_prop.is_readable();
bool writable = name_prop.is_writable();

// Use property
Person person;
name_prop.set_value(person, std::string("Bob"));
variant name = name_prop.get_value(person);
```

### 4. rttr::method
Represents class methods:

```cpp
type t = type::get<Person>();
method set_age = t.get_method("set_age");

// Method information
std::string method_name = set_age.get_name();
type return_type = set_age.get_return_type();
auto param_types = set_age.get_parameter_types();

// Invoke method
Person person;
variant result = set_age.invoke(person, 25);
```

### 5. rttr::constructor
Represents class constructors:

```cpp
type t = type::get<Person>();
constructor ctor = t.get_constructor({type::get<std::string>(), type::get<int>()});

// Constructor information
auto param_types = ctor.get_parameter_types();
bool is_valid = ctor.is_valid();

// Create instance
variant person = ctor.invoke("Charlie", 35);
```

## RTTR Design Philosophy

### 1. No Header Pollution
Registration happens in `.cpp` files, not headers:

```cpp
// Good - in .cpp file
RTTR_REGISTRATION {
    registration::class_<MyClass>("MyClass")
        .property("data", &MyClass::data);
}

// Bad - don't put registration in headers
```

### 2. Minimal Macros
Only two macros needed in most cases:
- `RTTR_ENABLE()` - For classes (especially with inheritance)
- `RTTR_REGISTRATION_FRIEND` - For private member access

### 3. No RTTI Required
RTTR provides its own type identification:

```cpp
// Works even with -fno-rtti
type t = type::get<MyClass>();
```

### 4. Exception-Free
All operations return error states instead of throwing:

```cpp
variant result = method.invoke(obj, args);
if (!result.is_valid()) {
    // Handle error - no exceptions thrown
}
```

### 5. Performance Focused
- Fast type lookups using hash tables
- Minimal runtime overhead
- Compile-time template optimization

## Common Patterns and Idioms

### Type-Safe Generic Programming

```cpp
template<typename T>
void print_object_info(const T& obj) {
    type t = type::get<T>();
    std::cout << "Type: " << t.get_name() << std::endl;
    
    for (auto& prop : t.get_properties()) {
        variant value = prop.get_value(obj);
        std::cout << "  " << prop.get_name() << ": " 
                  << value.to_string() << std::endl;
    }
}
```

### Runtime Type Discovery

```cpp
void process_unknown_object(const variant& obj) {
    type t = obj.get_type();
    
    if (t.is_derived_from<BaseClass>()) {
        // Handle base class interface
    } else if (t == type::get<SpecificClass>()) {
        // Handle specific type
    }
}
```

### Safe Downcasting

```cpp
variant base_ptr = get_some_base_pointer();
type derived_type = type::get_by_name("DerivedClass");

if (base_ptr.get_type().is_derived_from(derived_type)) {
    variant derived = base_ptr.convert(derived_type);
    // Safe to use as derived type
}
```

## Exercises

### Exercise 1: Basic Type Exploration
Create a program that:
1. Gets type information for `int`, `double`, `std::string`
2. Prints their names and properties
3. Creates variants and converts between them

### Exercise 2: Simple Class Registration
1. Create a `Rectangle` class with width/height properties
2. Register it with RTTR
3. Create instances using reflection
4. Set properties using reflection

### Exercise 3: Type Query System
Write a function that takes a `variant` and prints:
- Type name
- Whether it's a class, arithmetic type, or pointer
- If it's a class, list all properties

## What's Next?

In [Part 4: Basic Registration Patterns](part04_basic_registration.md), we'll explore:
- Different registration techniques
- Handling various member types
- Registration best practices
- Common registration patterns

## Key Takeaways

1. **RTTR provides runtime type information** for C++ without built-in reflection
2. **`rttr::type` is the gateway** to all reflection operations
3. **`rttr::variant` enables type-safe generic programming** at runtime
4. **Registration is separate** from class definition (no header pollution)
5. **Performance and safety** are core design principles

---

**Previous**: [Part 2: CMake Debug Setup](part02_cmake_debug.md) | **Next**: [Part 4: Basic Registration Patterns](part04_basic_registration.md)

**Back to**: [Learning Series Index](README.md)