// audio_system.hpp

#pragma once
#include "event_system.hpp"
#include "event_types.hpp"
#include <raylib.h>
#include <iostream>

class AudioSystem {
public:
    void initialize() {
        // Initialize audio device
        InitAudioDevice();

        // Load sound file
        loadSounds();

        // Subscription related events
        EventSystem::getInstance().subscribe(EventType::Collision,
            [this](const std::any& data) { onCollision(data); });

        EventSystem::getInstance().subscribe(EventType::PlayerShoot,
            [this](const std::any& data) { onPlayerShoot(data); });

        EventSystem::getInstance().subscribe(EventType::AsteroidDestroyed,
            [this](const std::any& data) { onAsteroidDestroyed(data); });

        EventSystem::getInstance().subscribe(EventType::CollisionAsteroid,
            [this](const std::any& data) { onCollisionAsteroid(data); });

        EventSystem::getInstance().subscribe(EventType::HyperspaceJump,
            [this](const std::any& data) { onHyperspaceJump(data); });

        EventSystem::getInstance().subscribe(EventType::LaserFired,
            [this](const std::any& data) { onLaserFired(data); });

        EventSystem::getInstance().subscribe(EventType::GameOver,
            [this](const std::any& data) { onGameOver(data); });

        std::cout << "AudioSystem initialized" << std::endl;
    }

    void shutdown() {
        // Uninstall all sounds
        for (auto& sound : m_sounds) {
            UnloadSound(sound.second);
        }
        m_sounds.clear();

        // Turn off audio devices
        CloseAudioDevice();
    }

    void setMasterVolume(float volume) {
        SetMasterVolume(volume);
    }

    void setSoundVolume(const std::string& soundName, float volume) {
        auto it = m_sounds.find(soundName);
        if (it != m_sounds.end()) {
            SetSoundVolume(it->second, volume);
        }
    }

private:
    void loadSounds() {
        loadSound("shoot", "assets/shoot.wav");
        loadSound("asteroid_explosion", "assets/asteroid_explosion.wav");
        loadSound("collision_asteroid", "assets/collision_asteroid.wav");
        loadSound("player_hit", "assets/player_hit.wav");
        loadSound("hyperspace_jump", "assets/hyperspace_jump.wav");
        loadSound("laser", "assets/laser.wav");

        // Set default volume
        setSoundVolume("shoot", 0.7f);
        setSoundVolume("asteroid_explosion", 0.8f);
        setSoundVolume("player_hit", 0.9f);
        setSoundVolume("hyperspace_jump", 3.0f);
        setSoundVolume("laser", 2.0f);
    }

    void loadSound(const std::string& name, const std::string& filepath) {
        if (FileExists(filepath.c_str())) {
            Sound sound = LoadSound(filepath.c_str());
            m_sounds[name] = sound;
            std::cout << "Loaded sound: " << name << " from " << filepath << std::endl;
        }
        else {
            std::cout << "Warning: Sound file not found: " << filepath << std::endl;
        }
    }

    void playSound(const std::string& name) {
        auto it = m_sounds.find(name);
        if (it != m_sounds.end()) {
            PlaySound(it->second);
        }
        else {
            std::cout << "Sound not found: " << name << std::endl;
        }
    }

    void onCollision(const std::any& data) {
        try {
            auto collisionData = std::any_cast<CollisionData>(data);

            // Play different sound effects based on collision type
            if (collisionData.entityA == EntityType::Bullet &&
                collisionData.entityB == EntityType::Asteroid) {
                // std::cout << "Audio: Playing bullet hit asteroid sound" << std::endl;
                // PlaySound(m_bulletHitSound);
            }
            else if (collisionData.entityA == EntityType::Player &&
                collisionData.entityB == EntityType::Asteroid) {
                // std::cout << "Audio: Playing player hit sound" << std::endl;
                // PlaySound(m_playerHitSound);
            }
        }
        catch (const std::bad_any_cast&) {
            std::cout << "AudioSystem: Invalid collision data" << std::endl;
        }
    }

    void onPlayerShoot(const std::any& data) {
        // std::cout << "Audio: Playing shoot sound" << std::endl;
        playSound("shoot");
    }

    void onAsteroidDestroyed(const std::any& data) {
        try {
            auto destroyedData = std::any_cast<AsteroidDestroyedData>(data);
            // std::cout << "Audio: Playing asteroid explosion sound (size: " << destroyedData.asteroidSize << ")" << std::endl;
            playSound("asteroid_explosion");
        }
        catch (const std::bad_any_cast&) {
            std::cout << "AudioSystem: Invalid asteroid destroyed data" << std::endl;
        }
    }

    void onCollisionAsteroid(const std::any& data) {
        // playSound("collision_asteroid");
    }

    void onHyperspaceJump(const std::any& data) {
        playSound("hyperspace_jump");
    }

    void onLaserFired(const std::any& data) {
        playSound("laser");
    }

    void onGameOver(const std::any& data) {
        playSound("player_hit");
    }

    std::unordered_map<std::string, Sound> m_sounds;
};