project(glfw)

if(WIN32)
    set(_GLFW_WIN32 1)
elseif(LINUX)
    set(_GLFW_X11 1)
endif()
configure_file(src/glfw_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/glfw_config.h)

set(SOURCE_FILES include/GLFW/glfw3.h include/GLFW/glfw3native.h
                 src/glfw_config.h src/context.c src/init.c
                 src/input.c src/monitor.c src/vulkan.c src/window.c)

if(_GLFW_WIN32)
    list(APPEND SOURCE_FILES src/win32_platform.h src/win32_joystick.h src/wgl_context.h src/egl_context.h src/osmesa_context.h
                             src/win32_init.c src/win32_joystick.c src/win32_monitor.c src/win32_time.c src/win32_thread.c
                             src/win32_window.c src/wgl_context.c src/egl_context.c src/osmesa_context.c)
elseif(_GLFW_X11)
    list(APPEND SOURCE_FILES src/x11_platform.h src/xkb_unicode.h src/posix_time.h src/posix_thread.h src/glx_context.h src/egl_context.h src/osmesa_context.h
                             src/x11_init.c src/x11_monitor.c src/x11_window.c src/xkb_unicode.c src/posix_time.c
                             src/posix_thread.c src/glx_context.c src/egl_context.c src/osmesa_context.c)
endif()

if (_GLFW_X11 OR _GLFW_WAYLAND)
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        list(APPEND SOURCE_FILES src/linux_joystick.h)
        list(APPEND SOURCE_FILES src/linux_joystick.c)
    else()
        list(APPEND SOURCE_FILES src/null_joystick.h)
        list(APPEND SOURCE_FILES src/null_joystick.c)
    endif()
endif()

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_definitions(${PROJECT_NAME} PRIVATE _GLFW_USE_CONFIG_H)
if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

if (_GLFW_WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)
endif()

# HACK: When building on MinGW, WINVER and UNICODE need to be defined before
# the inclusion of stddef.h (by glfw3.h), which is itself included before
# win32_platform.h.  We define them here until a saner solution can be found
# NOTE: MinGW-w64 and Visual C++ do /not/ need this hack.
if (MINGW)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WINVER=0x0501)
endif()