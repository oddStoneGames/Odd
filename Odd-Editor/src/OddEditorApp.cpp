#include <Odd.h>
#include "Odd/Core/EntryPoint.h"
#include "ImGui/include/imgui.h"
#include "glm/gtc/matrix_transform.hpp"

#include "EditorLayer.h"

namespace Odd
{
	class OddEditor : public Application
	{
	public:
		OddEditor()
			: Application("Odd Editor")
		{
			PushLayer(new EditorLayer());
		}
		~OddEditor() {}
	};

	Application* CreateApplication()
	{
		return new OddEditor();
	}
}