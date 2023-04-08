#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <string>
#include "fractal/Fractal.hpp"
#include <chrono>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION 330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION 100
#endif

int main() {
    const int screenWidth = 1600;
    const int screenHeight = 900;
    Image imageBuffer = GenImageColor(screenWidth, screenHeight, BLACK);


    // (0, -0.79) (-0.23, -0.79) (0.28,0.0113)
    Vector2 constant(-0.016, -0.665002);
    Vector2 offset(0,0);
    Vector2 constantSpeed(0.001, 0.001);
    float scale = 0.003f;
    const float offsetAmount = 20.0f;
    const float zoomSpeed = 1.01f;
    int iterationCount = 100;
    const int numThreads = omp_get_num_procs();

    // Создаем окно
    InitWindow(screenWidth, screenHeight, "Фрактал");
    // Инициализируем текстуру
    Texture displayTexture = LoadTextureFromImage(imageBuffer);

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
//        if (!GetKeyPressed()) continue;
        // Обновляем смещение
        if (IsKeyDown(KEY_RIGHT)) offset.x += offsetAmount * scale;
        if (IsKeyDown(KEY_LEFT)) offset.x -= offsetAmount * scale;
        if (IsKeyDown(KEY_UP)) offset.y -= offsetAmount * scale;
        if (IsKeyDown(KEY_DOWN)) offset.y += offsetAmount * scale;
        // -----------------------------------------------------------

        // Приближение
        if (IsKeyDown(KEY_EQUAL)) scale /= zoomSpeed;
        if (IsKeyDown(KEY_MINUS)) scale *= zoomSpeed;
        // -----------------------------------------------------------

        // Изменение константы
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_RIGHT_BRACKET))
            constant.x += constantSpeed.x;
        if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyDown(KEY_LEFT_BRACKET))
            constant.x -= constantSpeed.x;

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_RIGHT_BRACKET))
            constant.y += constantSpeed.y;
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_BRACKET))
            constant.y -= constantSpeed.y;
        // -----------------------------------------------------------
        // Изменение количества итераций
        if (IsKeyDown(KEY_SEMICOLON)) iterationCount += 1;
        if (IsKeyDown(KEY_APOSTROPHE)) iterationCount -= 1;
        // -----------------------------------------------------------

        UpdateTexture(displayTexture, imageBuffer.data);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(displayTexture, 0, 0, WHITE);

        render_p(imageBuffer, constant, scale, offset, iterationCount, 11);
//        renderSimple(imageBuffer, constant, scale, offset, iterationCount);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
