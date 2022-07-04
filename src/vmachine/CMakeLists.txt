
project(vmachine)

add_executable(${PROJECT_NAME}
    vmachine.c
    arith.c
    compare.c
    runloop.c
    val_stack.c
)

target_link_libraries(${PROJECT_NAME}
    common
    gc
    cord
    m
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../include
        ${PROJECT_SOURCE_DIR}/../common
        ${PROJECT_SOURCE_DIR}/../../external/include
)

target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra" "-g" "-DTRACE")
