cmake_minimum_required(VERSION 3.10)
project(rpi4-sdl)

cmake_policy(SET CMP0079 NEW)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -Wpedantic")

set(LV_BUILD_SET_CONFIG_OPTS
    ON
    CACHE BOOL "create CMAKE variables from lv_conf_internal.h" FORCE)

add_subdirectory(../lvgl ${CMAKE_CURRENT_BINARY_DIR}/lvgl)

set(EXTRA_SOURCE_FILES)

if(CONFIG_LV_USE_SDL)
  message("Including SDL2 support")

  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL2 REQUIRED sdl2)
  pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

  list(APPEND PKG_CONFIG_LIB ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
  list(APPEND PKG_CONFIG_INC ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

endif()

if(CONFIG_LV_USE_WAYLAND)
  message("Including Wayland support")

  find_package(PkgConfig REQUIRED)
  pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
  pkg_check_modules(WAYLAND_CURSOR REQUIRED wayland-cursor)
  pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)

  list(APPEND PKG_CONFIG_LIB ${WAYLAND_CLIENT_LIBRARIES})
  list(APPEND PKG_CONFIG_LIB ${WAYLAND_CURSOR_LIBRARIES})
  list(APPEND PKG_CONFIG_LIB ${XKBCOMMON_LIBRARIES})

  # Wayland protocols
  pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols>=1.25)
  pkg_get_variable(WAYLAND_PROTOCOLS_BASE wayland-protocols pkgdatadir)

  execute_process(
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND "scripts/gen_wl_protocols.sh"
            "${CMAKE_CURRENT_BINARY_DIR}/protocols"
    OUTPUT_VARIABLE WAYLAND_PROTOCOLS_SRC
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  string(REPLACE "\n" ";" WAYLAND_PROTOCOLS_SRC "${WAYLAND_PROTOCOLS_SRC}")

  list(APPEND PKG_CONFIG_INC "${CMAKE_CURRENT_BINARY_DIR}/protocols")
  list(APPEND EXTRA_SOURCE_FILES "${WAYLAND_PROTOCOLS_SRC}")
endif()

target_include_directories(lvgl PUBLIC ${PKG_CONFIG_INC})
target_link_libraries(lvgl PUBLIC ${PKG_CONFIG_LIB} m)

if(CONFIG_LV_USE_SDL)
  set(TARGET_NAME "rpi4-sdl")
elseif(CONFIG_LV_USE_WAYLAND)
  set(TARGET_NAME "rpi4-wayland")
else()
  message(FATAL_ERROR "Invalid config")
endif()

add_executable(${TARGET_NAME} src/main.c ${EXTRA_SOURCE_FILES})
target_include_directories(${TARGET_NAME} PRIVATE ../lvgl/demos ../lvgl)
target_link_libraries(${TARGET_NAME} lvgl)
set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_DEFINITIONS
                                                "${LVGL_COMPILER_DEFINES}")
