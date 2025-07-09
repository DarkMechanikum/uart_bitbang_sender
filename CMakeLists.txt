cmake_minimum_required(VERSION 3.10)
project(uart_bitbang_sender)

set(CMAKE_CXX_STANDARD 23)

# Find libgpiod using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiod)

# Create the uartbitbangsender shared library
add_library(uartbitbangsender SHARED
    src/uart_bitbang_sender.cpp
)

target_include_directories(uartbitbangsender
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${GPIOD_INCLUDE_DIRS}
)

target_link_libraries(uartbitbangsender
    PUBLIC PkgConfig::GPIOD
)

# (Optional) Add an executable for testing or demonstration
add_executable(uart_bitbang_sender_demo
    src/main.cpp
)

target_include_directories(uart_bitbang_sender_demo
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(uart_bitbang_sender_demo
    PRIVATE uartbitbangsender
)
