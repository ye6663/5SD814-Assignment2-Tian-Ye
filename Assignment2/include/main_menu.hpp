// main_menu.hpp
#pragma once
#include "raylib.h"
#include <functional>

class MainMenu {
public:
    void initialize();
    void update();
    void render();
    void setStartCallback(std::function<void()> callback) { m_startCallback = callback; }
    void setExitCallback(std::function<void()> callback) { m_exitCallback = callback; }

private:
    std::function<void()> m_startCallback;
    std::function<void()> m_exitCallback;

    Texture2D m_background;
    Font m_font;

    Rectangle m_startButton;
    Rectangle m_exitButton;

    bool m_startButtonHovered = false;
    bool m_exitButtonHovered = false;
};