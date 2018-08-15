add_executable(linesweeper LineSweeper.cpp AntSystem.cpp Graph.cpp)
install(TARGETS linesweeper DESTINATION build)
add_compile_options(-std=c++11)

