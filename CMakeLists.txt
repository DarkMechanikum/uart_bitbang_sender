cmake_minimum_required(VERSION 3.10)
project(uart_bitbang_sender)

set(CMAKE_CXX_STANDARD 23)

# Find libgpiod using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED IMPORTED_TARGET libgpiod)

# Add the executable from the src/ directory
add_executable(uart_bitbang_sender
    src/main.cpp
    src/uart_bitbang_sender.cpp
)

# Include headers from src/
target_include_directories(uart_bitbang_sender PRIVATE src ${GPIOD_INCLUDE_DIRS})

# Link with libgpiod
target_link_libraries(uart_bitbang_sender PRIVATE PkgConfig::GPIOD)
