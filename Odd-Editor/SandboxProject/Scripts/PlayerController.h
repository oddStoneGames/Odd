#pragma once

#include "Odd/Scene/ScriptableEntity.h"

using namespace Odd;

class b2World;
class b2Fixture;
class PlayerController : public ScriptableEntity
{
public:
	~PlayerController() {}
private:
	virtual void OnCreate() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate(Timestep ts) override;

	// Returns if the player is grounded.
	bool IsGrounded(b2World* world, b2Fixture* playerFixture);

	void Animate(Timestep ts);
private:
	float m_MovementSpeed = 1.6f;
	float m_JumpSpeed = 0.8f;
	
	glm::vec2 m_Force{}, m_VerticalForce{};

	// True when movement input is recieved.
	bool m_MovementInput = false;

	enum class PlayerState { Idle = 0, Running, Jumping };
	PlayerState m_PlayerState;

	// After this amount of time passes, 
	// player can jump again if grounded.
	float m_JumpCooldown = 0.5f;

	// time since last jump
	float m_TimeSinceLastJump = 0.0f;

	// True for first jump.
	bool m_FirstJump = true;

	// True when jump registered.
	bool m_JumpInput = false;

	// True if player is grounded.
	bool m_IsGrounded = false;

	// The direction in which player is looking, 1 for right, -1 for left.
	int m_Direction = 1;
	
	// Subtexture cofiguration data.
	std::string m_MoveSpriteSheetPath = PROJECT_DIR"../Odd-Editor/SandboxProject/Textures/player_run.png";
	std::string m_JumpSpriteSheetPath = PROJECT_DIR"../Odd-Editor/SandboxProject/Textures/player_jump.png";

	Ref<Texture2D> m_MoveSprite, m_JumpSprite;

	// Current Subtexture Index.
	int m_SubtextureIndex = 0;

	// Current Subtexture animation timer.
	float m_AnimationTimer = 0.0f;

	// The Time player spends on each subtexture while moving.
	float m_TimeToSwitchRunSubtexture = 0.1f;

	// The Time player spends on each subtexture while jumping.
	float m_TimeToSwitchJumpSubtexture = 0.2f;

	// Total number of sprites in the run sprite sheet.
	const int m_NumberOfRunSprites = 8;

	// Total number of sprites in the jump sprite sheet.
	const int m_NumberOfJumpSprites = 5;

	b2World* m_PhysicsWorld = nullptr;
	TransformComponent* m_Transform = nullptr;
	SpriteRendererComponent* m_SpriteRenderer = nullptr;
	Rigidbody2DComponent* m_Rigidbody2d = nullptr;
	BoxCollider2DComponent* m_BoxCollider2d = nullptr;
};