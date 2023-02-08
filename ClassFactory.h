//
// Created by hyn on 2023/2/5.
//

#ifndef CPP_REFLECT_CLASSFACTORY_H
#define CPP_REFLECT_CLASSFACTORY_H

#include <map>
#include <string>
#include <vector>
#include "sigleton.h"
#include "ClassField.h"

namespace hyn::reflect {

    class Object {
    public:
        Object() = default;

        virtual ~Object() = default;

        virtual void show() = 0;
    };

    typedef Object *(*create_object)();

    class ClassFactory {
        friend sigleton::Sigleton<ClassFactory>;
    public:
        void register_class(const std::string &class_name, hyn::reflect::create_object method);

        Object *create_class(const std::string &class_name);

    public:
        void register_class_field(const std::string &class_name, std::string name, std::string type, size_t offset);

        int get_field_cont(const std::string &class_name);

        ClassField *get_field(const std::string &class_name, int pos);

        ClassField *get_field(const std::string &class_name, const std::string &field_name);

    private:
        ClassFactory() = default;

        ~ClassFactory() = default;

        std::map<std::string, hyn::reflect::create_object> m_class_map;
        std::map<std::string, std::vector<ClassField *>> m_class_fields;
    };
}


#endif //CPP_REFLECT_CLASSFACTORY_H
