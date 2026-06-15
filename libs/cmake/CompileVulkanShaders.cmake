find_package(Vulkan REQUIRED)

if(NOT Vulkan_glslangValidator_FOUND)
    message(FATAL_ERROR "failed to find glslangValidator")
endif()

function(addCompileShadersCommand)
    cmake_parse_arguments(addCompileShadersCommand "" "TARGET_NAME;OUTPUT_DIR" "SHADERS;DEPENDS" ${ARGN})

    if(NOT addCompileShadersCommand_TARGET_NAME)
        message(FATAL_ERROR "provide unique target name")
    endif()

    if(NOT addCompileShadersCommand_SHADERS)
        message(FATAL_ERROR "provide at least one shader file name")
    endif()

    foreach(SHADER ${addCompileShadersCommand_SHADERS})
        if(NOT EXISTS ${SHADER})
            message(FATAL_ERROR "failed to find shader file ${SHADER}")
        endif()

        file(MAKE_DIRECTORY ${addCompileShadersCommand_OUTPUT_DIR})

        get_filename_component(FNC ${SHADER} NAME)

        add_custom_command(OUTPUT ${addCompileShadersCommand_OUTPUT_DIR}/${FNC}.spv
            COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -gVS --target-env vulkan1.3 -V ${SHADER} -o ${addCompileShadersCommand_OUTPUT_DIR}/${FNC}.spv
            DEPENDS ${SHADER}
            COMMENT "Compiling ${SHADER}"
        )

        cmrc_add_resources(${addCompileShadersCommand_TARGET_NAME} WHENCE ${addCompileShadersCommand_OUTPUT_DIR} ${addCompileShadersCommand_OUTPUT_DIR}/${FNC}.spv)
    endforeach()
endfunction()