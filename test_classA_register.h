//
// Created by hyn on 2023/2/5.
//

#ifndef CPP_REFLECT_TEST_CLASSA_REGISTER_H
#define CPP_REFLECT_TEST_CLASSA_REGISTER_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "ClassRegister.h"

using namespace std;

class A : public hyn::reflect::Object {
public:
    A() : name("A") {}

    void show() override {
        cout << name;
    }

    void f() {
        cout << "func" << endl;
    }

    void f2() {
        cout << "f2" << endl;
    }

public:
    string name;
    int m_age;
    vector<int> m_vec;
};

REGISTER_CLASS(A);

REGISTER_CLASS_FIELD(A, name, string);

REGISTER_CLASS_FIELD(A, m_age, int);

REGISTER_CLASS_FIELD(A, m_vec, vector<int>);

REGISTER_CLASS_METHOD(A, f);

REGISTER_CLASS_METHOD(A, f2);

void test_classA_register() {
    hyn::reflect::ClassFactory *factory = hyn::sigleton::Sigleton<hyn::reflect::ClassFactory>::get_instance();
    hyn::reflect::Object *a = factory->create_class("A");
    a->show();
    cout << endl;

    vector<int> v = {1, 2, 3, 4};

    a->set("m_vec", v);
    vector<int> res;
    a->get("m_vec", res);
    for_each(res.begin(), res.end(), [](const auto &a) { cout << a; });

    cout << endl;

    int a_count = a->get_field_count();
    for (int i = 0; i < a_count; ++i) {
        auto field = a->get_field(i);
        cout << field->get_name() << "," << field->get_type() << "," << field->get_offset() << endl;
    }

    a->call("f");
    cout << endl;
    a->call("f2");
}

#endif //CPP_REFLECT_TEST_CLASSA_REGISTER_H
