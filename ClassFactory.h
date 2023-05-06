//
// Created by hyn on 2023/2/5.
//

#ifndef CPP_REFLECT_CLASSFACTORY_H
#define CPP_REFLECT_CLASSFACTORY_H

#include <functional>
#include <map>
#include <string>
#include <vector>
#include "sigleton.h"
#include "ClassField.h"
#include "ClassMethod.h"

namespace hyn::reflect {


    class Object {
    public:
        Object() = default;

        virtual ~Object() = default;

        int get_field_count();

        ClassField *get_field(int pos);

        ClassField *get_field(const std::string &field_name);

        template<typename T>
        void get(const std::string &field_name, T &value);

        template<typename T>
        void set(const std::string &field_name, const T &value);

        void call(const std::string &method_name);

        virtual void show() = 0;//for test

    private:
        std::string m_class_name;
    public:
        [[nodiscard]] const std::string &get_m_class_name() const;

        void set_m_class_name(const std::string &class_name);
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

    public:
        void register_class_method(const std::string &class_name, const std::string &method_name, uintptr_t method);

        int get_class_method_count(const std::string &class_name);

        ClassMethod *get_method(const std::string &class_name, int pos);

        ClassMethod *get_method(const std::string &class_name, const std::string &method_name);

    private:
        ClassFactory() = default;

        ~ClassFactory() = default;

        std::map<std::string, hyn::reflect::create_object> m_class_map;
        std::map<std::string, std::vector<ClassField *>> m_class_fields;
        std::map<std::string, std::vector<ClassMethod *>> m_class_method;

    };
}


template<typename T>
void hyn::reflect::Object::set(const std::string &field_name, const T &value) {
    ClassFactory *factory = sigleton::Sigleton<ClassFactory>::get_instance();
    ClassField *field = factory->get_field(m_class_name, field_name);
    size_t offset = field->get_offset();
    *(T *) ((unsigned char *) (this) + offset) = value;
}

template<typename T>
void hyn::reflect::Object::get(const std::string &field_name, T &value) {
    ClassFactory *factory = sigleton::Sigleton<ClassFactory>::get_instance();
    ClassField *field = factory->get_field(m_class_name, field_name);
    size_t offset = field->get_offset();
    value = *(T *) ((unsigned char *) (this) + offset);
}


#endif //CPP_REFLECT_CLASSFACTORY_H
