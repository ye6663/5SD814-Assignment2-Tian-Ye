// main_menu.cpp
#include "main_menu.hpp"
#include <iostream>

void MainMenu::initialize() {
    // 加载背景图片
    // m_background = LoadTexture("assets/menu_background.png");

    // 加载字体
    m_font = GetFontDefault();

    // 初始化按钮位置
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

    // 检查按钮悬停状态
    m_startButtonHovered = CheckCollisionPointRec(mousePos, m_startButton);
    m_exitButtonHovered = CheckCollisionPointRec(mousePos, m_exitButton);

    // 检查按钮点击
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
    // 绘制背景
    ClearBackground(BLACK);

    // 如果有背景纹理
    // DrawTexture(m_background, 0, 0, WHITE);

    // 绘制标题
    const char* title = "ASTEROID FIELD";
    int titleWidth = MeasureText(title, 40);
    DrawText(title, GetScreenWidth() / 2 - titleWidth / 2, 100, 40, WHITE);

    // 绘制开始按钮
    Color startButtonColor = m_startButtonHovered ? GREEN : DARKGREEN;
    DrawRectangleRec(m_startButton, startButtonColor);
    DrawRectangleLinesEx(m_startButton, 2, WHITE);

    const char* startText = "START GAME";
    int startTextWidth = MeasureText(startText, 20);
    DrawText(startText,
        (int)(m_startButton.x + m_startButton.width / 2 - startTextWidth / 2),
        (int)(m_startButton.y + m_startButton.height / 2 - 10),
        20, WHITE);

    // 绘制退出按钮
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