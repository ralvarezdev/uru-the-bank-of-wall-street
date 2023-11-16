add_library(datatables STATIC
output.h
output.cpp)

target_include_directories(datatables PRIVATE ${CMAKE_CURRENT_LIST_DIR})