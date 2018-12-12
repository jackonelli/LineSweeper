add_library(AntSystem AntSystem.cpp)
target_link_libraries(AntSystem Graph)
target_compile_options(AntSystem PRIVATE
                       -Wall
                       -Wextra
                       -Wundef
                       -Wfloat-equal
                       -O1
                       )
