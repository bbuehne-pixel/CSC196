#pragma once
#include "Object/Actor.h"

namespace nc
{
	class Projectile : public nc::Actor
	{
	public:
		Projectile() {}

		virtual eType GetType() override { return eType::PROJECTILE; }

		virtual bool Load(const std::string& filename) override;
		virtual void Update(float dt) override;

		virtual void OnCollision(Actor* actor) override;

		void SetSpeed(float speed) { m_speed = speed; }

	protected:
		float m_speed{ 300.0f };
		float m_lifetime{ 1 };
	};
}
