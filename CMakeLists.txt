cmake_minimum_required(VERSION 3.16)

project(Vector)
enable_language(ASM)

function(embed_resource target section file)
    get_filename_component(asm_file "${file}" NAME)
    set(asm_file "${CMAKE_CURRENT_BINARY_DIR}/build/${target}-${section}.s")
    get_filename_component(input_file "${file}" ABSOLUTE)
    add_custom_command(
        OUTPUT "${asm_file}"
        COMMAND "${CMAKE_SOURCE_DIR}/embed.sh" "${asm_file}" "${section}" "${input_file}"
        DEPENDS "${input_file}" "${CMAKE_SOURCE_DIR}/embed.sh"
        COMMENT "Generating ${asm_file}"
    )
    target_sources("${target}" PRIVATE "${asm_file}")
    add_custom_target("add_section_${section}" DEPENDS ${asm_file})
    set_property(SOURCE ${asm_file} PROPERTY LANGUAGE ASM)
endfunction()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SerenityOS_SOURCE_DIR /home/anon/Code/serenity)
include(${SerenityOS_SOURCE_DIR}/Meta/CMake/code_generators.cmake)
add_custom_target(all_generated)
include_directories(/usr/include)

compile_gml(src/LoginDialog.gml build/LoginDialogGML.h login_dialog_gml)
compile_gml(src/HubWindow.gml build/HubWindowGML.h hub_window_gml)

set(SOURCES
    build/Vector-serenity_icon_s.s
    build/Vector-serenity_icon_m.s
    src/Matrix.h
    src/Matrix.cpp
    src/LoginDialog.h
    src/LoginDialog.cpp
    src/HubWindow.h
    src/HubWindow.cpp
    src/main.cpp
    build/LoginDialogGML.h
    build/HubWindowGML.h
)

add_executable(Vector ${SOURCES})
add_dependencies(Vector add_section_serenity_icon_s add_section_serenity_icon_m)
add_library(crypto SHARED IMPORTED)
set_property(TARGET crypto PROPERTY IMPORTED_LOCATION "/usr/lib/libcrypto.so")
target_link_libraries(Vector gui gfx core crypto tls http gcc_s gcc_eh)

embed_resource(Vector serenity_icon_s "${CMAKE_SOURCE_DIR}/icon_s.png")
embed_resource(Vector serenity_icon_m "${CMAKE_SOURCE_DIR}/icon_m.png")

install(TARGETS Vector RUNTIME DESTINATION bin)
