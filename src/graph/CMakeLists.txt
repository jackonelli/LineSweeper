add_library(Graph Graph.cpp ${CMAKE_SOURCE_DIR}/src/tools/fileIO.cpp)
#target_include_directories(Graph PRIVATE ${CMAKE_SOURCE_DIR}/src/tools/)
target_compile_options(Graph PRIVATE
                       -Wall
                       -Wextra
                       -Wundef
                       -Wfloat-equal
                       -O1
                       )
