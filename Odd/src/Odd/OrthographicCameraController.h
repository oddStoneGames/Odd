#pragma once

#include "Odd/Renderer/OrthographicCamera.h"
#include "Odd/Core/Timestep.h"

#include "Odd/Events/ApplicationEvent.h"
#include "Odd/Events/MouseEvent.h"

namespace Odd {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		float GetZoomLevel() { return m_ZoomLevel; }
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		bool m_Rotation;
		OrthographicCamera m_Camera;

		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 2.5f;
		float m_CameraRotationSpeed = 90.0f;
	};
}