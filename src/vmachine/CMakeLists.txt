
project(vmachine)

add_executable(${PROJECT_NAME}
    vmachine.c
    #do_arith.c
    #do_compare.c
    #do_branch.c
    #do_stack.c
    #do_load.c
    #do_misc.c
    runloop.c
    vm_errors.c
    val_stack.c
)

target_link_libraries(${PROJECT_NAME}
    common
    runtime
    gc
    pthread
    m
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/../include
        ${PROJECT_SOURCE_DIR}/../common
        ${PROJECT_SOURCE_DIR}/../runtime
        ${PROJECT_SOURCE_DIR}/../gc/include
        ${PROJECT_SOURCE_DIR}/../gc/include/gc
)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    $<$<CONFIG:DEBUG>:-g3>
    $<$<CONFIG:DEBUG>:-Og>
    $<$<CONFIG:DEBUG>:-Wno-tautological-constant-compare>
    $<$<CONFIG:DEBUG>:-DMEMORY_DEBUG>
    $<$<CONFIG:DEBUG>:-DENA_TRACE>
    $<$<CONFIG:RELEASE>:-Ofast>
    $<$<CONFIG:PROFILE>:-pg -O0>
)
