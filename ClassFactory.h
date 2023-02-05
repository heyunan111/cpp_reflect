//
// Created by hyn on 2023/2/5.
//

#ifndef CPP_REFLECT_CLASSFACTORY_H
#define CPP_REFLECT_CLASSFACTORY_H
#include <map>
#include <string>
#include <iostream>
#include "sigleton.h"

namespace hyn::reflect {

    class Object {
    public:
        Object()= default;
        virtual ~Object() = default;
        virtual void show() = 0;
    };

    typedef Object* (*create_object)();

    class ClassFactory {
        friend sigleton::Sigleton<ClassFactory>;
    public:
        void register_class (const std::string& class_name,hyn::reflect::create_object method);
        Object* create_class (const std::string& class_name);
    private:
        ClassFactory()= default;
        ~ClassFactory()= default;
        std::map<std::string,hyn::reflect::create_object> m_class_map;
    };






}



#endif //CPP_REFLECT_CLASSFACTORY_H
