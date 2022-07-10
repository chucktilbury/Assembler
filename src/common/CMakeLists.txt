
project(common)

add_library(${PROJECT_NAME} STATIC
    opcodes.c
    registers.c
    cmdline.c
    inst_buf.c
    str_table.c
    val_table.c
    str_utils.c
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../include
        ${PROJECT_SOURCE_DIR}/../common
)

target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra" "-DMEMORY_DEBUG" "-g" "-DTRACE")
