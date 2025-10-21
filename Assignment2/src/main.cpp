// main.cpp

#include "application.hpp"
#include <raylib.h>

int main(int argc, char** argv)
{
    // Set window size
    int width = 1280;
    int height = 720;
    InitWindow(width, height, "Asteroid Field Renderer");
    SetTargetFPS(60);

    Application app;
    if (!app.initialize(width, height))
    {
        CloseWindow();
        return -1;
    }

    // Main loop
    while (!WindowShouldClose())
    {
        app.update();

        BeginDrawing();
        ClearBackground(BLACK);
        app.render();
        EndDrawing();
    }

    app.shutdown();
    CloseWindow();

    return 0;
}