cmake_minimum_required (VERSION 2.6)
project(yamldom)
enable_language(C)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(LibYAML REQUIRED)

set(YD_FILES
src/gen.c
src/dump.c
src/clean.c
)

include_directories(${LIBYAML_INCLUDE_DIR})

add_library(yamldom STATIC ${YD_FILES})

set(EX_SC_FILES
example/scratch.c
example/setup.c
example/common.c
)
set(EX_ED_FILES
example/edit.c
example/setup.c
example/common.c
)

include_directories(src)

add_executable(example-scratch ${EX_SC_FILES})
target_link_libraries(example-scratch yamldom ${LIBYAML_LIBRARIES})

add_executable(example-edit ${EX_ED_FILES})
target_link_libraries(example-edit yamldom ${LIBYAML_LIBRARIES})
