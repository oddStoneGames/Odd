#include "oddpch.h"
#include "OrthographicCameraController.h"

#include "Odd/Input/Input.h"
#include "Odd/Input/KeyCodes.h"

namespace Odd {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}
	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		float zoomLevelTranslationSpeed = m_CameraTranslationSpeed * m_ZoomLevel;

		if (Input::IsKeyPressed(Key::A))
			m_CameraPosition.x -= zoomLevelTranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::D))
			m_CameraPosition.x += zoomLevelTranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::W))
			m_CameraPosition.y += zoomLevelTranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::S))
			m_CameraPosition.y -= zoomLevelTranslationSpeed * ts;

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(OrthographicCameraController::OnWindowResized));
	}
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.05f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.01f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float) e.GetWidth() / (float) e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}