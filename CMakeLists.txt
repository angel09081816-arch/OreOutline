cmake_minimum_required(VERSION 3.22)

project(ore_outline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(MOD_ID "ore_outline" CACHE STRING "LeviLauncher package directory")
set(MOD_NAME "OreOutline" CACHE STRING "LeviLauncher display name")
set(MOD_AUTHOR "OreOutline contributors" CACHE STRING "Mod author")
set(MOD_VERSION "0.1.0" CACHE STRING "Mod version")
set(MOD_LIBRARY_NAME "ore_outline" CACHE STRING "Native library name")
set(MOD_MINECRAFT_VERSIONS "[\"1.26.50.4\"]" CACHE STRING "Minecraft versions declared in the manifest")
set(MOD_ICON "" CACHE STRING "Optional package-relative icon")

include(FetchContent)

FetchContent_Declare(
    preloader_android
    GIT_REPOSITORY https://github.com/LiteLDev/preloader-android.git
    GIT_TAG 0.2.2
    SOURCE_SUBDIR cmake/preloader-sdk
)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_Declare(
    boost_pfr
    GIT_REPOSITORY https://github.com/boostorg/pfr.git
    GIT_TAG boost-1.88.0
)
FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.7
)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.2.0
)
set(FMT_DOC OFF CACHE BOOL "Disable fmt docs" FORCE)
set(FMT_TEST OFF CACHE BOOL "Disable fmt tests" FORCE)
set(FMT_INSTALL OFF CACHE BOOL "Disable fmt install rules" FORCE)
FetchContent_MakeAvailable(preloader_android nlohmann_json boost_pfr magic_enum fmt)

FetchContent_GetProperties(preloader_android)
set(LEVI_PRELOADER_INCLUDE_DIR "${preloader_android_SOURCE_DIR}/include")

if(NOT ANDROID)
    set(LEVI_GENERATED_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated-config")

    add_executable(ore_config_generator
        src/config_generator.cpp
        src/mod/Config.cpp
    )
    target_include_directories(ore_config_generator PRIVATE src "${LEVI_PRELOADER_INCLUDE_DIR}")
    target_compile_definitions(ore_config_generator PRIVATE PL_CONFIG_NO_RUNTIME)
    target_link_libraries(ore_config_generator PRIVATE
        nlohmann_json::nlohmann_json
        Boost::pfr
        magic_enum::magic_enum
    )

    add_custom_target(ore_generate_config
        COMMAND ore_config_generator "${LEVI_GENERATED_CONFIG_DIR}"
        DEPENDS ore_config_generator
        COMMENT "Generating OreOutline config files"
        VERBATIM
    )
    return()
endif()

if(NOT DEFINED LEVI_PACKAGE_CONFIG_DIR)
    set(LEVI_PACKAGE_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated-config")
endif()
get_filename_component(LEVI_PACKAGE_CONFIG_DIR "${LEVI_PACKAGE_CONFIG_DIR}" ABSOLUTE)
foreach(config_file config.json config.schema.json)
    if(NOT EXISTS "${LEVI_PACKAGE_CONFIG_DIR}/${config_file}")
        message(FATAL_ERROR "Missing ${config_file} in ${LEVI_PACKAGE_CONFIG_DIR}; run scripts/build.sh first")
    endif()
endforeach()

if(NOT CMAKE_ANDROID_ARCH_ABI STREQUAL "arm64-v8a")
    message(FATAL_ERROR "OreOutline supports only arm64-v8a builds")
endif()

add_subdirectory("${preloader_android_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/_preloader_android")

add_library(${MOD_LIBRARY_NAME} SHARED
    src/main.cpp
    src/mod/Config.cpp
    src/mod/OreOutlineMod.cpp
)
target_include_directories(${MOD_LIBRARY_NAME} PRIVATE src "${LEVI_PRELOADER_INCLUDE_DIR}")
target_compile_options(${MOD_LIBRARY_NAME} PRIVATE
    $<$<COMPILE_LANG_AND_ID:CXX,Clang,GNU>:-fvisibility=hidden -fvisibility-inlines-hidden>
)
target_link_options(${MOD_LIBRARY_NAME} PRIVATE "-Wl,-z,max-page-size=16384")
find_library(log-lib log REQUIRED)
target_link_libraries(${MOD_LIBRARY_NAME} PRIVATE
    ${log-lib}
    nlohmann_json::nlohmann_json
    Boost::pfr
    magic_enum::magic_enum
    fmt::fmt
    preloader
)

if(NOT CMAKE_STRIP)
    message(FATAL_ERROR "CMAKE_STRIP is required to strip the generated library")
endif()
add_custom_command(TARGET ${MOD_LIBRARY_NAME} POST_BUILD
    COMMAND "${CMAKE_STRIP}" --strip-unneeded "$<TARGET_FILE:${MOD_LIBRARY_NAME}>"
    COMMENT "Stripping ${MOD_LIBRARY_NAME}"
    VERBATIM
)

configure_file(manifest.json.in "${CMAKE_CURRENT_BINARY_DIR}/manifest.json" @ONLY)
set(LEVI_PACKAGE_ROOT "${CMAKE_CURRENT_BINARY_DIR}/package")
set(LEVI_PACKAGE_MOD_DIR "${LEVI_PACKAGE_ROOT}/${MOD_ID}")
set(LEVI_PACKAGE_FILE "${CMAKE_CURRENT_BINARY_DIR}/${MOD_ID}-${MOD_VERSION}-${CMAKE_ANDROID_ARCH_ABI}.levipack")

add_custom_target(levi_package
    COMMAND "${CMAKE_COMMAND}" -E rm -rf "${LEVI_PACKAGE_ROOT}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${LEVI_PACKAGE_MOD_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy "$<TARGET_FILE:${MOD_LIBRARY_NAME}>" "${LEVI_PACKAGE_MOD_DIR}/lib${MOD_LIBRARY_NAME}.so"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_BINARY_DIR}/manifest.json" "${LEVI_PACKAGE_MOD_DIR}/manifest.json"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${LEVI_PACKAGE_CONFIG_DIR}" "${LEVI_PACKAGE_MOD_DIR}/config"
    COMMAND "${CMAKE_COMMAND}" -E chdir "${LEVI_PACKAGE_ROOT}" "${CMAKE_COMMAND}" -E tar "cf" "${LEVI_PACKAGE_FILE}" --format=zip "${MOD_ID}"
    DEPENDS ${MOD_LIBRARY_NAME}
    COMMENT "Packaging OreOutline for LeviLauncher"
    VERBATIM
)