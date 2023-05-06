cmake_minimum_required(VERSION 3.24)
project(cpp_reflect)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp_reflect main.cpp ClassFactory.cpp ClassFactory.h sigleton.h ClassRegister.h test_classA_register.h ClassField.h ClassMethod.h)
