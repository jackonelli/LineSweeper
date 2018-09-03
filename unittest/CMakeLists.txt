find_package (Boost COMPONENTS unit_test_framework REQUIRED)

enable_testing()
include_directories(${CMAKE_SOURCE_DIR}/src)
# Weird error if name is "unittest"
add_executable(unit_test TestMain.cpp AntSystem_test.cpp Graph_test.cpp)
target_link_libraries (unit_test
                       AntSystem
                       Graph
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                       )
add_test (NAME UnitTest COMMAND unit_test)

#install(TARGETS unit_test DESTINATION ${CMAKE_BINARY_DIR})
