#include "oddpch.h"
#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Odd
{
	SceneCamera::SceneCamera()
	{
		CalculateProjection();
	}
	
	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float) width / (float) height;
		CalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		CalculateProjection();
	}

	void SceneCamera::CalculateProjection()
	{
		float orthoLeft = -0.5f * m_AspectRatio * m_OrthographicSize;
		float orthoRight = 0.5f * m_AspectRatio * m_OrthographicSize;
		float orthoBottom = -0.5f * m_OrthographicSize;
		float orthoTop = 0.5f * m_OrthographicSize;

		m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom,
			orthoTop, m_OrthographicNear, m_OrthographicFar);
	}
}