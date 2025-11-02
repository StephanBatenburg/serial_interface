cmake_minimum_required(VERSION 3.10)

project(serial_interface)

find_package(Boost CONFIG REQUIRED COMPONENTS system)
find_package(ftxui CONFIG REQUIRED)

add_executable(serial_interface src/main.cpp)

target_link_libraries(serial_interface 
    PRIVATE Boost::system
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component)
target_include_directories(serial_interface PRIVATE "${CMAKE_SOURCE_DIR}\\build\\vcpkg_installed\\x64-windows\\bin")