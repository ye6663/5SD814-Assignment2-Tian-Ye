// game_over_screen.cpp

#include "game_over_screen.hpp"
#include <iostream>

void GameOverScreen::initialize() {
    // Load background image
    // m_background = LoadTexture("assets/game_over_background.png");

    // Initialization button position
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    m_menuButton = {
        (float)screenWidth / 2 - 150,
        (float)screenHeight / 2 + 50,
        300.0f,
        50.0f
    };

    m_restartButton = {
        (float)screenWidth / 2 - 150,
        (float)screenHeight / 2 + 120,
        300.0f,
        50.0f
    };
}

void GameOverScreen::update() {
    Vector2 mousePos = GetMousePosition();

    // Check the hover status of the button
    m_menuButtonHovered = CheckCollisionPointRec(mousePos, m_menuButton);
    m_restartButtonHovered = CheckCollisionPointRec(mousePos, m_restartButton);

    // Check button click
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (m_menuButtonHovered && m_returnToMenuCallback) {
            m_returnToMenuCallback();
        }
        if (m_restartButtonHovered && m_restartCallback) {
            m_restartCallback();
        }
    }
}

void GameOverScreen::render() {
    // Draw a semi transparent black background
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));

    // Draw the game ending title
    const char* title = "GAME OVER";
    int titleWidth = MeasureText(title, 50);
    DrawText(title, GetScreenWidth() / 2 - titleWidth / 2, 150, 50, RED);

    // Draw Score
    std::string scoreText = "FINAL SCORE: " + std::to_string(m_finalScore);
    int scoreWidth = MeasureText(scoreText.c_str(), 30);
    DrawText(scoreText.c_str(), GetScreenWidth() / 2 - scoreWidth / 2, 250, 30, YELLOW);

    // Draw the return menu button
    Color menuButtonColor = m_menuButtonHovered ? BLUE : DARKBLUE;
    DrawRectangleRec(m_menuButton, menuButtonColor);
    DrawRectangleLinesEx(m_menuButton, 2, WHITE);

    const char* menuText = "RETURN TO MAIN MENU";
    int menuTextWidth = MeasureText(menuText, 20);
    DrawText(menuText,
        (int)(m_menuButton.x + m_menuButton.width / 2 - menuTextWidth / 2),
        (int)(m_menuButton.y + m_menuButton.height / 2 - 10),
        20, WHITE);

    // Draw the restart button
    Color restartButtonColor = m_restartButtonHovered ? GREEN : DARKGREEN;
    DrawRectangleRec(m_restartButton, restartButtonColor);
    DrawRectangleLinesEx(m_restartButton, 2, WHITE);

    const char* restartText = "RESTART GAME";
    int restartTextWidth = MeasureText(restartText, 20);
    DrawText(restartText,
        (int)(m_restartButton.x + m_restartButton.width / 2 - restartTextWidth / 2),
            (int)(m_restartButton.y + m_restartButton.height / 2 - 10),
        20, WHITE);
}