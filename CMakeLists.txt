cmake_minimum_required(VERSION 3.25)
project(yamp_labs)

set(CMAKE_CXX_STANDARD 17)

add_executable(yamp_labs main/main.cpp hash_table/Hash_table.cpp hash_table/Hash_table.h Node/Node.cpp Node/Node.h Strurcture_node/Structure_node.cpp Strurcture_node/Structure_node.h main/main.h)
