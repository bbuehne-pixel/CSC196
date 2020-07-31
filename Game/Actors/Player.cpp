#include "Player.h"
#include "Enemy.h"
#include "Math/MyOwnMath.h"
#include "Math/Random.h"
#include "Projectile.h"
#include "Object/Scene.h"
#include "Graphics/ParticleSystem.h"
#include "Audio/AudioSystem.h"
#include "../Game.h"
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
        }
        return success;
    }

    void Player::Update(float dt)
    {
        m_fireTimer += dt;
        if (Core::Input::IsPressed(VK_SPACE) && m_fireTimer >= m_fireRate)
        {
            m_fireTimer = 0;

            g_audioSystem.PlayAudio("PlayerShot");

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

        // rotation
        float torque{ 0 };
        if (Core::Input::IsPressed('A')) torque = -20.0f;
        if (Core::Input::IsPressed('D')) torque = 20.0f;

        m_angularVelocity = m_angularVelocity + (torque * dt);
        m_angularVelocity = m_angularVelocity * 0.95f;
        m_transform.angle = m_transform.angle + (m_angularVelocity * dt);

        if (force.LengthSqr() > 0)
        {
            Actor* child = m_children[0];
            g_particleSystem.Create(child->GetTransform().matrix.GetPosition(), child->GetTransform().matrix.GetAngle() + nc::PI, 10, 1, 1, nc::Color{ 1, 1, 0 }, 100, 200);

            child = m_children[1];
            g_particleSystem.Create(child->GetTransform().matrix.GetPosition(), child->GetTransform().matrix.GetAngle() + nc::PI, 10, 1, 1, nc::Color{ 1, 1, 0 }, 100, 200);
        }

        if (Core::Input::IsPressed('E') && !m_prevButtonPress)
        {
            m_transform.position = nc::Vector2{ nc::random(0, 800), nc::random(0, 600) };
            m_transform.angle = nc::random(0, nc::TWO_PI);
        }
        m_prevButtonPress = Core::Input::IsPressed('E');

        m_transform.Update();

        // update children
        for(auto child : m_children)
        {
            child->Update(dt);
        }
    }

    void Player::OnCollision(Actor* actor)
    {
        if (!m_destroy && actor->GetType() == eType::ENEMY)
        {
            m_destroy = true;
            m_scene->GetGame()->SetState(Game::eState::PLAYER_DEAD);

            g_audioSystem.PlayAudio("PlayerExplosion");

            // set target null
            auto enemies = m_scene->GetActors<Enemy>();
            for (auto enemy : enemies)
            {
                enemy->SetTarget(nullptr);
            }

            nc::Color colors[] = { {1,1,1}, nc::Color::red, {1,1,0}, {0,1,1} };
            nc::Color color = colors[rand() % 4];
            g_particleSystem.Create(m_transform.position, 0, 180, 100, 2, color, 100, 200);
        }
    }
}