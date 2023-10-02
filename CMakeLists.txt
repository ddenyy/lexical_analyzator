cmake_minimum_required(VERSION 3.25)
project(yamp_labs)

set(CMAKE_CXX_STANDARD 17)

add_executable(yamp_labs main/main.cpp hash_table/Hash_table.cpp hash_table/Hash_table.h Node/Node.cpp Node/Node.h Token/Token.cpp Token/Token.h main/main.h utils/Utils.h Test/Test.cpp Test/Test.h)
