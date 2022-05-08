#pragma once
#include "Odd/Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Odd/Renderer/Buffer.h"

namespace Odd {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		void RenderTriangle();
	private:
		unsigned int m_TriangleVAO, m_TriangleShaderID;
		std::unique_ptr<VertexBuffer> m_TriangleVBO;
		GLFWwindow* m_WindowHandle;
	};

}