//
// Created by hyn on 2023/2/5.
//

#include "ClassFactory.h"

#include <utility>


/*                                class:ClassFactory                                                          */
void hyn::reflect::ClassFactory::register_class(const std::string &class_name, hyn::reflect::create_object method) {
    m_class_map[class_name] = method;
}

hyn::reflect::Object *hyn::reflect::ClassFactory::create_class(const std::string &class_name) {
    auto it = m_class_map.find(class_name);
    if (it == m_class_map.end())
        return nullptr;
    return it->second();
}

void hyn::reflect::ClassFactory::register_class_field(const std::string &class_name, std::string name, std::string type,
                                                      size_t offset) {
    m_class_fields[class_name].push_back(new ClassField(std::move(name), std::move(type), offset));

}

int hyn::reflect::ClassFactory::get_field_cont(const std::string &class_name) {
    return m_class_fields[class_name].size();
}

hyn::reflect::ClassField *hyn::reflect::ClassFactory::get_field(const std::string &class_name, int pos) {
    int size = m_class_fields[class_name].size();
    if (pos < 0 || pos > size)
        return nullptr;
    return m_class_fields[class_name][pos];
}

hyn::reflect::ClassField *
hyn::reflect::ClassFactory::get_field(const std::string &class_name, const std::string &field_name) {
    auto it = m_class_fields[class_name].begin();
    for (; it != m_class_fields[class_name].end(); ++it) {
        if ((*it)->get_name() == field_name)
            return (*it);
    }
    return nullptr;
}

void hyn::reflect::ClassFactory::register_class_method(const std::string &class_name, const std::string &method_name,
                                                       uintptr_t method) {
    m_class_method[class_name].push_back(new ClassMethod(method_name, method));
}

hyn::reflect::ClassMethod *hyn::reflect::ClassFactory::get_method(const std::string &class_name, int pos) {
    int size = m_class_method[class_name].size();
    if (pos < 0 || pos > size)
        return nullptr;
    return m_class_method[class_name][pos];
}

hyn::reflect::ClassMethod *
hyn::reflect::ClassFactory::get_method(const std::string &class_name, const std::string &method_name) {
    for (auto it = m_class_method[class_name].begin(); it < m_class_method[class_name].end(); ++it) {
        if ((*it)->get_name() == method_name)
            return (*it);
    }
    return nullptr;
}

int hyn::reflect::ClassFactory::get_class_method_count(const std::string &class_name) {
    return m_class_method[class_name].size();
}


/*                                class:Object                                                          */
int hyn::reflect::Object::get_field_count() {
    return sigleton::Sigleton<ClassFactory>::get_instance()->get_field_cont(m_class_name);
}

hyn::reflect::ClassField *hyn::reflect::Object::get_field(int pos) {
    return sigleton::Sigleton<ClassFactory>::get_instance()->get_field(m_class_name, pos);
}

hyn::reflect::ClassField *hyn::reflect::Object::get_field(const std::string &field_name) {
    return sigleton::Sigleton<ClassFactory>::get_instance()->get_field(m_class_name, field_name);
}

const std::string &hyn::reflect::Object::get_m_class_name() const {
    return m_class_name;
}

void hyn::reflect::Object::set_m_class_name(const std::string &class_name) {
    this->m_class_name = class_name;
}

void hyn::reflect::Object::call(const std::string &method_name) {
    ClassMethod *method = sigleton::Sigleton<ClassFactory>::get_instance()->get_method(m_class_name, method_name);
    auto func = method->get_method();
    typedef std::function<void(decltype(this))> class_method;
    (*((class_method *) func))(this);

}
