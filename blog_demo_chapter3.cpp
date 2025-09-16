/**
 * RTTR第三篇博客示例代码
 * 标题：RTTR核心概念总览 - 构建完整认知框架
 * 
 * 本示例通过一个简单的学生管理系统，展示RTTR的核心概念
 * 重点：理解概念职责，避免陷入实现细节
 */

#include <rttr/registration>
#include <rttr/registration_friend>
#include <iostream>
#include <string>

using namespace rttr;

// 简单的学生类
class Student {
public:
    Student() = default;
    Student(const std::string& name, int age) : name_(name), age_(age) {}
    
    // 基础属性访问
    const std::string& getName() const { return name_; }
    void setName(const std::string& name) { name_ = name; }
    int getAge() const { return age_; }
    void setAge(int age) { age_ = age; }
    
    // 业务方法
    void introduce() const {
        std::cout << "Hi, I'm " << name_ << ", " << age_ << " years old." << std::endl;
    }
    
    bool isAdult() const {
        return age_ >= 18;
    }

private:
    std::string name_;
    int age_ = 0;
    
    RTTR_REGISTRATION_FRIEND
};

// ============================================================================
// 关键点1: registration - 反射信息的"登记处"
// ============================================================================
RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<Student>("Student")
        .constructor<>()
        .constructor<const std::string&, int>()
        .property("name", &Student::getName, &Student::setName)
        .property("age", &Student::getAge, &Student::setAge)
        .method("introduce", &Student::introduce)
        .method("isAdult", &Student::isAdult);
}

// ============================================================================
// 核心概念演示
// ============================================================================

void demo_step1_type_as_index() {
    std::cout << "\n=== 步骤1: type作为类型信息的'索引' ===\n";
    
    // 核心概念：type是类型信息的"索引"，通过它可以查询类型的所有反射信息
    type student_type = type::get<Student>();
    
    std::cout << "类型名: " << student_type.get_name() << std::endl;
    std::cout << "构造函数数量: " << student_type.get_constructors().size() << std::endl;
    std::cout << "属性数量: " << student_type.get_properties().size() << std::endl;
    std::cout << "方法数量: " << student_type.get_methods().size() << std::endl;
    
    // 重点理解：type就像一个"目录"，告诉你这个类型有哪些成员
}

void demo_step2_variant_as_container() {
    std::cout << "\n=== 步骤2: variant作为万能'容器' ===\n";
    
    // 核心概念：variant是类型安全的万能容器，可以存储任意类型的值
    variant int_var = 42;
    variant string_var = std::string("Hello");
    variant student_var = Student("Alice", 20);
    
    std::cout << "整数变量类型: " << int_var.get_type().get_name() 
              << ", 值: " << int_var.to_int() << std::endl;
    std::cout << "字符串变量类型: " << string_var.get_type().get_name() 
              << ", 值: " << string_var.to_string() << std::endl;
    std::cout << "学生变量类型: " << student_var.get_type().get_name() << std::endl;
    
    // 重点理解：variant就像一个"万能盒子"，可以装任何东西，同时记住装的是什么
}

void demo_step3_constructor_as_factory() {
    std::cout << "\n=== 步骤3: constructor作为对象'工厂' ===\n";
    
    // 核心概念：constructor是对象的"工厂"，负责创建对象实例
    type student_type = type::get<Student>();
    
    // 使用默认构造函数
    constructor default_ctor = student_type.get_constructor();
    variant student1 = default_ctor.invoke();
    std::cout << "默认构造函数创建的对象类型: " << student1.get_type().get_name() << std::endl;
    
    // 使用带参数的构造函数
    constructor param_ctor = student_type.get_constructor({type::get<std::string>(), type::get<int>()});
    variant student2 = param_ctor.invoke(std::string("Bob"), 22);
    
    // 重点理解：constructor就像"工厂"，根据不同的"模板"（参数）生产对象
}

void demo_step4_property_as_accessor() {
    std::cout << "\n=== 步骤4: property作为属性'访问器' ===\n";
    
    // 核心概念：property是属性的"访问器"，提供类型安全的读写操作
    type student_type = type::get<Student>();
    Student student("Charlie", 19);
    
    property name_prop = student_type.get_property("name");
    property age_prop = student_type.get_property("age");
    
    // 读取属性
    variant name_value = name_prop.get_value(student);
    variant age_value = age_prop.get_value(student);
    std::cout << "读取到的姓名: " << name_value.to_string() << std::endl;
    std::cout << "读取到的年龄: " << age_value.to_int() << std::endl;
    
    // 设置属性
    name_prop.set_value(student, std::string("Charlie Updated"));
    age_prop.set_value(student, 20);
    
    // 验证设置结果
    std::cout << "更新后的姓名: " << student.getName() << std::endl;
    std::cout << "更新后的年龄: " << student.getAge() << std::endl;
    
    // 重点理解：property就像"遥控器"，可以远程操作对象的属性
}

void demo_step5_method_as_invoker() {
    std::cout << "\n=== 步骤5: method作为方法'调用器' ===\n";
    
    // 核心概念：method是方法的"调用器"，提供类型安全的方法调用
    type student_type = type::get<Student>();
    Student student("David", 25);
    
    // 调用无返回值的方法
    method introduce_method = student_type.get_method("introduce");
    introduce_method.invoke(student);
    
    // 调用有返回值的方法
    method adult_method = student_type.get_method("isAdult");
    variant result = adult_method.invoke(student);
    std::cout << "是否成年: " << (result.to_bool() ? "是" : "否") << std::endl;
    
    // 重点理解：method就像"代理人"，可以代你调用对象的方法
}

void demo_step6_complete_workflow() {
    std::cout << "\n=== 步骤6: 完整的反射工作流程 ===\n";
    
    // 完整流程：从字符串类型名到对象操作
    std::string type_name = "Student";
    
    // 1. 通过名称获取类型信息
    type student_type = type::get_by_name(type_name);
    if (!student_type.is_valid()) {
        std::cout << "类型 " << type_name << " 未找到" << std::endl;
        return;
    }
    
    // 2. 动态创建对象
    variant student_obj = student_type.create({std::string("Eva"), 21});
    
    // 3. 动态访问属性
    property name_prop = student_type.get_property("name");
    if (name_prop.is_valid()) {
        variant name = name_prop.get_value(student_obj);
        std::cout << "动态读取的姓名: " << name.to_string() << std::endl;
    }
    
    // 4. 动态调用方法
    method introduce_method = student_type.get_method("introduce");
    if (introduce_method.is_valid()) {
        introduce_method.invoke(student_obj);
    }
    
    std::cout << "完整的反射工作流程演示完成!" << std::endl;
    
    // 重点理解：所有核心概念协作完成"动态编程"的能力
}

// ============================================================================
// 学习要点总结
// ============================================================================
void print_learning_summary() {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "RTTR核心概念学习要点总结" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    std::cout << "\n六大核心概念的职责分工：" << std::endl;
    std::cout << "1. registration: 反射信息的'登记处' - 编译时注册" << std::endl;
    std::cout << "2. type:         类型信息的'索引'   - 运行时查询" << std::endl;
    std::cout << "3. variant:      万能的'容器'      - 类型安全存储" << std::endl;
    std::cout << "4. constructor:  对象的'工厂'      - 动态创建" << std::endl;
    std::cout << "5. property:     属性的'访问器'    - 动态读写" << std::endl;
    std::cout << "6. method:       方法的'调用器'    - 动态调用" << std::endl;
    
    std::cout << "\n学习方法要点：" << std::endl;
    std::cout << "✓ 从概念职责开始理解，不要急于深入实现" << std::endl;
    std::cout << "✓ 通过完整示例掌握协作关系" << std::endl;
    std::cout << "✓ 先会用API，再研究原理" << std::endl;
    std::cout << "✓ 避免过早陷入模板元编程细节" << std::endl;
    
    std::cout << "\n下一步学习建议：" << std::endl;
    std::cout << "→ 深入学习每个概念的详细API" << std::endl;
    std::cout << "→ 实践序列化、配置系统等应用" << std::endl;
    std::cout << "→ 研究高级特性（metadata、policy等）" << std::endl;
    std::cout << "→ 最后探索实现原理和设计思想" << std::endl;
}

// ============================================================================
// 主函数
// ============================================================================
int main() {
    std::cout << "RTTR核心概念总览演示程序" << std::endl;
    std::cout << "目标：构建完整的认知框架，避免迷失在实现细节中" << std::endl;
    
    // 按步骤演示各个核心概念
    demo_step1_type_as_index();
    demo_step2_variant_as_container();
    demo_step3_constructor_as_factory();
    demo_step4_property_as_accessor();
    demo_step5_method_as_invoker();
    demo_step6_complete_workflow();
    
    // 总结学习要点
    print_learning_summary();
    
    return 0;
}