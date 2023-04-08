//
// Created by watermelon0guy on 04.04.23.
//
#pragma once
#include <raylib.h>
#include <vector>
#include "omp.h"
#include <chrono>

#include <immintrin.h>
#include <complex>

Vector2 computerNext(Vector2 current, Vector2 constant) {
    // возводим комплексное число в квадрат
    const float zr = current.x * current.x - current.y * current.y;
    const float zi = 2.0f * current.x * current.y;
    return Vector2(zr + constant.x, zi + constant.y);
}

float mod2(Vector2 z) {
    return z.x * z.x + z.y * z.y;
}

int computeIterations(Vector2 z0, Vector2 constant, int max_iteration) {
    Vector2 zn = z0;
    int iteration = 0;
    while (mod2(zn) < 4.0 && iteration < max_iteration) {
        zn = computerNext(zn, constant);
        iteration++;
    }
    return iteration;
}

//std::complex<__m256d> mod2(std::complex<__m256d> z){
//
//    return z * z;
//}
//
//int computeIterationsVec(std::complex<__m256d> z0, std::complex<__m256d> constant, int max_iteration) {
//    std::complex<__m256d> zn = z0;
//    int iteration = 0;
//    while (mod2(zn) < 4.0 && iteration < max_iteration) {
//        zn = computerNext(zn, constant);
//        iteration++;
//    }
//    return iteration;
//}

/// Обычная отрисовка без распараллеливания
void renderSimple(Image& imageBuffer, Vector2& constant, float scale, Vector2& offset, int iterationCount, int numTreads = 1) {
    // Выводим значение константы
    DrawText((std::to_string(constant.x) + " " + std::to_string(constant.y)).c_str(), 10, 10, 20, LIGHTGRAY);
    // --------------------------
    std::vector<std::vector<int>> arr(imageBuffer.width, std::vector<int>(imageBuffer.height));
    auto begin = std::chrono::steady_clock::now();
    for (int x = 0; x < imageBuffer.width; ++x) {
        for (int y = 0; y < imageBuffer.height; ++y) {
            const float px = float(x - imageBuffer.width / 2) * scale + offset.x;
            const float py = float(y - imageBuffer.height / 2) * scale + offset.y;

            const int iterations = computeIterations({px, py}, constant, iterationCount);
            ImageDrawPixel(&imageBuffer, x, y, ColorFromHSV(0, 0, 1.0f / iterationCount * iterations));
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    // Выводим время работы подсчета фрактала
    DrawText(("Calc time " + std::to_string(elapsed_ms.count()) + " ms").c_str(), 10, 30, 20, LIGHTGRAY);
    // --------------------------------------
    DrawText((std::to_string(iterationCount) + " iterations").c_str(), 10, 70, 20, LIGHTGRAY);
    DrawText(("Scale = " + std::to_string(scale)).c_str(), 10, 90, 20, LIGHTGRAY);
}

/// Функция отрисовки с распараллеливанием
void render_p(Image& imageBuffer, Vector2& constant, float scale, Vector2& offset, int iterationCount, int numTreads = 1) {
    // Выводим значение константы
    DrawText((std::to_string(constant.x) + " " + std::to_string(constant.y)).c_str(), 10, 10, 20, LIGHTGRAY);
    // --------------------------
    std::vector<std::vector<int>> arr(imageBuffer.width, std::vector<int>(imageBuffer.height));
    auto begin = std::chrono::steady_clock::now();
    #pragma omp parallel shared(arr, scale, offset, constant, imageBuffer, iterationCount) num_threads(numTreads) default(none)
    {
        # pragma omp for schedule(guided) nowait
        for (int x = 0; x < imageBuffer.width; ++x) {
            for (int y = 0; y < imageBuffer.height; ++y) {
                const float px = float(x - imageBuffer.width / 2) * scale + offset.x;
                const float py = float(y - imageBuffer.height / 2) * scale + offset.y;

                const int iterations = computeIterations({px, py}, constant, iterationCount);
                ImageDrawPixel(&imageBuffer, x, y, ColorFromHSV(0, 0, 1.0f / iterationCount * iterations));
            }
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    // Выводим время работы подсчета и отрисовки фрактала
    DrawText(("Calc time " + std::to_string(elapsed_ms.count()) + " ms").c_str(), 10, 30, 20, LIGHTGRAY);
    // --------------------------------------
    DrawText((std::to_string(iterationCount) + " iterations").c_str(), 10, 50, 20, LIGHTGRAY);
    DrawText(("Scale = " + std::to_string(scale)).c_str(), 10, 70, 20, LIGHTGRAY);
}

void renderParallelVectorized() {
    // TODO Векторизировать функцию рендера
}