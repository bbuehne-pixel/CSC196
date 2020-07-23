#include "Player.h"
#include "Math/MyOwnMath.h"
#include "Projectile.h"
#include "Object/Scene.h"
#include "Graphics/ParticleSystem.h"
#include <fstream>

namespace nc
{
    bool Player::Load(const std::string& filename)
    {
        bool success = false;

        std::ifstream stream(filename);
        if (stream.is_open())
        {
            Actor::Load(stream);

            std::string line;

            //std::getline(stream, line);
            //m_thrust = stof(line);

            //std::getline(stream, line);
            //m_rotation = stof(line);
        }
        return success;
    }

    void Player::Update(float dt)
    {
        m_fireTimer += dt;
        if (Core::Input::IsPressed(VK_SPACE) && m_fireTimer >= m_fireRate)
        {
            m_fireTimer = 0;

            nc::Projectile* projectile = new nc::Projectile;
            projectile->Load("projectile.txt");
            projectile->GetTransform().position = m_transform.position;
            projectile->GetTransform().angle = m_transform.angle;
            m_scene->AddActor(projectile);
        }


        // get force
        nc::Vector2 force{ 0, 0 };
        if (Core::Input::IsPressed('W')) { force = nc::Vector2::forward * m_thrust; }
        // point force in direction of ship
        force = nc::Vector2::Rotate(force, m_transform.angle);

        // add force to veloicty
        m_velocity = m_velocity + (force * dt);
        m_velocity = m_velocity * 0.98f;
        // set position with velocity
        m_transform.position = m_transform.position + (m_velocity * dt);

        if (m_transform.position.x > 800) m_transform.position.x = 0;
        if (m_transform.position.x < 0) m_transform.position.x = 800;
        if (m_transform.position.y > 600) m_transform.position.y = 0;
        if (m_transform.position.y < 0) m_transform.position.y = 600;

        if (Core::Input::IsPressed('A')) m_transform.angle -= dt * nc::DegreesToRadians(360.0f);
        if (Core::Input::IsPressed('D')) m_transform.angle += dt * nc::DegreesToRadians(360.0f);

        if (force.LengthSqr() > 0)
        {
            g_particleSystem.Create(m_transform.position, m_transform.angle + nc::PI, 10, 1, 1, nc::Color{ 1, 1, 0 }, 100, 200);
        }

        m_transform.Update();
    }
}