//
// Created by hyn on 2023/2/5.
//

#ifndef CPP_REFLECT_CLASSREGISTER_H
#define CPP_REFLECT_CLASSREGISTER_H

#include "ClassFactory.h"

namespace hyn::reflect {
    class ClassRegister {
    public:
        ClassRegister(const std::string &class_name, create_object method) {
            std::cout << class_name << " register " << std::endl;
            sigleton::Sigleton<ClassFactory>::get_instance()->register_class(class_name, method);
        }

        ClassRegister(const std::string &class_name, const std::string &field_name, const std::string &type,
                      const size_t &offset) {
            std::cout << class_name << " register field " << field_name << std::endl;
            sigleton::Sigleton<ClassFactory>::get_instance()->register_class_field(class_name, field_name, type,
                                                                                   offset);
        }

        ClassRegister(const std::string &class_name, const std::string &method_name, uintptr_t method) {
            std::cout << class_name << " register method " << method_name << std::endl;
            sigleton::Sigleton<ClassFactory>::get_instance()->register_class_method(class_name, method_name, method);
        }
    };

#define REGISTER_CLASS(class_name)                          \
    hyn::reflect::Object* creat_object_##class_name() {     \
        hyn::reflect::Object* obj = new class_name();       \
        obj->set_m_class_name(#class_name);                 \
        return obj;                                         \
    }                                                       \
    hyn::reflect::ClassRegister class_register_##class_name (#class_name,creat_object_##class_name)


#define REGISTER_CLASS_FIELD(class_name, field_name, field_type) \
class_name class_name##field_name;                               \
hyn::reflect::ClassRegister class_register_##class_name##field_name(#class_name,#field_name,#field_type,((size_t)(&(class_name##field_name).field_name)-(size_t)(&class_name##field_name)))


#define REGISTER_CLASS_METHOD(class_name, method_name)                                \
std::function<void(class_name *)> class_name##method_name##_method = &class_name::method_name; \
hyn::reflect::ClassRegister class_register_##class_name##method_name(#class_name,#method_name,(uintptr_t)&(class_name##method_name##_method))

}
#endif //CPP_REFLECT_CLASSREGISTER_H
