#include "FollowPlayer.h"
#include "Odd.h"

void FollowPlayer::OnCreate()
{
	// Cache Transform
	m_Transform = &GetComponent<TransformComponent>();

	// Get Stars & Player from the scene.
	m_Stars = m_Entity.GetEntityByName("Stars");
	m_Player = m_Entity.GetEntityByName("Player");
	
	if (m_Stars)
	{
		// Store the Difference in position between 
		// stars & camera at the start.
		m_StarsTransform = &m_Stars.GetComponent<TransformComponent>();

		// Calculate Offset.
		m_StarsOffsetFromCamera = glm::vec2(m_StarsTransform->Translation - m_Transform->Translation);
	}

	if (m_Player)
	{
		// Store the Difference in position between 
		// camera & player at the start.
		m_PlayerTransform = &m_Player.GetComponent<TransformComponent>();

		// Calculate Offset.
		m_PlayerOffsetFromCamera = glm::vec2(m_PlayerTransform->Translation - m_Transform->Translation);
	}
}

void FollowPlayer::OnDestroy()
{
	
}

void FollowPlayer::OnUpdate(Timestep ts)
{
	m_Transform = &GetComponent<TransformComponent>();

	if (!m_Player) return;

	m_PlayerTransform = &m_Player.GetComponent<TransformComponent>();

	m_CurrentTime += ts.GetSeconds();
	if (m_CurrentTime >= m_TimeToReachPlayer)
		m_CurrentTime = 0.0f;

	// Range: 0 - 1
	float interpolate = m_CurrentTime / m_TimeToReachPlayer;

	m_Transform->Translation.x = Lerp(m_Transform->Translation.x, m_PlayerTransform->Translation.x - m_PlayerOffsetFromCamera.x, interpolate);
	m_Transform->Translation.y = Lerp(m_Transform->Translation.y, m_PlayerTransform->Translation.y - m_PlayerOffsetFromCamera.y, interpolate);

	if (m_Stars)
	{
		m_StarsTransform = &m_Stars.GetComponent<TransformComponent>();

		// Make sure stars are same offset away from camera.
		m_StarsTransform->Translation.x = m_Transform->Translation.x + m_StarsOffsetFromCamera.x;
		m_StarsTransform->Translation.y = m_Transform->Translation.y + m_StarsOffsetFromCamera.y;
	}
}

float FollowPlayer::Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}