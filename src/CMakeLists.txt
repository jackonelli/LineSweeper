add_library(Graph Graph.cpp tools/fileIO.cpp)
add_library(AntSystem AntSystem.cpp)
add_executable(linesweeper LineSweeper.cpp)
target_link_libraries(linesweeper AntSystem Graph)
install(TARGETS linesweeper DESTINATION ${CMAKE_BINARY_DIR})

add_custom_target(wasm
  # I think it is possible to run emsdk_env instead of all settings
  COMMAND export
    PATH=$PATH:/home/jakob/dev/emsdk
    PATH=$PATH:/home/jakob/dev/emsdk/clang/e1.38.12_64bit
    PATH=$PATH:/home/jakob/dev/emsdk/node/8.9.1_64bit/bin
    PATH=$PATH:/home/jakob/dev/emsdk/emscripten/1.38.12
    EMSDK=/home/jakob/dev/emsdk
    EM_CONFIG=/home/jakob/.emscripten
    LLVM_ROOT=/home/jakob/dev/emsdk/clang/e1.38.12_64bit
    EMSCRIPTEN_NATIVE_OPTIMIZER=/home/jakob/dev/emsdk/clang/e1.38.12_64bit/optimizer
    BINARYEN_ROOT=/home/jakob/dev/emsdk/clang/e1.38.12_64bit/binaryen
    EMSDK_NODE=/home/jakob/dev/emsdk/node/8.9.1_64bit/bin/node
    EMSCRIPTEN=/home/jakob/dev/emsdk/emscripten/1.38.12
  COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/web/script
  COMMAND em++ --bind -O3 ${CMAKE_SOURCE_DIR}/src/AntSystem.cpp ${CMAKE_SOURCE_DIR}/src/Graph.cpp -s WASM=1 -o ${CMAKE_SOURCE_DIR}/web/script/AntSystem.js
  SOURCES AntSystem.cpp Graph.cpp
  VERBATIM
  )

add_dependencies(wasm Graph AntSystem)
