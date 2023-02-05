//
// Created by hyn on 2023/2/5.
//

#include "ClassFactory.h"

void hyn::reflect::ClassFactory::register_class(const std::string &class_name, hyn::reflect::create_object method) {
    m_class_map[class_name] = method;
}

hyn::reflect::Object *hyn::reflect::ClassFactory::create_class(const std::string &class_name) {
    auto it = m_class_map.find(class_name);
    if (it == m_class_map.end())
        return nullptr;
    return it->second();
}
