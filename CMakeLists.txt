cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -Os -flto")
set(CMAKE_CXX_FLAGS_DEBUG "-Os")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-DNDEBUG -Os -flto")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -flto")
# Assembler complains when -DNDEBUG is here
set(CMAKE_ASM_FLAGS_RELEASE "-Os -flto")
set(CMAKE_ASM_FLAGS_DEBUG "-Os")
set(CMAKE_ASM_FLAGS_MINSIZEREL "-Os -flto")
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO "-Os -flto")

set(LLVM_MOS_PLATFORM nes-nrom)
find_package(llvm-mos-sdk REQUIRED)

project(mesen-integration CXX ASM)
set(version 0.1.0)
set(namespace scarduadev)

add_compile_options(
  -g -gdwarf-4
  -std=c++23
  -Wall -Wextra -Wconversion -Wsign-conversion -Wno-unused-command-line-argument
)

add_link_options(
  -g -gdwarf-4

  # make functions non-recursive by default
  -fnonreentrant

  # -Map generates a file with information about how the final binary's memory
  # is laid out. This will eventually be used to show the remaining space in
  # CHR-ROM, PRG-ROM, PRG-RAM, System RAM, and ZEROPAGE

  # --lto-whole-program-visibility helps ensure clang does as many
  # optimizations as possible
  -Wl,-Map,${CMAKE_PROJECT_NAME}.map,--lto-whole-program-visibility
)

add_library(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME} 
  PRIVATE
    src/mesen-integration.cpp
)

set(public_headers
    src/mesen-integration.hpp
)

# note that ${public_headers} has to be in quotes
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")

if(DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX is not set\n"
        "Default value: ${CMAKE_INSTALL_PREFIX}\n"
        "Will set it to ${CMAKE_SOURCE_DIR}/install"
    )
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_SOURCE_DIR}/install"
        CACHE PATH "Where the library will be installed to" FORCE
    )
else()
    message(
        STATUS
        "CMAKE_INSTALL_PREFIX was already set\n"
        "Current value: ${CMAKE_INSTALL_PREFIX}"
    )
endif()

include(GNUInstallDirs)

install(TARGETS ${CMAKE_PROJECT_NAME}
    EXPORT "${CMAKE_PROJECT_NAME}Targets"
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME} # include/SomeLibrary
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # include
)

install(EXPORT "${CMAKE_PROJECT_NAME}Targets"
    FILE "${CMAKE_PROJECT_NAME}Targets.cmake"
    NAMESPACE ${namespace}::
    DESTINATION cmake
)

include(CMakePackageConfigHelpers)

# generate the version file for the config file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${version}"
    COMPATIBILITY AnyNewerVersion
)
# create config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION cmake
)
# install config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION cmake
)

# install script
install(FILES
    scripts/mesen-integration.lua
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME}
)

# packaging

set(package_files src/ CMakeLists.txt LICENSE)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-src.zip
    COMMAND ${CMAKE_COMMAND} -E tar c ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-src.zip --format=zip -- ${package_files}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${package_files})
add_custom_target(${CMAKE_PROJECT_NAME}-package DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}-src.zip)