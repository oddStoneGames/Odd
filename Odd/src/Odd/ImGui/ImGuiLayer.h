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
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
		float m_Time;
	};

}