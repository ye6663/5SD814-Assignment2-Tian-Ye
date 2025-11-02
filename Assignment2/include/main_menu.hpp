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
    void updateConfig();

private:
    std::function<void()> m_startCallback;
    std::function<void()> m_exitCallback;

    int m_showBackgroundSelect = 0;
    Texture2D m_background;
    Texture2D m_background2;
    Font m_font;

    Rectangle m_startButton;
    Rectangle m_exitButton;

    bool m_startButtonHovered = false;
    bool m_exitButtonHovered = false;
};