#include "Projectile.h"
#include "Math/MyOwnMath.h"
#include "Graphics/ParticleSystem.h"
#include <fstream>

namespace nc
{
    bool Projectile::Load(const std::string& filename)
    {
        bool success = false;

        std::ifstream stream(filename);
        if (stream.is_open())
        {
            success = true;

            stream >> m_transform;

            std::string shapename;
            std::getline(stream, shapename);
            m_shape.Load(shapename);
        }

        return success;
    }

    void Projectile::Update(float dt)
    {
        m_lifetime -= dt;
        if (m_lifetime <= 0)
        {
            m_destroy = true;
        }

        nc::Vector2 direction = nc::Vector2::Rotate(nc::Vector2::forward, m_transform.angle);
        nc::Vector2 projectileVelocity = direction * m_speed;
        m_transform.position = m_transform.position + (projectileVelocity * dt);

        g_particleSystem.Create(m_transform.position, m_transform.angle + nc::PI, 10, 1, 1, nc::Color{ 1, 1, 0 }, 100, 200);

        m_transform.Update();
    }

    void Projectile::OnCollision(Actor* actor)
    {
        if (actor->GetType() == eType::ENEMY)
        {
            m_destroy = true;
        }
    }
}