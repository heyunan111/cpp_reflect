//
// Created by hyn on 2023/2/8.
//

#ifndef CPP_REFLECT_CLASSFIELD_H
#define CPP_REFLECT_CLASSFIELD_H

#include <utility>
#include <cstdlib>
#include <string>

namespace hyn::reflect {

    class ClassField {
    private:
        std::string m_name;
        std::string m_type;
        size_t m_offset;
    public:
        ClassField() : m_offset(0) {}

        ClassField(std::string name, std::string type, size_t offset) : m_name(std::move(name)),
                                                                        m_type(std::move(type)), m_offset(offset) {}

        std::string &get_name() {
            return m_name;
        }

        const std::string &get_type() {
            return m_type;
        }

        [[nodiscard]] size_t get_offset() const {
            return m_offset;
        }


    };

}

#endif //CPP_REFLECT_CLASSFIELD_H
