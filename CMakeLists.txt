cmake_minimum_required(VERSION 3.15)
project(Calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Сборка всех четырех плагинов
add_library(sin_plugin SHARED sin_plug.cpp)
add_library(ln_plugin SHARED ln_plug.cpp)
add_library(cos_plugin SHARED cos_plug.cpp)
add_library(pow_plugin SHARED pow_plug.cpp)

# Сборка главного калькулятора
add_executable(calc main.cpp)

if(UNIX)
    target_link_libraries(calc PRIVATE dl)
endif()

# Настройка папок для вывода (чтобы всё собиралось в bin и bin/plugins)
set(OUTPUT_BIN_DIR "${CMAKE_BINARY_DIR}/bin")
set(OUTPUT_PLUGIN_DIR "${OUTPUT_BIN_DIR}/plugins")

set_target_properties(calc PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_BIN_DIR}"
)

# Применяем настройки вывода ко всем плагинам
set_target_properties(sin_plugin ln_plugin cos_plugin pow_plugin PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_PLUGIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_PLUGIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_PLUGIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_PLUGIN_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_PLUGIN_DIR}"
)