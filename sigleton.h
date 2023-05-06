//
// Created by hyn on 2023/2/5.
//

#ifndef CPP_REFLECT_SIGLETON_H
#define CPP_REFLECT_SIGLETON_H
namespace hyn :: sigleton {

    template<typename T>
    class Sigleton {

    private:
        Sigleton() = default;
        ~Sigleton() = default;

    public:

        static T* get_instance() {
            if (m_instance == nullptr) {
                m_instance = new T();
                return m_instance;
            }
            return m_instance;
        }

        Sigleton(T&&) = delete;
        Sigleton(const T&) = delete;
        void operator = (const T&) = delete;

    private:
        static T* m_instance;
    };

    template<typename T>
    T* Sigleton<T>::m_instance = nullptr;
}
#endif //CPP_REFLECT_SIGLETON_H
