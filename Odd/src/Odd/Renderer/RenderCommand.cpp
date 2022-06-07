#include "oddpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Odd {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

}