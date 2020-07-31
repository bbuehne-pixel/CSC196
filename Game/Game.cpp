#include "Game.h"
#include "Math/MyOwnMath.h"
#include "Math/Random.h"
#include "Math/Transform.h"
#include "Graphics/Shape.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"
#include "Actors/Locator.h"
#include "Actors/Asteroid.h"
#include "Object/Scene.h"
#include "Graphics/ParticleSystem.h"
#include "Audio/AudioSystem.h"
#include <iostream>
#include <string>
#include <array>

void Game::Initialize()
{
    m_scene.Startup();
    m_scene.SetGame(this);

    g_audioSystem.AddAudio("PlayerShot", "Laser_Shoot2.wav");
    g_audioSystem.AddAudio("PlayerExplosion", "Explosion.wav");
    g_audioSystem.AddAudio("EnemyExplosion", "Explosion2.wav");
    g_audioSystem.AddAudio("AsteroidExplosion", "Explosion9.wav");
    g_audioSystem.AddAudio("SpawnSound", "Blip_Select10.wav");
    g_audioSystem.AddAudio("UfoShot", "Laser_Shoot3.wav");
}

bool Game::Update(float dt)
{
    m_frametime = dt;
    bool quit = Core::Input::IsPressed(Core::Input::KEY_ESCAPE);

    switch (m_state)
    {
    case Game::eState::TITLE:
        if (Core::Input::IsPressed(VK_SPACE))
        {
            m_state = eState::START_GAME;
        }
        break;
    case::Game::eState::INIT_GAME:
        m_score = 0;
        m_lives = 3;
        m_state = eState::START_GAME;
        break;
    case Game::eState::START_GAME:
    {
        nc::Player* player = new nc::Player;
        player->Load("player.txt");
        m_scene.AddActor(player);

        Locator* locator = new Locator;
        locator->GetTransform().position = nc::Vector2{ 5, 2 };
        player->AddChild(locator);

        locator = new Locator;
        locator->GetTransform().position = nc::Vector2{ -5, 2 };
        player->AddChild(locator);

        for (size_t i = 0; i < 3; i++)
        {
            nc::Enemy* enemy = new nc::Enemy;
            enemy->Load("enemy.txt");
            enemy->SetTarget(m_scene.GetActor<nc::Player>());
            float distance = nc::random(300, 600);
            float angle = nc::random(0, nc::TWO_PI);
            nc::Vector2 position = nc::Vector2::Rotate({ 0.0f, distance }, angle);
            enemy->GetTransform().position = m_scene.GetActor<nc::Player>()->GetTransform().position + position;
            m_scene.AddActor(enemy);
        }

        for (size_t i = 0; i < 2; i++)
        {
            nc::Asteroid* asteroid = new nc::Asteroid;
            asteroid->Load("asteroid.txt");
            float distance = nc::random(300, 600);
            float angle = nc::random(0, nc::TWO_PI);
            nc::Vector2 position = nc::Vector2::Rotate({ 0.0f, distance }, angle);
            asteroid->GetTransform().position = m_scene.GetActor<nc::Player>()->GetTransform().position + position;
            m_scene.AddActor(asteroid);
        }

        for (size_t i = 0; i < 1; i++)
        {
            nc::Enemy* bigenemy = new nc::Enemy;
            bigenemy->Load("bigenemy.txt");
            bigenemy->SetTarget(m_scene.GetActor<nc::Player>());
            float distance = nc::random(300, 600);
            float angle = nc::random(0, nc::TWO_PI);
            nc::Vector2 position = nc::Vector2::Rotate({ 0.0f, distance }, angle);
            bigenemy->GetTransform().position = m_scene.GetActor<nc::Player>()->GetTransform().position + position;
            m_scene.AddActor(bigenemy);
        }

        m_state = eState::GAME;
    }
        break;
    case Game::eState::GAME:
        m_spawnTimer += dt;
        if (m_spawnTimer >= 3.0f)
        {
            m_spawnTimer = 0;

            nc::Enemy* enemy = new nc::Enemy;
            enemy->Load("enemy.txt");
            enemy->SetTarget(m_scene.GetActor<nc::Player>());
            float distance = nc::random(200, 400);
            float angle = nc::random(0, nc::TWO_PI);
            nc::Vector2 position = nc::Vector2::Rotate({ 0.0f, distance }, angle);
            enemy->GetTransform().position = m_scene.GetActor<nc::Player>()->GetTransform().position + position;
            m_scene.AddActor(enemy);

            nc::Asteroid* asteroid = new nc::Asteroid;
            asteroid->Load("asteroid.txt");
            asteroid->GetTransform().position = m_scene.GetActor<nc::Player>()->GetTransform().position + position;
            m_scene.AddActor(asteroid);

            nc::Enemy* bigenemy = new nc::Enemy;
            bigenemy->Load("bigenemy.txt");
            bigenemy->SetTarget(m_scene.GetActor<nc::Player>());
            bigenemy->GetTransform().position = m_scene.GetActor<nc::Player>()->GetTransform().position + position;
            m_scene.AddActor(bigenemy);
        }
        if (m_score > m_highScore) m_highScore = m_score;

        break;
    case Game::eState::PLAYER_DEAD:
        m_lives = m_lives - 1;
        m_state = (m_lives == 0) ? eState::GAME_OVER : eState::GAME_WAIT;
        m_stateTimer = 3;
        break;
    case Game::eState::GAME_WAIT:
        m_stateTimer -= dt;
        if (m_stateTimer <= 0)
        {
            m_scene.RemoveAllActors();
            m_state = eState::START_GAME;
        }
        break;
    case Game::eState::GAME_OVER:
        m_stateTimer -= dt;
        if (m_stateTimer <= 0)
        {
            m_scene.RemoveAllActors();
            m_state = eState::TITLE;
        }
        break;
    default:
        break;
    }

    m_scene.Update(dt);

    g_particleSystem.Update(dt);

    g_audioSystem.Update(dt);

    return quit;
}

void Game::Draw(Core::Graphics& graphics)
{
    graphics.SetColor(nc::Color{ 1, 1, 1 });
    graphics.DrawString(10, 10, std::to_string(m_frametime).c_str());
    graphics.DrawString(10, 20, std::to_string(1.0f / m_frametime).c_str());

    switch (m_state)
    {
    case Game::eState::TITLE:
        graphics.DrawString(400, 300, "VECTREX");
        break;
    case Game::eState::START_GAME:
        break;
    case Game::eState::GAME:
    {
        
    }
        break;
    case Game::eState::GAME_OVER:
        graphics.DrawString(400, 300, "GAME OVER");
        break;
    default:
        break;
    }

    graphics.SetColor(nc::Color{ 1, 1, 1 });

    std::string score = "SCORE: " + std::to_string(m_score);
    graphics.DrawString(700, 10, score.c_str());
    std::string lives = "LIVES: " + std::to_string(m_lives);
    graphics.DrawString(700, 20, lives.c_str());
    score = "HIGH SCORE: " + std::to_string(m_highScore);
    graphics.DrawString(350, 10, score.c_str());

    m_scene.Draw(graphics);
    g_particleSystem.Draw(graphics);
}