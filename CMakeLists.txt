cmake_minimum_required(VERSION 3.10)

project(camera_manager VERSION 1.0 LANGUAGES CXX)

option(CAMERA_MANAGER_SKIP_CONFIG "Skip installing CMake package config files" OFF)


find_package(OpenCV CONFIG REQUIRED)

add_library(${CMAKE_PROJECT_NAME}  STATIC src/CameraManager.cpp)

# Add include paths
target_include_directories( ${CMAKE_PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE ${OpenCV_LIBS}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

# Install the library binary
install(TARGETS ${CMAKE_PROJECT_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)  
# Install the public headers
install(DIRECTORY ./include/ DESTINATION include)

if(NOT CAMERA_MANAGER_SKIP_CONFIG)
    install(EXPORT ${CMAKE_PROJECT_NAME}Targets
        FILE ${CMAKE_PROJECT_NAME}Targets.cmake
        NAMESPACE ${CMAKE_PROJECT_NAME}::
        DESTINATION lib/cmake
    )
    include(CMakePackageConfigHelpers)

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${CMAKE_PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION lib/cmake
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION lib/cmake
    )
endif()
