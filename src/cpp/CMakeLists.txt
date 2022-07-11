project(cpp)

add_executable(${PROJECT_NAME}
    mem.c
    nhash.c
    cpp.c
    lexer.c
    assert.c
    macro.c
    eval.c
)

target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-DSTAND_ALONE" "-DUCPP_CONFIG" "-Wextra" "-g" "-DTRACE")
