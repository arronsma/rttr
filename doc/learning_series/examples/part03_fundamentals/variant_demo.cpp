/**
 * Variant Demo - Part 3 of RTTR Learning Series
 * 
 * This example demonstrates:
 * - Creating and using variants
 * - Type safety and conversion
 * - Variant state checking
 * - Working with different data types
 */

#include <rttr/type>
#include <iostream>
#include <string>
#include <vector>

using namespace rttr;

void demonstrate_variant_creation() {
    std::cout << "=== Variant Creation Demo ===\n\n";
    
    // Create variants with different types
    variant int_var = 42;
    variant double_var = 3.14159;
    variant string_var = std::string("Hello RTTR");
    variant bool_var = true;
    
    std::cout << "Variant types and values:\n";
    std::cout << "  int_var: " << int_var.get_type().get_name() 
              << " = " << int_var.to_string() << "\n";
    std::cout << "  double_var: " << double_var.get_type().get_name() 
              << " = " << double_var.to_string() << "\n";
    std::cout << "  string_var: " << string_var.get_type().get_name() 
              << " = " << string_var.to_string() << "\n";
    std::cout << "  bool_var: " << bool_var.get_type().get_name() 
              << " = " << bool_var.to_string() << "\n\n";
}

void demonstrate_variant_extraction() {
    std::cout << "=== Variant Value Extraction Demo ===\n\n";
    
    variant int_var = 100;
    variant double_var = 2.718;
    variant string_var = std::string("RTTR Rocks!");
    
    // Safe extraction with type checking
    if (int_var.can_convert<int>()) {
        int value = int_var.get_value<int>();
        std::cout << "int_var value: " << value << "\n";
    }
    
    if (double_var.can_convert<double>()) {
        double value = double_var.get_value<double>();
        std::cout << "double_var value: " << value << "\n";
    }
    
    if (string_var.can_convert<std::string>()) {
        std::string value = string_var.get_value<std::string>();
        std::cout << "string_var value: " << value << "\n";
    }
    
    std::cout << "\n";
}

void demonstrate_variant_conversion() {
    std::cout << "=== Variant Conversion Demo ===\n\n";
    
    variant int_var = 42;
    variant double_var = 3.14159;
    variant string_var = std::string("123");
    
    std::cout << "Automatic conversions:\n";
    
    // int to double
    if (int_var.can_convert<double>()) {
        double converted = int_var.to_double();
        std::cout << "  int " << int_var.to_string() 
                  << " to double: " << converted << "\n";
    }
    
    // double to int (truncation)
    if (double_var.can_convert<int>()) {
        int converted = double_var.to_int();
        std::cout << "  double " << double_var.to_string() 
                  << " to int: " << converted << "\n";
    }
    
    // string to int (if possible)
    if (string_var.can_convert<int>()) {
        int converted = string_var.to_int();
        std::cout << "  string \"" << string_var.to_string() 
                  << "\" to int: " << converted << "\n";
    }
    
    // int to string
    if (int_var.can_convert<std::string>()) {
        std::string converted = int_var.to_string();
        std::cout << "  int " << int_var.get_value<int>() 
                  << " to string: \"" << converted << "\"\n";
    }
    
    std::cout << "\n";
}

void demonstrate_variant_state() {
    std::cout << "=== Variant State Demo ===\n\n";
    
    // Valid variants
    variant valid_var = 42;
    std::cout << "Valid variant:\n";
    std::cout << "  is_valid(): " << valid_var.is_valid() << "\n";
    std::cout << "  is_null(): " << valid_var.is_null() << "\n";
    std::cout << "  type: " << valid_var.get_type().get_name() << "\n\n";
    
    // Invalid/empty variant
    variant empty_var;
    std::cout << "Empty variant:\n";
    std::cout << "  is_valid(): " << empty_var.is_valid() << "\n";
    std::cout << "  is_null(): " << empty_var.is_null() << "\n";
    std::cout << "  type: " << empty_var.get_type().get_name() << "\n\n";
    
    // Null pointer variant
    int* null_ptr = nullptr;
    variant null_var = null_ptr;
    std::cout << "Null pointer variant:\n";
    std::cout << "  is_valid(): " << null_var.is_valid() << "\n";
    std::cout << "  is_null(): " << null_var.is_null() << "\n";
    std::cout << "  type: " << null_var.get_type().get_name() << "\n\n";
}

void demonstrate_variant_containers() {
    std::cout << "=== Variant Containers Demo ===\n\n";
    
    // Vector of variants
    std::vector<variant> variant_list = {
        42,
        3.14159,
        std::string("Hello"),
        true,
        'A'
    };
    
    std::cout << "Vector of variants:\n";
    for (size_t i = 0; i < variant_list.size(); ++i) {
        const auto& var = variant_list[i];
        std::cout << "  [" << i << "] " << var.get_type().get_name() 
                  << " = " << var.to_string() << "\n";
    }
    std::cout << "\n";
    
    // Process mixed types
    std::cout << "Processing mixed types:\n";
    for (const auto& var : variant_list) {
        type t = var.get_type();
        
        if (t.is_arithmetic()) {
            std::cout << "  Arithmetic value: " << var.to_string() << "\n";
        } else if (t == type::get<std::string>()) {
            std::cout << "  String value: \"" << var.to_string() << "\"\n";
        } else {
            std::cout << "  Other type: " << t.get_name() << " = " << var.to_string() << "\n";
        }
    }
    std::cout << "\n";
}

void demonstrate_variant_assignment() {
    std::cout << "=== Variant Assignment Demo ===\n\n";
    
    variant var;
    std::cout << "Initial state: " << var.get_type().get_name() << "\n";
    
    // Assign different types
    var = 42;
    std::cout << "After int assignment: " << var.get_type().get_name() 
              << " = " << var.to_string() << "\n";
    
    var = 3.14159;
    std::cout << "After double assignment: " << var.get_type().get_name() 
              << " = " << var.to_string() << "\n";
    
    var = std::string("Dynamic typing!");
    std::cout << "After string assignment: " << var.get_type().get_name() 
              << " = " << var.to_string() << "\n";
    
    // Clear variant
    var.clear();
    std::cout << "After clear: " << var.get_type().get_name() 
              << " (valid: " << var.is_valid() << ")\n\n";
}

int main() {
    std::cout << "RTTR Learning Series - Part 3: Variant Demo\n";
    std::cout << "===========================================\n\n";
    
    demonstrate_variant_creation();
    demonstrate_variant_extraction();
    demonstrate_variant_conversion();
    demonstrate_variant_state();
    demonstrate_variant_containers();
    demonstrate_variant_assignment();
    
    std::cout << "Demo completed successfully!\n";
    return 0;
}