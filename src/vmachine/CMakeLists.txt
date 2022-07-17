
project(vmachine)

add_executable(${PROJECT_NAME}
    vmachine.c
    do_arith.c
    do_compare.c
    do_branch.c
    do_stack.c
    do_load.c
    do_abort.c
    do_trap.c
    runloop.c
    vm_errors.c
    val_stack.c
    runtime.c
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
        ${PROJECT_SOURCE_DIR}/../runtime
        ${PROJECT_SOURCE_DIR}/../../external/include
)

target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-Wextra" "-g" )
