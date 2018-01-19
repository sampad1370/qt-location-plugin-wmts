cmake_minimum_required(VERSION 3.10)

set(PACKAGE "qtlocation-plugin-wms")
set(PACKAGE_VERSION_MAJOR 0)
set(PACKAGE_VERSION_MINOR 1)
set(PACKAGE_VERSION_REVISION 0)
set(PACKAGE_VERSION_COMMITHASH "developerbuild" CACHE STRING "git commit hash")
enable_testing()
project(${PACKAGE} LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

option (STATIC_PLUGIN "Build as a static qt plugin" OFF)

message(STATUS "Version: ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_REVISION} (${PACKAGE_VERSION_COMMITHASH})")

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    option(ENABLE_ADDRESS_SANITIZER "Enable the address sanitizer" OFF)
    message(STATUS "Address sanitizer: ${ENABLE_ADDRESS_SANITIZER}")

    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /bigobj /w34100 /w34101 /w35038 /w34127")
        # avoid windows specific warnings
        add_definitions(
            -D_CRT_SECURE_NO_WARNINGS
            -D_SCL_SECURE_NO_WARNINGS
            -D_SILENCE_CXX17_NEGATORS_DEPRECATION_WARNING
            -D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
            -D_SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING
        )

        # Enable faster linking for debug builds
        set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG:FASTLINK")
        set (CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /DEBUG:FASTLINK")
        set (CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} /DEBUG:FASTLINK")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wfatal-errors -Wno-unknown-pragmas -fdiagnostics-color")
        if (CMAKE_COMPILER_IS_GNUCXX)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized")
        endif ()
    endif ()

    if (NOT MSVC AND ENABLE_ADDRESS_SANITIZER)
        set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1 -fno-optimize-sibling-calls -fno-omit-frame-pointer -fsanitize=address")
        set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -static-libasan -fno-omit-frame-pointer -fsanitize=address")
        set (CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -static-libasan -fno-omit-frame-pointer -fsanitize=address")
        set (CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} -static-libasan -fno-omit-frame-pointer -fsanitize=address")
    endif ()

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND NOT MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif ()

    find_package(Qt5Location CONFIG REQUIRED)
    find_package(Qt5Positioning CONFIG REQUIRED)
    find_package(Qt5Quick CONFIG REQUIRED)
    find_package(Qt5QuickControls2 CONFIG REQUIRED)
endif ()

if (MSVC)
    set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
    set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
else ()
    set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
    set (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
endif ()

add_subdirectory(plugin)
add_subdirectory(demo)
