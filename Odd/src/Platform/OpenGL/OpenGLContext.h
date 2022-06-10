#pragma once
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include "Odd/Renderer/GraphicsContext.h"
#include "Odd/Renderer/Renderer.h"
#include "Odd/Renderer/Shader.h"
#include "Odd/Renderer/Buffer.h"
#include "Odd/Renderer/VertexArray.h"
#include "Odd/Renderer/OrthographicCamera.h"

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

		std::shared_ptr<Shader> m_TriangleShader;
		std::shared_ptr<VertexArray>  m_TriangleVAO;

		std::shared_ptr<Shader> m_SquareShader;
		std::shared_ptr<VertexArray>  m_SquareVAO;

		GLFWwindow* m_WindowHandle;
	};

}