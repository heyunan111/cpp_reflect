//
// Created by hyn on 2023/2/8.
//
#include <string>
#include <utility>

namespace hyn::reflect {
    class ClassMethod {
    public:
        ClassMethod() = default;

        ClassMethod(std::string name, uintptr_t method) : m_name(std::move(name)), m_method(method) {}

        ~ClassMethod() = default;

        std::string get_name() {
            return m_name;
        }

        [[nodiscard]] uintptr_t get_method() const {
            return m_method;
        }

    private:
        std::string m_name;
        uintptr_t m_method{};
    };

}