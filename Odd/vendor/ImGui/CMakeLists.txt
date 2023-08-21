project(imgui)

add_library(
        ${PROJECT_NAME} STATIC
        include/imgui.cpp
        include/imgui.h
        include/imgui_demo.cpp
        include/imgui_draw.cpp
        include/imgui_widgets.cpp
        include/imgui_tables.cpp
        include/imgui_internal.h
        include/imstb_rectpack.h
        include/imstb_textedit.h
        include/imstb_truetype.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include)