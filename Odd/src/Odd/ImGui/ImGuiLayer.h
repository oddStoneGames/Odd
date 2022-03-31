#pragma once

#include "Odd/Core/Layer.h"
#include "Odd/Events/KeyEvent.h"
#include "Odd/Events/MouseEvent.h"
#include "Odd/Events/ApplicationEvent.h"

namespace Odd {

	class ODD_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time;
	};

}