cmake_minimum_required(VERSION 3.20)      # any recent version works
project(frostbit C)                        # your project name

# ------------------------------------------------------------------
# 1️⃣  Compiler options – match what you had in the Makefile
# ------------------------------------------------------------------
set(CMAKE_C_STANDARD 11)                   # -std=c11
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g")

# Base warnings (‑Wall ‑Wextra ‑pedantic …)
add_compile_options(
        -Wall
        -Wextra
        -pedantic
        -Wmissing-declarations
)

# Uncomment if you really want “‑Werror”
# add_compile_options(-Werror)

# ------------------------------------------------------------------
# 2️⃣  Find SDL 3 via pkg‑config (same as `pkg-config --cflags sdl3 sdl3-image`)
# ------------------------------------------------------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
pkg_check_modules(SDL3_IMAGE REQUIRED IMPORTED_TARGET sdl3-image)

# Include directories reported by pkg‑config
include_directories(${SDL3_INCLUDE_DIRS} ${SDL3_IMAGE_INCLUDE_DIRS})

# ------------------------------------------------------------------
# 3️⃣  Gather **all** source files that belong to the executable
# ------------------------------------------------------------------
# Assuming everything under ./src/*.c is part of the program
file(GLOB_RECURSE PROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# ------------------------------------------------------------------
# 4️⃣  Link the SDL 3 libraries (same as `pkg-config --libs sdl3 sdl3-image tmx`)
# ------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${SDL3_LIBRARIES}
        ${SDL3_IMAGE_LIBRARIES}
        ${TMX_LIBRARIES}
        tmx
)

# ------------------------------------------------------------------
# 5️⃣  Optional: Enable AddressSanitizer for a “memcheck” style build
# ------------------------------------------------------------------
option(ENABLE_ASAN "Compile with AddressSanitizer" OFF)
if (ENABLE_ASAN)
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()