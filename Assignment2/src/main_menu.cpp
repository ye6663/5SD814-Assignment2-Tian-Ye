// main_menu.cpp

#include "main_menu.hpp"
#include "math_utils.hpp"
#include <iostream>

void MainMenu::initialize() {
    // Load background image
    m_background = LoadTexture("assets/menu_background.png");
    m_background2 = LoadTexture("assets/menu_background2.png");

    // Loading fonts
    m_font = GetFontDefault();

    // Initialization button position
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    m_startButton = {
        (float)screenWidth / 2 - 100,
        (float)screenHeight / 2 - 25,
        200.0f,
        50.0f
    };

    m_exitButton = {
        (float)screenWidth / 2 - 100,
        (float)screenHeight / 2 + 50,
        200.0f,
        50.0f
    };
}

void MainMenu::update() {
    Vector2 mousePos = GetMousePosition();

    // Check the hover status of the button
    m_startButtonHovered = CheckCollisionPointRec(mousePos, m_startButton);
    m_exitButtonHovered = CheckCollisionPointRec(mousePos, m_exitButton);

    // Check button click
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (m_startButtonHovered && m_startCallback) {
            m_startCallback();
        }
        if (m_exitButtonHovered && m_exitCallback) {
            m_exitCallback();
        }
    }
}

void MainMenu::render() {
    // Draw Background
    ClearBackground(BLACK);

    if ((m_showBackgroundSelect % 2) == 0) {
        float scaleX = (float)GetScreenWidth() / m_background.width;
        float scaleY = (float)GetScreenHeight() / m_background.height;
        float scale = (scaleX > scaleY) ? scaleX : scaleY;
        float scaledWidth = m_background.width * scale;
        float scaledHeight = m_background.height * scale;
        float posX = (GetScreenWidth() - scaledWidth) / 2;
        float posY = (GetScreenHeight() - scaledHeight) / 2;
        DrawTextureEx(m_background, { posX, posY }, 0.0f, scale, WHITE);
    }
    else {
        float scaleX = (float)GetScreenWidth() / m_background2.width;
        float scaleY = (float)GetScreenHeight() / m_background2.height;
        float scale = (scaleX > scaleY) ? scaleX : scaleY;
        float scaledWidth = m_background2.width * scale;
        float scaledHeight = m_background2.height * scale;
        float posX = (GetScreenWidth() - scaledWidth) / 2;
        float posY = (GetScreenHeight() - scaledHeight) / 2;
        DrawTextureEx(m_background2, { posX, posY }, 0.0f, scale, WHITE);
    }
    // DrawTexture(m_background, 0, 0, WHITE);


    // Draw Title
    const char* title = "ASTEROID FIELD";
    int titleWidth = MeasureText(title, 40);
    DrawText(title, GetScreenWidth() / 2 - titleWidth / 2, 100, 40, WHITE);

    // Draw Start Button
    Color startButtonColor = m_startButtonHovered ? GREEN : DARKGREEN;
    DrawRectangleRec(m_startButton, startButtonColor);
    DrawRectangleLinesEx(m_startButton, 2, WHITE);

    const char* startText = "START GAME";
    int startTextWidth = MeasureText(startText, 20);
    DrawText(startText,
        (int)(m_startButton.x + m_startButton.width / 2 - startTextWidth / 2),
        (int)(m_startButton.y + m_startButton.height / 2 - 10),
        20, WHITE);

    // Draw exit button
    Color exitButtonColor = m_exitButtonHovered ? RED : GRAY;
    DrawRectangleRec(m_exitButton, exitButtonColor);
    DrawRectangleLinesEx(m_exitButton, 2, WHITE);

    const char* exitText = "EXIT";
    int exitTextWidth = MeasureText(exitText, 20);
    DrawText(exitText,
        (int)(m_exitButton.x + m_exitButton.width / 2 - exitTextWidth / 2),
        (int)(m_exitButton.y + m_exitButton.height / 2 - 10),
        20, WHITE);
}

void MainMenu::updateConfig() {
    m_showBackgroundSelect++;
}