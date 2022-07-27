cmake_minimum_required(VERSION 3.18)
project(gasm)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# include_directories(${PROJECT_SOURCE_DIR}/../include)

# set(LEX_FILE      ${CMAKE_CURRENT_SOURCE_DIR}/scanner.l)
# set(LEX_C_SOURCE  ${CMAKE_CURRENT_SOURCE_DIR}/scanner.c)
# set(LEX_H         ${PROJECT_INCLUDE_DIRECTORY}/scanner.h)
# set(YACC_FILE     ${CMAKE_CURRENT_SOURCE_DIR}/parser.y)
# set(YACC_C_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/parser.c)
# set(YACC_H        ${PROJECT_INCLUDE_DIRECTORY}/parser.h)

# add_custom_target(unionLexHeader DEPENDS ${LEX_H})
# add_custom_command(OUTPUT ${LEX_C_SOURCE}
#         DEPENDS ${LEX_FILE}
#         PRE_BUILD
#         COMMAND flex -i -o scanner.c scanner.l
#         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# add_custom_target(unionYaccHeader DEPENDS ${YACC_H} ${YACC_C_SOURCE} parser.output)
# add_custom_command(OUTPUT ${YACC_C_SOURCE} ${YACC_H}
#         DEPENDS ${YACC_FILE}
#         PRE_BUILD
#         #COMMAND bison --report=lookahead -Wcounterexamples -tvdo parser.c parser.y
#         COMMAND bison --report=lookahead -tvdo parser.c parser.y
#         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

BISON_TARGET(PARSER parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c COMPILE_FLAGS "--report=lookahead -tvdo")
FLEX_TARGET(SCANNER scanner.l  ${CMAKE_CURRENT_BINARY_DIR}/scanner.c COMPILE_FLAGS "-i" )
ADD_FLEX_BISON_DEPENDENCY(SCANNER PARSER)

set(BISON_PARSER_COMPILE_FLAG "--report=lookahead -tv")

include_directories( ${CMAKE_CURRENT_BINARY_DIR} ./ )

add_executable(${PROJECT_NAME}
    ${BISON_PARSER_OUTPUTS}
    ${FLEX_SCANNER_OUTPUTS}
    post.c
    ast.c
    arith.c
    asm_errors.c
    gas.c
)

# add_executable(${PROJECT_NAME}
#     parser.c
#     scanner.c
#     post.c
#     ast.c
#     arith.c
#     asm_errors.c
#     gas.c
# )

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../include
        ${PROJECT_SOURCE_DIR}/../common
        ${PROJECT_SOURCE_DIR}/../../external/include
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
