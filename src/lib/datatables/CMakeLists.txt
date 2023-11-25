add_library(datatables STATIC
output.h
output.cpp)

target_link_libraries(clients
  PRIVATE terminal)

target_include_directories(datatables PRIVATE ${CMAKE_CURRENT_LIST_DIR})