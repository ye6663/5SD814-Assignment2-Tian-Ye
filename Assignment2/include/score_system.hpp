// score_system.hpp

#pragma once
#include "event_system.hpp"
#include "event_types.hpp"
#include <iostream>

class ScoreSystem {
public:
    ScoreSystem() : m_score(0) {}

    void initialize() {
        EventSystem::getInstance().subscribe(EventType::AsteroidDestroyed, [this](const std::any& data) { onAsteroidDestroyed(data); });

        std::cout << "ScoreSystem initialized" << std::endl;
    }

    int getScore() const { return m_score; }
    void reset() { m_score = 0; }

private:
    void onAsteroidDestroyed(const std::any& data) {
        try {
            auto destroyedData = std::any_cast<AsteroidDestroyedData>(data);
            m_score += destroyedData.scoreValue;

            // std::cout << "Score: +" << destroyedData.scoreValue << " (Total: " << m_score << ")" << std::endl;

            // Publish score update event
            EventSystem::getInstance().publish(EventType::ScoreChanged,
                ScoreChangedData(m_score, destroyedData.scoreValue));
        }
        catch (const std::bad_any_cast&) {
            std::cout << "ScoreSystem: Invalid asteroid destroyed data" << std::endl;
        }
    }

    int m_score;
};