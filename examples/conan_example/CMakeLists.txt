cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)
find_package(camera_manager CONFIG REQUIRED)
add_executable(example src/example.cpp)
target_include_directories(example PUBLIC ${camera_manager_INCLUDE_DIRS})
target_link_libraries(example PRIVATE ${camera_manager_LIBRARIES})
