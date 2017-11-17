cmake_minimum_required(VERSION 3.7)
project(esp-ulp-adc-bug)

set(IDF_PATH $ENV{IDF_PATH})

set(CMAKE_CXX_STANDARD 11)

add_definitions(-DWITH_POSIX)

# enable CLion to find the esp-idf files
set(COMPONENTS
        app_update
        aws_iot
        aws_iot/aws-iot-device-sdk-embedded-C
        bootloader
        bootloader_support
        bt
        cxx
        driver
        esp32
        esptool_py
        ethernet
        expat
        fatfs
        freertos
        idf_test
        json
        log
        mbedtls
        micro-ecc
        newlib
        nghttp
        nvs_flash
        openssl
        partition_table
        sdmmc
        spi_flash
        soc/esp32
        tcpip_adapter
        ulp
        vfs
        wpa_supplicant
        xtensa-debug-module)

set(INCLUDE_DIRS "")
foreach (component ${COMPONENTS})
    list(APPEND INCLUDE_DIRS ${IDF_PATH}/components/${component}/include)
endforeach ()
list(APPEND INCLUDE_DIRS ${IDF_PATH}/components/lwip/include/lwip)
list(APPEND INCLUDE_DIRS ${IDF_PATH}/components/lwip/include/lwip/port)
list(APPEND INCLUDE_DIRS ${IDF_PATH}/components/lwip/include/lwip/posix)
list(APPEND INCLUDE_DIRS ${IDF_PATH}/components/coap/port/include/coap)
list(APPEND INCLUDE_DIRS ${IDF_PATH}/components/coap/libcoap/include/coap)
list(REMOVE_DUPLICATES INCLUDE_DIRS)
include_directories(${INCLUDE_DIRS})
include_directories("build/include")
include_directories("build/main")

# add source files
file(
        GLOB SOURCE_FILES
        "main/*.h"
        "main/*.c"
        "main/*.hpp"
        "main/*.cpp"
)
add_executable(esp-ulp-adc-bug ${SOURCE_FILES})

# add build targets for esp-idf makefile
add_custom_target(
        make
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(
        make-flash
        COMMAND make flash
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(
        make-clean
        COMMAND make clean
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
