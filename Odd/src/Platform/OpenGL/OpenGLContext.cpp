#include "oddpch.h"
#include "OpenGLContext.h"

namespace Odd {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		if (windowHandle == nullptr)
			DEBUG_CORE_ERROR("Window Handle is Null");
	}

	void OpenGLContext::Init()
	{
		ODD_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);
		int gladStatus = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (gladStatus > 0)
			DEBUG_CORE_INFO("Glad Initialized Successfully!");
		else
			DEBUG_CORE_ERROR("Failed to Initialized Glad!");

		DEBUG_CORE_INFO("OpenGL Info:\n\t\t\t\tVendor:   {0}\n\t\t\t\tRenderer: {1}\n\t\t\t\tVersion:  {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		ODD_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}

	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
			case Odd::ShaderDataType::None:		return 0;
			case Odd::ShaderDataType::Bool:		return GL_BOOL;
			case Odd::ShaderDataType::Int:		return GL_INT;
			case Odd::ShaderDataType::Int2:		return GL_INT;
			case Odd::ShaderDataType::Int3:		return GL_INT;
			case Odd::ShaderDataType::Int4:		return GL_INT;
			case Odd::ShaderDataType::Float:	return GL_FLOAT;
			case Odd::ShaderDataType::Float2:	return GL_FLOAT;
			case Odd::ShaderDataType::Float3:	return GL_FLOAT;
			case Odd::ShaderDataType::Float4:	return GL_FLOAT;
			case Odd::ShaderDataType::Mat3:		return GL_FLOAT;
			case Odd::ShaderDataType::Mat4:		return GL_FLOAT;
		}

		DEBUG_CORE_ERROR("Shader Data Type Unknown!");
		return 0;
	}
}