#include "oddpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Odd {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}