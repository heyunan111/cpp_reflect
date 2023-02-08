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
            sigleton::Sigleton<ClassFactory>::get_instance()->register_class(class_name, method);
        }
    };

#define REGISTER_CLASS(class_name)                          \
    hyn::reflect::Object* creat_object_##class_name() {     \
        hyn::reflect::Object* obj = new class_name();       \
        return obj;                                         \
    }                                                       \
    hyn::reflect::ClassRegister class_register_##class_name (#class_name,creat_object_##class_name);

}
#endif //CPP_REFLECT_CLASSREGISTER_H
