
message(Using glfw from $$PWD)

## GLFW #####################################################################
INCLUDEPATH += $$PWD/include
SOURCES += \
        $$PWD/src/context.c \
        $$PWD/src/init.c \
        $$PWD/src/input.c \
        $$PWD/src/monitor.c \
        $$PWD/src/vulkan.c \
        $$PWD/src/window.c \
        $$PWD/src/xkb_unicode.c\
        $$PWD/src/osmesa_context.c \
        $$PWD/src/egl_context.c

win32{ #Windows
    DEFINES+= _GLFW_WIN32
    SOURCES += \
        $$PWD/src/wgl_context.c \
        $$PWD/src/win32_init.c \
        $$PWD/src/win32_joystick.c \
        $$PWD/src/win32_monitor.c \
        $$PWD/src/win32_thread.c \
        $$PWD/src/win32_time.c \
        $$PWD/src/win32_window.c

    LIBS+= \
        -lkernel32 \
        -luser32 \
        -lgdi32 \
        -lshell32
    DEFINES+= _CRT_SECURE_NO_WARNINGS
}

unix:!macx{ # Linux
    DEFINES+= _GLFW_X11
    SOURCES +=\
        $$PWD/src/glx_context.c \
        $$PWD/src/linux_joystick.c \
        $$PWD/src/posix_thread.c \
        $$PWD/src/posix_time.c \
        $$PWD/src/x11_init.c \
        $$PWD/src/x11_monitor.c \
        $$PWD/src/x11_window.c

    LIBS += -lX11 -lm -pthread -ldl
# If we ever want to switch to wayland
#   DEFINES+= _GLFW_WAYLAND
#        $$PWD/src/wl_init.c \
#        $$PWD/src/wl_monitor.c \
#        $$PWD/src/wl_window.c \
}

macx{ # Mac support NOT TESTED!!
    DEFINES+= _GLFW_COCOA
    SOURCES += \
        $$PWD/src/cocoa_time.c \
        $$PWD/src/cocoa_init.m\
        $$PWD/src/cocoa_joystick.m\
        $$PWD/src/cocoa_monitor.m\
        $$PWD/src/cocoa_window.m\
        $$PWD/src/nsgl_context.m

}
