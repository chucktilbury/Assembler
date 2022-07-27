
project(asm)

add_executable(${PROJECT_NAME}
    asm.c
)

target_link_libraries(${PROJECT_NAME}
    common
    gc
    cord
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../common
        ${PROJECT_SOURCE_DIR}/../include
        ${PROJECT_SOURCE_DIR}/../../external/include
)

link_directories (
    PUBLIC
        ${PROJECT_SOURCE_DIR}/bin
        ${PROJECT_SOURCE_DIR}/external/lib
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
