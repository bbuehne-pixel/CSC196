#pragma once
#include "Object/Actor.h"

namespace nc
{
	class Player : public nc::Actor
	{
	public:
		Player() {}

		virtual eType GetType() override { return eType::PLAYER; }

		virtual bool Load(const std::string& filename) override;
		virtual void Update(float dt) override;

	protected:
		float m_thrust{200.0f};
		float m_rotation{ 0 };

		float m_fireRate{ 0.2f };
		float m_fireTimer{ 0 };
		nc::Vector2 m_velocity;
	};
}