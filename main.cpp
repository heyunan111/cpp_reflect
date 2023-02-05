#include <iostream>
#include "ClassRegister.h"

using namespace std;

class A : public hyn::reflect::Object{
public:
    A():name("A"){}

    void show() {
        cout<<name;
    }

private:
    string name;
};

REGISTER_CLASS(A);

void test_class_register() {
    hyn::reflect::ClassFactory* factory = hyn::sigleton::Sigleton<hyn::reflect::ClassFactory>::get_instance();
    hyn::reflect::Object* a = factory->create_class("A");
    a->show();
}

int main() {
    test_class_register();
}
