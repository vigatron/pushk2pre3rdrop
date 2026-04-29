cmake_minimum_required(VERSION 3.16)

project(pushk2pre3rdrop LANGUAGES CXX)


include_directories(${CMAKE_SOURCE_DIR}/src
                    ${CMAKE_SOURCE_DIR}/src/mods
                    ${CMAKE_SOURCE_DIR}/src/mods/misc)


# Опции компиляции
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    APP_NAME_STR="${PROJECT_NAME}"
    APP_VERSION=1
)

# Установка бинарника
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# python3 -m unittest discover -s tests

# cp pushk2pre3rdrop ~/bin