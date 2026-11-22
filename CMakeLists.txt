cmake_minimum_required(VERSION 3.16)

project(pushk2pre3rdrop VERSION 0.1.1 LANGUAGES CXX)


# Опции компиляции
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app_version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/app_version.hpp
    @ONLY
)

# Источники
file(GLOB_RECURSE source_list
    src/*
    src/mods/*
    src/mods/misc/*
)

# Исполняемый файл
add_executable(${PROJECT_NAME} ${source_list}
    src/funcsplit.cpp
    src/funcmerge.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/mods
    ${CMAKE_SOURCE_DIR}/src/mods/misc
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    APP_NAME_STR="${PROJECT_NAME}"
)

# Установка бинарника
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# python3 -m unittest discover -s tests

# cp pushk2pre3rdrop ~/bin