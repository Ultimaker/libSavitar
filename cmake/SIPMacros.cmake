# Macros for SIP
# ~~~~~~~~~~~~~~

set(SIP_ARGS --pep484-pyi --no-protected-is-public)

function(add_sip_module MODULE_TARGET)
    if(NOT SIP_BUILD_EXECUTABLE)
        set(SIP_BUILD_EXECUTABLE ${CMAKE_PREFIX_PATH}/Scripts/sip-build)
    endif()

    message(STATUS "SIP: Generating pyproject.toml")
    configure_file(${CMAKE_SOURCE_DIR}/pyproject.toml.in ${CMAKE_CURRENT_BINARY_DIR}/pyproject.toml)
    configure_file(${CMAKE_SOURCE_DIR}/cmake/CMakeBuilder.py ${CMAKE_CURRENT_BINARY_DIR}/CMakeBuilder.py)
    if(WIN32)
        set(ext .pyd)
        set(env_path_sep ";")
    else()
        set(ext .so)
        set(env_path_sep ":")
    endif()

    message(STATUS "SIP: Generating source files")
    execute_process(
            COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${PYTHONPATH}${env_path_sep}$ENV{PYTHONPATH}${env_path_sep}${CMAKE_CURRENT_BINARY_DIR}" ${SIP_BUILD_EXECUTABLE} ${SIP_ARGS}
            COMMAND_ECHO STDOUT
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
    )
    # This will generate the source-files during the configuration step in CMake. Needed to obtain the sources

    # Touch the generated files (8 in total) to make them dirty and force them to rebuild
    message(STATUS "SIP: Touching the source files")
    set(_sip_output_files)
    list(LENGTH SIP_FILES _no_outputfiles)
    foreach(_concat_file_nr RANGE 0 ${_no_outputfiles})
        if(${_concat_file_nr} LESS 8)
            list(APPEND _sip_output_files "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_TARGET}/${MODULE_TARGET}/sip${MODULE_TARGET}part${_concat_file_nr}.cpp")
        endif()
    endforeach()

    # Find the generated source files
    message(STATUS "SIP: Collecting the generated source files")
    file(GLOB sip_c "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_TARGET}/${MODULE_TARGET}/*.c")
    file(GLOB sip_cpp "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_TARGET}/${MODULE_TARGET}/*.cpp")
    file(GLOB sip_hdr "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_TARGET}/${MODULE_TARGET}/*.h")

    # Add the user specified source files
    message(STATUS "SIP: Collecting the user specified source files")
    get_target_property(usr_src ${MODULE_TARGET} SOURCES)

    # create the target library and link all the files (generated and user specified
    message(STATUS "SIP: Linking the interface target against the shared library")
    set(sip_sources "${sip_c}" "${sip_cpp}")
    if(${usr_src})
        list(APPEND sip_sources "${usr_src}")
    endif()

    if (BUILD_SHARED_LIBS)
        add_library("sip_${MODULE_TARGET}" SHARED ${sip_sources})
    else()
        add_library("sip_${MODULE_TARGET}" STATIC ${sip_sources})
    endif()

    # Make sure that the library name of the target is the same as the MODULE_TARGET with the appropriate extension
    target_link_libraries("sip_${MODULE_TARGET}" PRIVATE "${MODULE_TARGET}")
    set_target_properties("sip_${MODULE_TARGET}" PROPERTIES PREFIX "")
    set_target_properties("sip_${MODULE_TARGET}" PROPERTIES SUFFIX ${ext})
    set_target_properties("sip_${MODULE_TARGET}" PROPERTIES OUTPUT_NAME "${MODULE_TARGET}")

    # Make sure all rpaths are set from the INTERFACE target
    get_target_property(_SKIP_BUILD_RPATH ${MODULE_TARGET} SKIP_BUILD_RPATH)
    set_target_properties("sip_${MODULE_TARGET}" PROPERTIES SKIP_BUILD_RPATH "${_SKIP_BUILD_RPATH}")
    get_target_property(_BUILD_WITH_INSTALL_RPATH ${MODULE_TARGET} BUILD_WITH_INSTALL_RPATH)
    set_target_properties("sip_${MODULE_TARGET}" PROPERTIES BUILD_WITH_INSTALL_RPATH "${_BUILD_WITH_INSTALL_RPATH}")
    get_target_property(_INSTALL_RPATH_USE_LINK_PATH ${MODULE_TARGET} INSTALL_RPATH_USE_LINK_PATH)
    set_target_properties("sip_${MODULE_TARGET}" PROPERTIES INSTALL_RPATH_USE_LINK_PATH "${_INSTALL_RPATH_USE_LINK_PATH}")
    get_target_property(_MACOSX_RPATH ${MODULE_TARGET} MACOSX_RPATH)
    set_target_properties("sip_${MODULE_TARGET}" PROPERTIES MACOSX_RPATH "${_MACOSX_RPATH}")
    get_target_property(_INSTALL_RPATH ${MODULE_TARGET} INSTALL_RPATH)
    set_target_properties("sip_${MODULE_TARGET}" PROPERTIES INSTALL_RPATH "${_INSTALL_RPATH}")

    # Add the custom command to (re-)generate the files and mark them as dirty. This allows the user to actually work
    # on the sip definition files without having to reconfigure the complete project.
    add_custom_command(
            TARGET "sip_${MODULE_TARGET}"
            COMMAND ${CMAKE_COMMAND} -E env "PYTHONPATH=${PYTHONPATH}${env_path_sep}$ENV{PYTHONPATH}${env_path_sep}${CMAKE_CURRENT_BINARY_DIR}" ${SIP_BUILD_EXECUTABLE} ${SIP_ARGS}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
            MAIN_DEPENDENCY ${MODULE_SIP}
            DEPENDS ${sip_sources}
            VERBATIM
    )

    set_target_properties("sip_${MODULE_TARGET}"
            PROPERTIES
            RESOURCE "${CMAKE_CURRENT_BINARY_DIR}/${MODULE_TARGET}/${MODULE_TARGET}/${MODULE_TARGET}.pyi")
endfunction()

function(install_sip_module MODULE_TARGET)
    if(DEFINED ARGV1)
        set(_install_path ${ARGV1})
    else()
        if(DEFINED Python_SITEARCH)
            set(_install_path ${Python_SITEARCH})
        elseif(DEFINED Python_SITELIB)
            set(_install_path ${Python_SITELIB})
        else()
            message(FATAL_ERROR "SIP: Specify the site-packages location")
        endif()
    endif()
    message(STATUS "SIP: Installing Python module and PEP 484 file in ${_install_path}")
    install(TARGETS "sip_${MODULE_TARGET}"
            ARCHIVE DESTINATION ${_install_path}
            LIBRARY DESTINATION ${_install_path}
            RUNTIME DESTINATION ${_install_path}
            RESOURCE DESTINATION ${_install_path}
            )
endfunction()