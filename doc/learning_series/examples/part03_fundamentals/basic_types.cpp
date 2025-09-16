/**
 * Basic Types Demo - Part 3 of RTTR Learning Series
 * 
 * This example demonstrates:
 * - Getting type information for built-in types
 * - Type comparison and queries  
 * - Basic type introspection
 */

#include <rttr/type>
#include <iostream>
#include <string>
#include <vector>

using namespace rttr;

void demonstrate_basic_types() {
    std::cout << "=== Basic Types Demo ===\n\n";
    
    // Get type information for built-in types
    type int_type = type::get<int>();
    type double_type = type::get<double>();
    type string_type = type::get<std::string>();
    type vector_type = type::get<std::vector<int>>();
    
    std::cout << "Type names:\n";
    std::cout << "  int: " << int_type.get_name() << "\n";
    std::cout << "  double: " << double_type.get_name() << "\n";
    std::cout << "  string: " << string_type.get_name() << "\n";
    std::cout << "  vector<int>: " << vector_type.get_name() << "\n\n";
    
    // Type queries
    std::cout << "Type queries:\n";
    std::cout << "  int is arithmetic: " << int_type.is_arithmetic() << "\n";
    std::cout << "  int is class: " << int_type.is_class() << "\n";
    std::cout << "  string is arithmetic: " << string_type.is_arithmetic() << "\n";
    std::cout << "  string is class: " << string_type.is_class() << "\n";
    std::cout << "  vector is array: " << vector_type.is_array() << "\n";
    std::cout << "  vector is sequential container: " << vector_type.is_sequential_container() << "\n\n";
}

void demonstrate_type_comparison() {
    std::cout << "=== Type Comparison Demo ===\n\n";
    
    type int1 = type::get<int>();
    type int2 = type::get<int>();
    type double_type = type::get<double>();
    
    std::cout << "Type comparison:\n";
    std::cout << "  int == int: " << (int1 == int2) << "\n";
    std::cout << "  int == double: " << (int1 == double_type) << "\n";
    std::cout << "  int != double: " << (int1 != double_type) << "\n\n";
}

void demonstrate_pointer_types() {
    std::cout << "=== Pointer Types Demo ===\n\n";
    
    type int_type = type::get<int>();
    type int_ptr_type = type::get<int*>();
    type int_ref_type = type::get<int&>();
    
    std::cout << "Pointer and reference types:\n";
    std::cout << "  int: " << int_type.get_name() << "\n";
    std::cout << "  int*: " << int_ptr_type.get_name() << "\n";
    std::cout << "  int&: " << int_ref_type.get_name() << "\n\n";
    
    std::cout << "Type queries:\n";
    std::cout << "  int* is pointer: " << int_ptr_type.is_pointer() << "\n";
    std::cout << "  int& is reference: " << int_ref_type.is_reference() << "\n";
    std::cout << "  int* raw type: " << int_ptr_type.get_raw_type().get_name() << "\n";
    std::cout << "  int& raw type: " << int_ref_type.get_raw_type().get_name() << "\n\n";
}

void demonstrate_type_lookup() {
    std::cout << "=== Type Lookup Demo ===\n\n";
    
    // Get type by name
    type string_type = type::get_by_name("std::string");
    if (string_type.is_valid()) {
        std::cout << "Found type by name: " << string_type.get_name() << "\n";
    } else {
        std::cout << "Type 'std::string' not found by name\n";
    }
    
    // Alternative names might work
    type string_type2 = type::get_by_name("class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >");
    if (string_type2.is_valid()) {
        std::cout << "Found string type with full name\n";
    }
    
    // Get all registered types
    std::cout << "\nAll registered types:\n";
    for (auto& t : type::get_types()) {
        std::cout << "  " << t.get_name() << "\n";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "RTTR Learning Series - Part 3: Basic Types Demo\n";
    std::cout << "===============================================\n\n";
    
    demonstrate_basic_types();
    demonstrate_type_comparison();
    demonstrate_pointer_types();
    demonstrate_type_lookup();
    
    std::cout << "Demo completed successfully!\n";
    return 0;
}