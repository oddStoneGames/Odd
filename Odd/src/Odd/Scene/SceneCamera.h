#pragma once

#include "glm.hpp"
#include "Odd/Renderer/Camera.h"

namespace Odd
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetViewportSize(uint32_t width, uint32_t height);
		void SetOrthographic(float size, float nearClip, float farClip);

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; CalculateProjection(); }

		const float GetAspectRatio() const { return m_AspectRatio; }
	private:
		void CalculateProjection();
	private:
		float m_AspectRatio = 0.0f;
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar =  1.0f;
	};
}