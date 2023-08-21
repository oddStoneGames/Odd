project(imguizmo)

set(SOURCE_FILES GraphEditor.cpp GraphEditor.h ImCurveEdit.cpp ImCurveEdit.h ImGradient.cpp ImGradient.h
                 ImGuizmo.cpp ImGuizmo.h ImSequencer.cpp ImSequencer.h ImZoomSlider.h)

# OPENGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ../ImGui/include PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})