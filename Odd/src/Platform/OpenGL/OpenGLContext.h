#pragma once
#include "Odd/Renderer/GraphicsContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Odd/Renderer/Buffer.h"
#include "Odd/Renderer/VertexArray.h"

namespace Odd {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		void RenderTriangle();
		void RenderSquare();
	private:
		unsigned int m_TriangleShaderID;
		std::shared_ptr<VertexArray>  m_TriangleVAO;

		unsigned int m_SquareShaderID;
		std::shared_ptr<VertexArray>  m_SquareVAO;

		GLFWwindow* m_WindowHandle;
	};

}