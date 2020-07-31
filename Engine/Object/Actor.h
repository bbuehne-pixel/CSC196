#pragma once
#include "Math/Transform.h"
#include "Graphics/Shape.h"
#include <string>

namespace nc
{

	class Scene; // forward declaration

	class Actor
	{

	public:
		enum class eType
		{
			PLAYER,
			ENEMY,
			UFO,
			PROJECTILE,
			LOCATOR,
			ASTEROID,
		};

	public:
		Actor() {}
		Actor(const Transform& transform, const Shape& shape) {}
		~Actor() {}

		virtual void Destroy();

		virtual eType GetType() = 0;

		virtual bool Load(const std::string& filename);
		virtual void Load(std::istream& stream);

		virtual void Update(float dt);
		void Draw(Core::Graphics& graphics);

		virtual void OnCollision(Actor* actor) {}

		float GetRadius();

		bool IsDestroy() { return m_destroy; }
		void SetScene(Scene* scene) { m_scene = scene; }
		Transform& GetTransform() { return m_transform; }
		Shape& GetShape() { return m_shape; }

		Actor* GetParent() { return m_parent; }
		void AddChild(Actor* child);

	protected:
		bool m_destroy{ false };
		Scene* m_scene{ nullptr };
		Transform m_transform;
		Shape m_shape;

		Actor* m_parent{ nullptr };
		std::vector<Actor*> m_children;
	};
}
