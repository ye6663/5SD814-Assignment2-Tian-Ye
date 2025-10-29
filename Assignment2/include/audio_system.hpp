// audio_system.hpp
#pragma once
#include "event_system.hpp"
#include "event_types.hpp"
#include <raylib.h>
#include <iostream>

class AudioSystem {
public:
    void initialize() {
        // 初始化音频设备
        InitAudioDevice();

        // 加载声音文件
        loadSounds();

        // 订阅相关事件
        EventSystem::getInstance().subscribe(EventType::Collision,
            [this](const std::any& data) { onCollision(data); });

        EventSystem::getInstance().subscribe(EventType::PlayerShoot,
            [this](const std::any& data) { onPlayerShoot(data); });

        EventSystem::getInstance().subscribe(EventType::AsteroidDestroyed,
            [this](const std::any& data) { onAsteroidDestroyed(data); });

        EventSystem::getInstance().subscribe(EventType::PlayerHit,
            [this](const std::any& data) { onPlayerHit(data); });

        std::cout << "AudioSystem initialized" << std::endl;
    }

    void shutdown() {
        // 卸载所有声音
        for (auto& sound : m_sounds) {
            UnloadSound(sound.second);
        }
        m_sounds.clear();

        // 关闭音频设备
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
        // 尝试加载各种音效
        loadSound("shoot", "assets/shoot.wav");
        loadSound("asteroid_explosion", "assets/asteroid_explosion.wav");
        loadSound("player_hit", "assets/player_hit.wav");
        //loadSound("bullet_hit", "assets/bullet_hit.wav");

        // 设置默认音量
        setSoundVolume("shoot", 0.7f);
        setSoundVolume("asteroid_explosion", 0.8f);
        setSoundVolume("player_hit", 0.9f);
        //setSoundVolume("bullet_hit", 0.5f);
    }

    void loadSound(const std::string& name, const std::string& filepath) {
        if (FileExists(filepath.c_str())) {
            Sound sound = LoadSound(filepath.c_str());
            m_sounds[name] = sound;
            std::cout << "Loaded sound: " << name << " from " << filepath << std::endl;
        }
        else {
            std::cout << "Warning: Sound file not found: " << filepath << std::endl;
            // 你可以在这里创建默认声音或使用备用方案
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

            // 根据碰撞类型播放不同音效
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

    void onPlayerHit(const std::any& data) {
        playSound("player_hit");
    }

    std::unordered_map<std::string, Sound> m_sounds;
};