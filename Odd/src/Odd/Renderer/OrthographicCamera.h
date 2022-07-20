#pragma once

#include "glm.hpp"

namespace Odd {

	class OrthographicCamera
	{
	public:
		OrthographicCamera() { m_ProjectionMatrix = m_ViewMatrix = m_ViewProjectionMatrix = glm::mat4(1.0f); }
		OrthographicCamera(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; CalculateViewMatrix(); }

		float GetRotation() { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; CalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void CalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		float m_Rotation = 0.0f;
	};

}