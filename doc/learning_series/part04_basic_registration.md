# Part 4: Basic Registration Patterns

> **Learning Objective**: Master the fundamental patterns for registering C++ classes, methods, and properties with RTTR.

## Overview

Registration is how you tell RTTR about your C++ types. In this part, we'll cover:

- The `RTTR_REGISTRATION` block structure
- Class registration patterns
- Property registration (member variables vs. getter/setter pairs)
- Method registration with various signatures
- Constructor and destructor registration
- Best practices and common pitfalls

## Registration Block Structure

All RTTR registration happens inside `RTTR_REGISTRATION` blocks, typically in `.cpp` files:

```cpp
#include <rttr/registration>

RTTR_REGISTRATION {
    using namespace rttr;
    
    // All registration code goes here
    registration::class_<MyClass>("MyClass")
        .constructor<>()
        .property("name", &MyClass::name)
        .method("do_something", &MyClass::do_something)
        ;
}
```

**Key Points:**
- One registration block per translation unit (`.cpp` file)
- Executed during static initialization
- Order of registration within a block doesn't matter
- Registration across files happens in undefined order

## Class Registration Fundamentals

### Basic Class Registration

```cpp
// player.h
#include <rttr/type>
#include <string>

class Player {
public:
    Player() = default;
    Player(const std::string& name) : name_(name) {}
    
    const std::string& get_name() const { return name_; }
    void set_name(const std::string& name) { name_ = name; }
    
    int get_health() const { return health_; }
    void set_health(int health) { health_ = health; }
    
    void take_damage(int damage) { health_ -= damage; }
    bool is_alive() const { return health_ > 0; }
    
private:
    std::string name_;
    int health_ = 100;
    
    RTTR_ENABLE() // Always add this for classes you want to reflect
};
```

```cpp
// player.cpp
#include "player.h"
#include <rttr/registration>

RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<Player>("Player")
        .constructor<>()
        .constructor<const std::string&>()
        .property("name", &Player::get_name, &Player::set_name)
        .property("health", &Player::get_health, &Player::set_health)
        .method("take_damage", &Player::take_damage)
        .method("is_alive", &Player::is_alive)
        ;
}
```

### Registration Naming Strategies

```cpp
RTTR_REGISTRATION {
    using namespace rttr;
    
    // Strategy 1: Simple names
    registration::class_<Player>("Player");
    
    // Strategy 2: Namespace-qualified names  
    registration::class_<Player>("game::Player");
    
    // Strategy 3: Hierarchical naming
    registration::class_<Player>("Game.Entities.Player");
    
    // Strategy 4: Use actual C++ qualified name
    registration::class_<Player>(type::get<Player>().get_name());
}
```

**Naming Best Practices:**
- Be consistent across your project
- Consider scripting language conventions if binding to scripts
- Include namespace info for disambiguation
- Keep names readable and meaningful

## Property Registration Patterns

### Method-Based Properties (Recommended)

```cpp
class Character {
public:
    const std::string& get_name() const { return name_; }
    void set_name(const std::string& name) { name_ = name; }
    
    int get_level() const { return level_; }
    void set_level(int level) { 
        if (level >= 1 && level <= 100) {
            level_ = level; 
        }
    }
    
    // Read-only property
    int get_experience() const { return experience_; }
    
private:
    std::string name_;
    int level_ = 1;
    int experience_ = 0;
    
    RTTR_ENABLE()
};

RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<Character>("Character")
        // Read-write property
        .property("name", &Character::get_name, &Character::set_name)
        
        // Read-write with validation in setter
        .property("level", &Character::get_level, &Character::set_level)
        
        // Read-only property
        .property_readonly("experience", &Character::get_experience)
        ;
}
```

### Direct Member Access

```cpp
class Point {
public:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    
    RTTR_ENABLE()
};

RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<Point>("Point")
        .property("x", &Point::x)
        .property("y", &Point::y)
        .property("z", &Point::z)
        ;
}
```

### Private Member Access

```cpp
class BankAccount {
public:
    BankAccount(const std::string& owner) : owner_(owner) {}
    
    const std::string& get_owner() const { return owner_; }
    double get_balance() const { return balance_; }
    
    void deposit(double amount) { balance_ += amount; }
    bool withdraw(double amount) {
        if (amount <= balance_) {
            balance_ -= amount;
            return true;
        }
        return false;
    }
    
private:
    std::string owner_;
    double balance_ = 0.0;
    std::string account_number_ = "SECRET123";
    
    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND // Allow registration access to private members
};

RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<BankAccount>("BankAccount")
        .constructor<const std::string&>()
        
        // Public getters - normal registration
        .property_readonly("owner", &BankAccount::get_owner)
        .property_readonly("balance", &BankAccount::get_balance)
        
        // Private member access - requires RTTR_REGISTRATION_FRIEND
        .property("account_number", &BankAccount::account_number_, registration::private_access)
        
        .method("deposit", &BankAccount::deposit)
        .method("withdraw", &BankAccount::withdraw)
        ;
}
```

## Method Registration Patterns

### Basic Method Registration

```cpp
class Calculator {
public:
    int add(int a, int b) { return a + b; }
    double divide(double a, double b) { return b != 0 ? a / b : 0; }
    
    // Overloaded methods
    double power(double base, double exponent) { return std::pow(base, exponent); }
    double power(double base, int exponent) { return std::pow(base, static_cast<double>(exponent)); }
    
    // Const methods
    std::string get_name() const { return "Calculator"; }
    
    // Static methods
    static double pi() { return 3.14159265359; }
    
    RTTR_ENABLE()
};

RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<Calculator>("Calculator")
        .constructor<>()
        
        // Basic methods
        .method("add", &Calculator::add)
        .method("divide", &Calculator::divide)
        
        // Overloaded methods - use method resolution
        .method("power", 
                select_overload<double(double, double)>(&Calculator::power))
        .method("power_int", 
                select_overload<double(double, int)>(&Calculator::power))
        
        // Const methods
        .method("get_name", &Calculator::get_name)
        
        // Static methods  
        .method("pi", &Calculator::pi)
        ;
}
```

### Method Resolution for Overloads

```cpp
class MathUtils {
public:
    // Multiple overloads
    float abs(float x) { return std::abs(x); }
    double abs(double x) { return std::abs(x); }
    int abs(int x) { return std::abs(x); }
    
    RTTR_ENABLE()
};

RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<MathUtils>("MathUtils")
        // Option 1: Use select_overload
        .method("abs_float", select_overload<float(float)>(&MathUtils::abs))
        .method("abs_double", select_overload<double(double)>(&MathUtils::abs))
        .method("abs_int", select_overload<int(int)>(&MathUtils::abs))
        
        // Option 2: Use static_cast
        .method("abs_f", static_cast<float(MathUtils::*)(float)>(&MathUtils::abs))
        ;
}
```

### Virtual and Abstract Methods

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void draw() const { std::cout << "Drawing shape\n"; }
    
    RTTR_ENABLE()
};

class Circle : public Shape {
public:
    Circle(double radius) : radius_(radius) {}
    
    double area() const override { return 3.14159 * radius_ * radius_; }
    double perimeter() const override { return 2 * 3.14159 * radius_; }
    void draw() const override { std::cout << "Drawing circle\n"; }
    
    double get_radius() const { return radius_; }
    
private:
    double radius_;
    
    RTTR_ENABLE(Shape) // Specify base class
};

RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<Shape>("Shape")
        // Register abstract methods - they work with derived instances
        .method("area", &Shape::area)
        .method("perimeter", &Shape::perimeter)
        .method("draw", &Shape::draw)
        ;
    
    registration::class_<Circle>("Circle")
        .constructor<double>()
        .property_readonly("radius", &Circle::get_radius)
        // Inherited methods automatically available
        ;
}
```

## Constructor Registration

### Basic Constructors

```cpp
class Vehicle {
public:
    Vehicle() : make_("Unknown"), model_("Unknown") {}
    Vehicle(const std::string& make) : make_(make), model_("Unknown") {}
    Vehicle(const std::string& make, const std::string& model) 
        : make_(make), model_(model) {}
    
    // Copy constructor
    Vehicle(const Vehicle& other) : make_(other.make_), model_(other.model_) {}
    
    const std::string& get_make() const { return make_; }
    const std::string& get_model() const { return model_; }
    
private:
    std::string make_;
    std::string model_;
    
    RTTR_ENABLE()
};

RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<Vehicle>("Vehicle")
        // Default constructor
        .constructor<>()
        
        // Single parameter constructor
        .constructor<const std::string&>()
        
        // Multiple parameter constructor
        .constructor<const std::string&, const std::string&>()
        
        // Copy constructor
        .constructor<const Vehicle&>()
        
        .property_readonly("make", &Vehicle::get_make)
        .property_readonly("model", &Vehicle::get_model)
        ;
}
```

### Factory Method Constructors

```cpp
class Database {
public:
    // Factory methods instead of public constructors
    static std::shared_ptr<Database> create_sqlite(const std::string& filename) {
        return std::shared_ptr<Database>(new Database("sqlite", filename));
    }
    
    static std::shared_ptr<Database> create_mysql(const std::string& host, int port) {
        return std::shared_ptr<Database>(new Database("mysql", host + ":" + std::to_string(port)));
    }
    
    const std::string& get_type() const { return type_; }
    const std::string& get_connection() const { return connection_; }
    
private:
    Database(const std::string& type, const std::string& connection)
        : type_(type), connection_(connection) {}
    
    std::string type_;
    std::string connection_;
    
    RTTR_ENABLE()
};

RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<Database>("Database")
        // Register factory methods as constructors
        .constructor(&Database::create_sqlite)
        .constructor(&Database::create_mysql)
        
        .property_readonly("type", &Database::get_type)
        .property_readonly("connection", &Database::get_connection)
        ;
}
```

## Registration Best Practices

### 1. Consistent Registration Style

```cpp
// Good - consistent pattern
RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<Player>("Player")
        .constructor<>()
        .constructor<const std::string&>()
        .property("name", &Player::get_name, &Player::set_name)
        .property("health", &Player::get_health, &Player::set_health)
        .method("take_damage", &Player::take_damage)
        .method("is_alive", &Player::is_alive)
        ;
}
```

### 2. Group Related Registrations

```cpp
RTTR_REGISTRATION {
    using namespace rttr;
    
    // Register base classes first
    registration::class_<GameObject>("GameObject")
        .property("position", &GameObject::get_position, &GameObject::set_position)
        .method("update", &GameObject::update)
        ;
    
    // Then derived classes
    registration::class_<Player>("Player")
        .constructor<>()
        .property("health", &Player::get_health, &Player::set_health)
        ;
    
    registration::class_<Enemy>("Enemy")
        .constructor<>()
        .property("damage", &Enemy::get_damage, &Enemy::set_damage)
        ;
}
```

### 3. Use Meaningful Names

```cpp
RTTR_REGISTRATION {
    using namespace rttr;
    
    registration::class_<Player>("Player")
        // Use clear, consistent naming
        .property("player_name", &Player::get_name, &Player::set_name)  // not just "name"
        .property("current_health", &Player::get_health, &Player::set_health)
        .method("receive_damage", &Player::take_damage)  // clear about direction
        .method("check_alive_status", &Player::is_alive)
        ;
}
```

### 4. Handle Registration Errors

```cpp
void register_player_type() {
    using namespace rttr;
    
    auto reg = registration::class_<Player>("Player");
    
    // Check if registration succeeded
    if (!reg.constructor<>()) {
        std::cerr << "Failed to register Player default constructor\n";
    }
    
    if (!reg.property("name", &Player::get_name, &Player::set_name)) {
        std::cerr << "Failed to register Player name property\n";
    }
}

RTTR_REGISTRATION {
    register_player_type();
}
```

## Common Pitfalls and Solutions

### 1. Missing RTTR_ENABLE()

```cpp
// Bad - will cause runtime errors
class MyClass {
    // Missing RTTR_ENABLE()
};

// Good 
class MyClass {
    RTTR_ENABLE()
};
```

### 2. Registration Order Dependencies

```cpp
// Problematic - base class might not be registered yet
RTTR_REGISTRATION {
    registration::class_<Derived>("Derived");  // Depends on Base
    registration::class_<Base>("Base");        // Registered after
}

// Better - register base classes first
RTTR_REGISTRATION {
    registration::class_<Base>("Base");
    registration::class_<Derived>("Derived"); 
}
```

### 3. Forgetting const in Method Signatures

```cpp
class MyClass {
public:
    int get_value() const { return value_; }  // const method
private:
    int value_;
};

// Wrong - missing const
.method("get_value", &MyClass::get_value)  // Won't match const method

// Correct
.method("get_value", &MyClass::get_value)  // Automatically handles const
```

## Exercises

### Exercise 1: Complete Registration
Create a `Book` class with title, author, pages, and ISBN. Register it completely including:
- Multiple constructors
- All properties (some read-only)
- Methods for getting info and checking validity

### Exercise 2: Inheritance Registration
Create a hierarchy: `Animal` → `Mammal` → `Dog`. Register all classes with appropriate properties and methods.

### Exercise 3: Factory Pattern
Create a `Logger` class that can only be created through factory methods (`create_file_logger`, `create_console_logger`). Register it properly.

## What's Next?

In [Part 5: Properties and Methods](part05_properties_methods.md), we'll dive deeper into:
- Advanced property patterns and metadata
- Method parameter handling and default arguments
- Property and method queries and introspection
- Custom property behaviors

## Key Takeaways

1. **Registration is declarative** - describe what you want to expose
2. **Use consistent patterns** - makes code maintainable
3. **Register base classes first** - ensures proper inheritance
4. **RTTR_ENABLE() is crucial** - don't forget it in class definitions
5. **Method overloads need resolution** - use select_overload or casts
6. **Factory methods can be constructors** - very flexible pattern

---

**Previous**: [Part 3: RTTR Fundamentals](part03_fundamentals.md) | **Next**: [Part 5: Properties and Methods](part05_properties_methods.md)

**Back to**: [Learning Series Index](README.md)