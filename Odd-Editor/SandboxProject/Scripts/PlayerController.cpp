#include "PlayerController.h"
#include "Odd.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_world_callbacks.h"

void PlayerController::OnCreate()
{
	// Cache Components.
	m_Transform = &GetComponent<TransformComponent>();
	m_Rigidbody2d = &GetComponent<Rigidbody2DComponent>();
	m_SpriteRenderer = &GetComponent<SpriteRendererComponent>();
	m_BoxCollider2d = &GetComponent<BoxCollider2DComponent>();
	m_PhysicsWorld = m_Entity.GetScene()->GetPhysics2DWorld();

	if(HasComponent<AudioSourceComponent>())
		m_AudioSourceComponent = &GetComponent<AudioSourceComponent>();

	// Create Textures for subtexturing.
	m_MoveSprite = Texture2D::Create(m_MoveSpriteSheetPath);
	m_JumpSprite = Texture2D::Create(m_JumpSpriteSheetPath);
}

void PlayerController::OnDestroy()
{
	
}

void PlayerController::OnUpdate(Timestep ts)
{
	m_Transform = &GetComponent<TransformComponent>();
	m_MovementInput = false;

	if (Input::IsKeyPressed(Key::D))
	{
		// Set Movement Direction to Right.
		m_Direction = 1;
		m_MovementInput = true;
	}
	if (Input::IsKeyPressed(Key::A))
	{
		// Set Movement Direction to Left.
		m_Direction = -1;
		m_MovementInput = true;
	}
	
	if (m_MovementInput)
	{
		// Move based on direction.
		m_Force = m_Rigidbody2d->GetLinearVelocity();
		m_Force.x = m_Direction * m_MovementSpeed;
		m_Rigidbody2d->SetLinearVelocity(m_Force);
	}
	
	// Set Player State.
	m_PlayerState = m_MovementInput ? PlayerState::Running : (m_JumpInput || glm::abs(m_Rigidbody2d->GetLinearVelocity().y) > 0.01f  ? PlayerState::Jumping : PlayerState::Idle);

	if (m_JumpInput)
	{
		// Increment time since last jump.
		m_TimeSinceLastJump += ts.GetSeconds();

		if (m_TimeSinceLastJump >= m_JumpCooldown)
			m_JumpInput = false;
	}

	// Check if the player is grounded.
	m_IsGrounded = IsGrounded(m_PhysicsWorld, (b2Fixture*)m_BoxCollider2d->RuntimeFixture);

	if (m_IsGrounded)
	{
		// Allow jumping.
		if (Input::IsKeyPressed(Key::Space) && (m_TimeSinceLastJump >= m_JumpCooldown || m_FirstJump))
		{
			m_VerticalForce = glm::vec2(0, m_JumpSpeed);
			m_Rigidbody2d->ApplyLinearImpulseToCenter(m_VerticalForce);
			m_TimeSinceLastJump = 0.0f;
			m_JumpInput = true;
			m_FirstJump = false;

			//Play Jump Sound!
			if(m_AudioSourceComponent)
				m_AudioSourceComponent->Play();
		}
	}

	// Change which direction player is facing
	//  using negative/positive scaling.
	m_Transform->Scale.x = glm::abs(m_Transform->Scale.x) * m_Direction;

	// Chane Sprites based on the current player state using different sprite sheets.
	Animate(ts);
}

bool PlayerController::IsGrounded(b2World* world, b2Fixture* playerFixture)
{
	// Get the AABB of the player's fixture
	b2AABB playerAABB = playerFixture->GetAABB(0); // Use the first child index

	// Define the AABB for the ground check area
	b2AABB groundAABB;
	groundAABB.lowerBound = playerAABB.lowerBound - b2Vec2(0.0f, 0.3f); // Adjust for more grounding length.
	groundAABB.upperBound = playerAABB.lowerBound + b2Vec2(0.0f, 0.3f);

	// Callback for QueryAABB to check for ground
	class QueryCallback : public b2QueryCallback
	{
	public:
		bool hitGround = false;

		bool ReportFixture(b2Fixture* fixture) override
		{
			// Ignore the player's own fixture
			if (fixture != playerFixture)
			{
				hitGround = true;
				return false; // Stop the query after hitting the first fixture
			}
			return true; // Continue the query
		}

		b2Fixture* playerFixture;
	};

	QueryCallback queryCallback;
	queryCallback.playerFixture = playerFixture;

	// Perform the QueryAABB
	world->QueryAABB(&queryCallback, groundAABB);

	return queryCallback.hitGround;
}

void PlayerController::Animate(Timestep ts)
{
	if (m_PlayerState == PlayerState::Idle)
	{
		// Set Current texture as move sprite.
		m_SpriteRenderer->Texture = m_MoveSprite;
		// Idle State Index = 0
		m_SubtextureIndex = 0;
		// Set Subtexture Index
		m_SpriteRenderer->SubtextureCoords = glm::vec2(m_SubtextureIndex, 0);
	}
	else if (m_PlayerState == PlayerState::Running)
	{
		// Set Current texture as move sprite.
		m_SpriteRenderer->Texture = m_MoveSprite;
		m_AnimationTimer += ts.GetSeconds();
		if (m_AnimationTimer >= m_TimeToSwitchRunSubtexture)
		{
			m_SubtextureIndex++;
			m_AnimationTimer = 0.0f;
		}

		// Repeat
		if (m_SubtextureIndex > m_NumberOfRunSprites - 1)
			m_SubtextureIndex = 0;

		// Set Subtexture Index
		m_SpriteRenderer->SubtextureCoords = glm::vec2(m_SubtextureIndex, 0);
	}
	else if (m_PlayerState == PlayerState::Jumping)
	{
		// Set Current texture as jump sprite.
		m_SpriteRenderer->Texture = m_JumpSprite;
		m_AnimationTimer += ts.GetSeconds();
		if (m_AnimationTimer >= m_TimeToSwitchJumpSubtexture)
		{
			m_SubtextureIndex++;
			m_AnimationTimer = 0.0f;
		}

		// Repeat
		if (m_SubtextureIndex > m_NumberOfJumpSprites - 1)
			m_SubtextureIndex = 0;

		// Set Subtexture Index
		m_SpriteRenderer->SubtextureCoords = glm::vec2(m_SubtextureIndex, 0);
	}

	// Create Subtexture.
	m_SpriteRenderer->Subtexture = SubTexture2D::CreateFromCoords(m_SpriteRenderer->Texture, m_SpriteRenderer->SubtextureCoords, 
													m_SpriteRenderer->SubtextureCellSize, m_SpriteRenderer->SubtextureSpriteSize);
}