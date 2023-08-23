#pragma once

#include "Odd/Scene/ScriptableEntity.h"

using namespace Odd;

class FollowPlayer : public ScriptableEntity
{
public:
	~FollowPlayer() {}
private:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(Timestep ts) override;

	// Linear interpolation
	float Lerp(float a, float b, float t);
private:
	Entity m_Stars;
	TransformComponent* m_StarsTransform = nullptr;
	glm::vec2 m_StarsOffsetFromCamera = glm::vec2(0.0f);

	Entity m_Player;
	TransformComponent* m_PlayerTransform = nullptr;

	glm::vec2 m_PlayerOffsetFromCamera = glm::vec2(0.0f);
	// Time in which camera maintains the said offset with player.
	float m_TimeToReachPlayer = 0.5f;
	float m_CurrentTime = 0.0f;

	TransformComponent* m_Transform = nullptr;
};