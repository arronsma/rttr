# RTTR Quick Reference Guide

A concise reference for the RTTR (Run Time Type Reflection) library. This guide covers the most commonly used APIs and patterns.

## Core Headers

```cpp
#include <rttr/type>           // For type queries and basic reflection
#include <rttr/registration>   // For registering types (registration blocks only)
```

## Basic Type Operations

### Getting Type Information
```cpp
// From template parameter
type t = type::get<MyClass>();

// From object instance  
MyClass obj;
type t = type::get(obj);

// From type name
type t = type::get_by_name("MyClass");

// Check if type exists
if (t.is_valid()) { /* type found */ }
```

### Type Queries
```cpp
type t = type::get<int>();

std::string name = t.get_name();          // "int"
bool is_class = t.is_class();             // false
bool is_arithmetic = t.is_arithmetic();   // true
bool is_pointer = t.is_pointer();         // false
bool is_array = t.is_array();             // false
```

### Type Comparison
```cpp
type t1 = type::get<int>();
type t2 = type::get<int>();
type t3 = type::get<double>();

bool same = (t1 == t2);    // true
bool diff = (t1 != t3);    // true
```

## Variant Operations

### Creating Variants
```cpp
variant v1 = 42;                    // int
variant v2 = 3.14;                  // double  
variant v3 = std::string("hello");  // string
variant v4;                         // invalid/empty
```

### Variant Queries
```cpp
variant v = 42;

type t = v.get_type();              // get type info
bool valid = v.is_valid();          // true if contains value
bool null = v.is_null();            // true if null pointer
```

### Value Extraction
```cpp
variant v = 42;

// Safe extraction with checking
if (v.can_convert<int>()) {
    int value = v.get_value<int>();
}

// Direct conversion (may fail)
int value = v.to_int();
double value = v.to_double();
std::string value = v.to_string();
```

## Registration Patterns

### Basic Class Registration
```cpp
RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<MyClass>("MyClass")
        .constructor<>()
        .property("name", &MyClass::get_name, &MyClass::set_name)
        .method("do_something", &MyClass::do_something)
        ;
}
```

### Property Registration
```cpp
// Getter/setter pair (recommended)
.property("name", &MyClass::get_name, &MyClass::set_name)

// Read-only property
.property_readonly("id", &MyClass::get_id)

// Direct member access
.property("x", &MyClass::x)

// Private member (requires RTTR_REGISTRATION_FRIEND)
.property("data", &MyClass::data_, registration::private_access)
```

### Method Registration
```cpp
// Basic method
.method("calculate", &MyClass::calculate)

// Overloaded method (need to specify signature)
.method("add", select_overload<int(int, int)>(&MyClass::add))

// Static method
.method("create", &MyClass::create)

// Const method (handled automatically)
.method("get_info", &MyClass::get_info)
```

### Constructor Registration
```cpp
// Default constructor
.constructor<>()

// Parameterized constructors
.constructor<int>()
.constructor<const std::string&, int>()

// Factory method as constructor
.constructor(&MyClass::create_instance)
```

## Reflection Queries

### Getting Properties
```cpp
type t = type::get<MyClass>();

// Get single property
property prop = t.get_property("name");

// Get all properties
auto props = t.get_properties();
for (auto& prop : props) {
    std::cout << prop.get_name() << std::endl;
}
```

### Using Properties
```cpp
MyClass obj;
property prop = type::get<MyClass>().get_property("name");

// Set property value
bool success = prop.set_value(obj, std::string("New Name"));

// Get property value
variant value = prop.get_value(obj);
std::string name = value.to_string();

// Property info
type prop_type = prop.get_type();
bool readable = prop.is_readable();
bool writable = prop.is_writable();
```

### Getting Methods
```cpp
type t = type::get<MyClass>();

// Get single method
method meth = t.get_method("calculate");

// Get methods by name (handles overloads)
auto methods = t.get_methods("calculate");

// Get all methods
auto all_methods = t.get_methods();
```

### Using Methods
```cpp
MyClass obj;
method meth = type::get<MyClass>().get_method("calculate");

// Invoke method
variant result = meth.invoke(obj, arg1, arg2);

// Check if invoke was successful
if (result.is_valid()) {
    // Success
}

// Method info
type return_type = meth.get_return_type();
auto param_types = meth.get_parameter_types();
std::string signature = meth.get_signature();
```

### Object Creation
```cpp
type t = type::get_by_name("MyClass");

// Create with default constructor
variant obj = t.create();

// Create with parameters
variant obj = t.create({arg1, arg2});

// Using specific constructor
constructor ctor = t.get_constructor({type::get<int>()});
variant obj = ctor.invoke(42);
```

## Class Preparation

### Header File
```cpp
// my_class.h
#include <rttr/type>

class MyClass {
public:
    MyClass();
    // ... your methods ...
    
private:
    // ... your data ...
    
    RTTR_ENABLE()                    // Required for reflection
    RTTR_REGISTRATION_FRIEND         // Optional: for private access
};
```

### Source File
```cpp
// my_class.cpp
#include "my_class.h"
#include <rttr/registration>

RTTR_REGISTRATION {
    using namespace rttr;
    // ... registration code ...
}
```

## Inheritance

### Base Class
```cpp
class Base {
    RTTR_ENABLE()  // Always in base
};

RTTR_REGISTRATION {
    registration::class_<Base>("Base")
        // ... base registration ...
        ;
}
```

### Derived Class
```cpp
class Derived : public Base {
    RTTR_ENABLE(Base)  // Specify base class
};

RTTR_REGISTRATION {
    registration::class_<Derived>("Derived")
        // ... derived registration ...
        ;
}
```

## Common Patterns

### Generic Property Inspector
```cpp
void print_properties(const variant& obj) {
    type t = obj.get_type();
    for (auto& prop : t.get_properties()) {
        variant value = prop.get_value(obj);
        std::cout << prop.get_name() << ": " << value.to_string() << std::endl;
    }
}
```

### Type-Safe Downcast
```cpp
variant base_obj = get_base_object();
type derived_type = type::get_by_name("DerivedClass");

if (base_obj.get_type().is_derived_from(derived_type)) {
    variant derived_obj = base_obj.convert(derived_type);
    // Safe to use as derived type
}
```

### Dynamic Method Calling
```cpp
bool call_method(variant& obj, const std::string& method_name) {
    type t = obj.get_type();
    method meth = t.get_method(method_name);
    
    if (meth.is_valid()) {
        variant result = meth.invoke(obj);
        return result.is_valid();
    }
    return false;
}
```

## Error Handling

### Check Validity
```cpp
// Always check if operations succeeded
type t = type::get_by_name("UnknownClass");
if (!t.is_valid()) {
    // Type not found
}

property prop = t.get_property("unknown_prop");
if (!prop.is_valid()) {
    // Property not found
}

variant result = method.invoke(obj, args);
if (!result.is_valid()) {
    // Method call failed
}
```

### Common Failure Causes
- Forgetting `RTTR_ENABLE()` in class
- Type not registered
- Wrong parameter types for method calls
- Accessing non-existent properties/methods
- Trying to write to read-only properties

---

*This reference covers the most common RTTR usage patterns. For complete documentation, see the [RTTR Learning Series](README.md).*