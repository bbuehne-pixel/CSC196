#include "Asteroid.h"
#include "Math/MyOwnMath.h"
#include "Graphics/ParticleSystem.h"
#include "Object/Scene.h"
#include "../Game.h"
#include "Audio/AudioSystem.h"
#include <fstream>

namespace nc
{
    bool Asteroid::Load(const std::string& filename)
    {
        bool success = false;

        std::ifstream stream(filename);
        if (stream.is_open())
        {
            success = true;
            stream >> m_transform;

            std::string shapename;
            stream >> shapename;
            m_shape.Load(shapename);

            g_audioSystem.PlayAudio("SpawnSound");
        }

        return success;
    }

    void Asteroid::Update(float dt)
    {
        nc::Vector2 direction = m_transform.position;
        direction.Normalize();
        nc::Vector2 Velocity = direction * 30.0f;// m_speed;
        m_transform.position = m_transform.position + (Velocity * dt);
        m_transform.angle = std::atan2(direction.y, direction.x) + nc::DegreesToRadians(90);

        m_transform.Update();
    }

    void Asteroid::OnCollision(Actor* actor)
    {
        if (actor->GetType() == eType::PROJECTILE)
        {
            m_destroy = true;

            g_audioSystem.PlayAudio("AsteroidExplosion");

            // set game points / score
            m_scene->GetGame()->AddPoints(150);

            nc::Color colors[] = { {1,1,1}, nc::Color::red, {1,1,0}, {0,1,1} };
            nc::Color color = colors[rand() % 4];
            g_particleSystem.Create(m_transform.position, 0, 180, 30, 1, color, 100, 200);
        }
    }
}