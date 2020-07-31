#pragma once
#include "Object/Actor.h"

namespace nc
{
	class Asteroid : public nc::Actor
	{
	public:
		Asteroid() {}

		virtual eType GetType() override { return eType::ASTEROID; }

		virtual bool Load(const std::string& filename) override;
		virtual void Update(float dt) override;

		virtual void OnCollision(Actor* actor) override;

		void SetSpeed(float speed) { m_speed = speed; }

	protected:
		float m_speed{ 50.0f };
		float m_thrust{ 0.0f };
	};
}