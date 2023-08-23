# MINIMUM VERSION REQUIRED IS 3.25 BECAUSE OF "LINUX"
cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ODD ENGINE
project(Odd)

# BOX 2D
add_subdirectory(Odd/vendor/Box2D)
list(APPEND INCLUDES Odd/vendor/Box2D/include)
list(APPEND LIBS box2d)

# ENTT
list(APPEND INCLUDES Odd/vendor/entt/include)

# GLAD
add_subdirectory(Odd/vendor/Glad)
list(APPEND LIBS glad)

# GLFW
add_subdirectory(Odd/vendor/GLFW)
list(APPEND LIBS glfw)

# GLM
list(APPEND INCLUDES Odd/vendor/glm)

# IMGUI
add_subdirectory(Odd/vendor/ImGui)
list(APPEND LIBS imgui)

# IMGUIZMO
add_subdirectory(Odd/vendor/ImGuizmo)
list(APPEND INCLUDES Odd/vendor/ImGuizmo)
list(APPEND LIBS imguizmo)

# OPENGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
list(APPEND INCLUDES ${OPENGL_INCLUDE_DIR})
list(APPEND LIBS ${OPENGL_LIBRARIES})

# SPDLOG
list(APPEND INCLUDES Odd/vendor)

# STB_IMAGE
list(APPEND INCLUDES Odd/vendor/stb_image)

# YAML-CPP
add_subdirectory(Odd/vendor/yaml-cpp)
list(APPEND INCLUDES Odd/vendor/yaml-cpp/include)
list(APPEND LIBS yaml-cpp)

# ADD EXTRA LIBRARIES BASED ON THE OPERATING SYSTEM
if(WIN32)
    list(APPEND gdi32 user32)
    list(APPEND COMPILE_DEFS ODD_PLATFORM_WINDOWS)
elseif(LINUX)
    list(APPEND LIBS X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor)
    list(APPEND COMPILE_DEFS ODD_PLATFORM_LINUX)
endif()

# SET SOURCE FILES
set(SOURCE_FILES Odd/src/Odd/Core/Application.cpp Odd/src/Odd/Core/Application.h Odd/src/Odd/Core/Core.h Odd/src/Odd/Core/EntryPoint.h Odd/src/Odd/Core/Layer.cpp Odd/src/Odd/Core/Layer.h
                 Odd/src/Odd/Core/LayerStack.cpp Odd/src/Odd/Core/LayerStack.h Odd/src/Odd/Core/Log.cpp Odd/src/Odd/Core/Log.h Odd/src/Odd/Core/Timestep.h Odd/src/Odd/Core/Window.h
                 Odd/src/Odd/Core/UUID.h Odd/src/Odd/Core/UUID.cpp
                 Odd/src/Odd/Debug/Instrumentor.h
                 Odd/src/Odd/Events/ApplicationEvent.h Odd/src/Odd/Events/Event.h Odd/src/Odd/Events/KeyEvent.h Odd/src/Odd/Events/MouseEvent.h
                 Odd/src/Odd/ImGui/Windows/imgui_impl_glfw.cpp Odd/src/Odd/ImGui/Windows/imgui_impl_glfw.h Odd/src/Odd/ImGui/Windows/imgui_impl_opengl3.cpp Odd/src/Odd/ImGui/Windows/imgui_impl_opengl3.h
                 Odd/src/Odd/ImGui/ImGuiLayer.cpp Odd/src/Odd/ImGui/ImGuiLayer.h
                 Odd/src/Odd/Input/Input.h Odd/src/Odd/Input/KeyCodes.h Odd/src/Odd/Input/MouseCodes.h
                 Odd/src/Odd/Math/Math.cpp Odd/src/Odd/Math/Math.h
                 Odd/src/Odd/Renderer/Buffer.cpp Odd/src/Odd/Renderer/Buffer.h Odd/src/Odd/Renderer/Camera.h Odd/src/Odd/Renderer/EditorCamera.cpp Odd/src/Odd/Renderer/EditorCamera.h
                 Odd/src/Odd/Renderer/FrameBuffer.cpp Odd/src/Odd/Renderer/FrameBuffer.h Odd/src/Odd/Renderer/GraphicsContext.h Odd/src/Odd/Renderer/OrthographicCamera.cpp Odd/src/Odd/Renderer/OrthographicCamera.h
                 Odd/src/Odd/Renderer/RenderCommand.cpp Odd/src/Odd/Renderer/RenderCommand.h Odd/src/Odd/Renderer/Renderer.cpp Odd/src/Odd/Renderer/Renderer.h Odd/src/Odd/Renderer/Renderer2D.cpp Odd/src/Odd/Renderer/Renderer2D.h
                 Odd/src/Odd/Renderer/RendererAPI.cpp Odd/src/Odd/Renderer/RendererAPI.h Odd/src/Odd/Renderer/Shader.cpp Odd/src/Odd/Renderer/Shader.h Odd/src/Odd/Renderer/ShaderDataTypes.h Odd/src/Odd/Renderer/SubTexture2D.cpp Odd/src/Odd/Renderer/SubTexture2D.h
                 Odd/src/Odd/Renderer/Texture.cpp Odd/src/Odd/Renderer/Texture.h Odd/src/Odd/Renderer/UniformBuffer.cpp Odd/src/Odd/Renderer/UniformBuffer.h Odd/src/Odd/Renderer/VertexArray.cpp Odd/src/Odd/Renderer/VertexArray.h
                 Odd/src/Odd/Scene/Components.h Odd/src/Odd/Scene/Entity.h Odd/src/Odd/Scene/Scene.cpp Odd/src/Odd/Scene/Scene.h Odd/src/Odd/Scene/SceneCamera.cpp Odd/src/Odd/Scene/SceneCamera.h Odd/src/Odd/Scene/SceneSerializer.cpp Odd/src/Odd/Scene/SceneSerializer.h
                 Odd/src/Odd/Scene/ScriptableEntity.h Odd/src/Odd/Utils/PlatformUtils.h Odd/src/Odd/OrthographicCameraController.cpp Odd/src/Odd/OrthographicCameraController.h
                 Odd/src/Platform/OpenGL/OpenGLBuffer.cpp Odd/src/Platform/OpenGL/OpenGLBuffer.h Odd/src/Platform/OpenGL/OpenGLContext.cpp Odd/src/Platform/OpenGL/OpenGLContext.h Odd/src/Platform/OpenGL/OpenGLFramebuffer.cpp Odd/src/Platform/OpenGL/OpenGLFramebuffer.h
                 Odd/src/Platform/OpenGL/OpenGLRendererAPI.cpp Odd/src/Platform/OpenGL/OpenGLRendererAPI.h Odd/src/Platform/OpenGL/OpenGLShader.cpp Odd/src/Platform/OpenGL/OpenGLShader.h Odd/src/Platform/OpenGL/OpenGLTexture.cpp Odd/src/Platform/OpenGL/OpenGLTexture.h
                 Odd/src/Platform/OpenGL/OpenGLUniformBuffer.cpp Odd/src/Platform/OpenGL/OpenGLUniformBuffer.h Odd/src/Platform/OpenGL/OpenGLVertexArray.cpp Odd/src/Platform/OpenGL/OpenGLVertexArray.h
                 Odd/src/Platform/Windows/WindowsInput.cpp Odd/src/Platform/Windows/WindowsPlatformUtils.cpp Odd/src/Platform/Windows/WindowsWindow.cpp Odd/src/Platform/Windows/WindowsWindow.h
                 Odd/src/Odd.h
                 Odd/vendor/stb_image/stb_image.cpp Odd/vendor/stb_image/stb_image.h)

# BUILD STATIC LIBRARY
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# PRECOMPILED HEADER
target_precompile_headers(${PROJECT_NAME} PRIVATE Odd/src/oddpch.h)

# COMPILE DEFINITIONS
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_DIR="${PROJECT_SOURCE_DIR}/Odd/" PRIVATE ${COMPILE_DEFS} PRIVATE GLFW_INCLUDE_NONE)

# INCLUDE DIRECTORIES
list(APPEND INCLUDES Odd/src)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})

# LINK LIBRARIES
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBS})



# SANDBOX
project(Sandbox)

# SET SOURCE FILES
set(SOURCE_FILES Examples/Sandbox/src/Sandbox2D.cpp Examples/Sandbox/src/Sandbox2D.h Examples/Sandbox/src/SandboxApp.cpp)

# BUILD EXECUTABLE
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# COMPILE DEFINITIONS
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_DIR="${PROJECT_SOURCE_DIR}/Examples/Sandbox/" PRIVATE ${COMPILE_DEFS})

# INCLUDE DIRECTORIES
set(INCLUDES Odd/vendor Odd/src Odd/vendor/glm Odd/vendor/entt/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})

# LINK LIBRARIES
target_link_libraries(${PROJECT_NAME} PRIVATE Odd)




# ODD EDITOR
project(Odd-Editor)

# SET SOURCE FILES
set(SOURCE_FILES Odd-Editor/src/Panels/ContentBrowserPanel.cpp Odd-Editor/src/Panels/ContentBrowserPanel.h 
                 Odd-Editor/src/Panels/SceneHierarchyPanel.cpp Odd-Editor/src/Panels/SceneHierarchyPanel.h
                 Odd-Editor/src/EditorLayer.cpp Odd-Editor/src/EditorLayer.h Odd-Editor/src/OddEditorApp.cpp
                 Odd-Editor/SandboxProject/Scripts/PlayerController.cpp Odd-Editor/SandboxProject/Scripts/PlayerController.h
                 Odd-Editor/SandboxProject/Scripts/FollowPlayer.cpp Odd-Editor/SandboxProject/Scripts/FollowPlayer.h)

# BUILD EXECUTABLE
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# SET THIS PROJECT AS STARTUP PROJECT
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# COMPILE DEFINITIONS
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_DIR="${PROJECT_SOURCE_DIR}/Odd-Editor/" PRIVATE ${COMPILE_DEFS})

# INCLUDE DIRECTORIES
set(INCLUDES Odd/vendor Odd/src Odd/vendor/glm Odd/vendor/entt/include Odd/vendor/ImGuizmo)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})

# LINK LIBRARIES
target_link_libraries(${PROJECT_NAME} PRIVATE Odd)