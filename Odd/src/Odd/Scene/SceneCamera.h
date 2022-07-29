#pragma once

#include "glm.hpp"
#include "Odd/Renderer/Camera.h"

namespace Odd
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType {Perspective = 0, Orthographic = 1};
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetViewportSize(uint32_t width, uint32_t height);
		void SetPerspective(float fov, float nearClip, float farClip);
		void SetOrthographic(float size, float nearClip, float farClip);

		float GetPerspectiveFOV() const { return m_PerspectiveFOV; }
		void  SetPerspectiveFOV(float fov) { m_PerspectiveFOV = fov; CalculateProjection(); }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void  SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; CalculateProjection(); }
		float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void  SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; CalculateProjection(); }

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void  SetOrthographicSize(float size) { m_OrthographicSize = size; CalculateProjection(); }
		float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void  SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; CalculateProjection(); }
		float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void  SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; CalculateProjection(); }

		const float GetAspectRatio() const { return m_AspectRatio; }

		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; CalculateProjection(); }
		const ProjectionType GetProjectionType() const { return m_ProjectionType; }
	private:
		void CalculateProjection();
	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;
		float m_AspectRatio = 0.0f;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar =  1.0f;

		float m_PerspectiveFOV = 45.0f;
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
	};
}