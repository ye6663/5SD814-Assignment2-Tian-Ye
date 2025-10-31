// game_over_screen.hpp
#pragma once
#include "raylib.h"
#include <functional>
#include <string>

class GameOverScreen {
public:
    void initialize();
    void update();
    void render();
    void setFinalScore(int score) { m_finalScore = score; }
    void setReturnToMenuCallback(std::function<void()> callback) { m_returnToMenuCallback = callback; }
    void setRestartCallback(std::function<void()> callback) { m_restartCallback = callback; }

private:
    std::function<void()> m_returnToMenuCallback;
    std::function<void()> m_restartCallback;

    int m_finalScore = 0;

    Rectangle m_menuButton;
    Rectangle m_restartButton;

    bool m_menuButtonHovered = false;
    bool m_restartButtonHovered = false;

    Texture2D m_background;
};