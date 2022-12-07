cmake_minimum_required(VERSION 3.18)
project(compiler)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

BISON_TARGET(PARSER parser.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.c
    COMPILE_FLAGS "--report=lookahead -tvdo")

FLEX_TARGET(SCANNER scanner.l
    ${CMAKE_CURRENT_BINARY_DIR}/scanner.c
    COMPILE_FLAGS "-i" )

ADD_FLEX_BISON_DEPENDENCY(SCANNER PARSER)

#set(BISON_PARSER_COMPILE_FLAG "--report=lookahead -tv")

include_directories( ${CMAKE_CURRENT_BINARY_DIR} ./ )

add_executable(${PROJECT_NAME}
    ${BISON_PARSER_OUTPUTS}
    ${FLEX_SCANNER_OUTPUTS}
    errors.c
    main.c
    symtable.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../include
        ${PROJECT_SOURCE_DIR}/../common
)

target_link_libraries(${PROJECT_NAME}
    common
    m
    gc
    cord
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wno-sign-compare
    -Wno-unused-variable
    -Wno-parentheses-equality
    $<$<CONFIG:DEBUG>:-g3>
    $<$<CONFIG:DEBUG>:-Og>
    $<$<CONFIG:DEBUG>:-DMEMORY_DEBUG>
    $<$<CONFIG:DEBUG>:-DENA_TRACE>
    $<$<CONFIG:RELEASE>:-Ofast>
    $<$<CONFIG:PROFILE>:-pg -O0>
)

set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/parser.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/parser.output"
)
