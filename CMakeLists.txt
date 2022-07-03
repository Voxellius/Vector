cmake_minimum_required(VERSION 3.16)

project(Vector)
enable_language(ASM)

function(embed_resource target section file)
    get_filename_component(asm_file "${file}" NAME)
    set(asm_file "${CMAKE_CURRENT_BINARY_DIR}/${target}-${section}.s")
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

compile_gml(src/ExampleWindow.gml ExampleWindowGML.h example_window_gml)
compile_gml(src/LoginDialog.gml LoginDialogGML.h login_dialog_gml)

set(SOURCES
    Vector-serenity_icon_s.s
    Vector-serenity_icon_m.s
    src/LoginDialog.h
    src/LoginDialog.cpp
    src/main.cpp
    ExampleWindowGML.h
    LoginDialogGML.h
)

add_executable(Vector ${SOURCES})
add_dependencies(Vector add_section_serenity_icon_s add_section_serenity_icon_m)
target_link_libraries(Vector gui core gcc_s gcc_eh)

embed_resource(Vector serenity_icon_s "${CMAKE_SOURCE_DIR}/icon_s.png")
embed_resource(Vector serenity_icon_m "${CMAKE_SOURCE_DIR}/icon_m.png")

install(TARGETS Vector RUNTIME DESTINATION bin)
