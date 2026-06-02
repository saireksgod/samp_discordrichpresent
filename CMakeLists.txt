cmake_minimum_required(VERSION 3.15)
project(samp_discord_rpc CXX)

set(CMAKE_CONFIGURATION_TYPES "Release;MinSizeRel" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_GENERATOR MATCHES "Visual Studio")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "SAMP требует 32-битную сборку (x86). Пожалуйста, настройте генератор на Win32/x86.")
endif()

file(GLOB SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h"
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES 
    PREFIX ""
    SUFFIX ".asi"
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/discord-rpc/discord-rpc.lib
        Ws2_32
        Wininet
)

target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS WIN32_LEAN_AND_MEAN)