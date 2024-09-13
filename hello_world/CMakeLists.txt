SET(RESOURCE_COMPILER xxd)

set(COMPILED_RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/rc/index.html.gz")

FOREACH(INPUT_FILE ${COMPILED_RESOURCES})
    SET(OUTPUT_FILE ${INPUT_FILE}.hex.h)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${OUTPUT_FILE}
        COMMAND ${RESOURCE_COMPILER} -i ${INPUT_FILE} ${OUTPUT_FILE}
        COMMENT "Compiling ${INPUT_FILE} to binary"
        DEPENDS ${INPUT_FILE})
    LIST(APPEND COMPILED_RESOURCES ${OUTPUT_FILE})
ENDFOREACH()

add_executable(hello_world_https
  main.cpp
  request_handler.cpp
  ${COMPILED_RESOURCES}
  )

# Stack size is excessively large as this is a simple demo, should be reduced
target_compile_definitions(hello_world_https PRIVATE
  PICO_STDIO_DEFAULT_CRLF=1
  PICO_STDIO_ENABLE_CRLF_SUPPORT=1
  PICO_STACK_SIZE=0x8000 # 32K
  PICO_STDIO_STACK_BUFFER_SIZE=512
  )

target_include_directories(hello_world_https PRIVATE ${CMAKE_CURRENT_LIST_DIR})
      
target_link_libraries(hello_world_https
        pico_cyw43_arch_lwip_poll
        pico_lwip_mbedtls
        pico_mbedtls
        pico_stdlib
        pico_http
        pico_tls
        pico_logger
        )
pico_add_extra_outputs(hello_world_https)

pico_enable_stdio_usb(hello_world_https 1)
pico_enable_stdio_uart(hello_world_https 0)

target_include_directories(hello_world_https PUBLIC config)
