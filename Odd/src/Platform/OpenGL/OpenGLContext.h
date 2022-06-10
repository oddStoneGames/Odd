#pragma once
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "Odd/Renderer/GraphicsContext.h"
#include "Odd/Renderer/Renderer.h"
#include "Odd/Renderer/Shader.h"
#include "Odd/Renderer/Buffer.h"
#include "Odd/Renderer/VertexArray.h"

namespace Odd {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}