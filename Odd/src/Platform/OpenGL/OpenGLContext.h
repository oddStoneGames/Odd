#pragma once
#include "Odd/Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Odd {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		void RenderTriangle();
	private:
		unsigned int m_TriangleVAO, m_TriangleVBO, m_TriangleShaderID;
		GLFWwindow* m_WindowHandle;
	};

}