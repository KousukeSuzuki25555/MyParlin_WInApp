#pragma once

#include "resource.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <array>
#include <fstream>

// パーリンノイズ関数の前方宣言
float perlinNoise(float x, float y);

// フェード関数の前方宣言
float fade(float t);

// 線形補間関数の前方宣言
float lerp(float t, float a, float b);

// グラデーション関数の前方宣言
float grad(int hash, float x, float y);

// パーリンノイズに使用するランダムな値の配列
int p[512];

// パーリンノイズ関数
float perlinNoise(float x, float y) {
    int X = static_cast<int>(floor(x)) & 255;
    int Y = static_cast<int>(floor(y)) & 255;
    x -= floor(x);
    y -= floor(y);
    float u = fade(x);
    float v = fade(y);

    int A = p[X] + Y;
    int B = p[X + 1] + Y;

    return lerp(v, lerp(u, grad(p[A], x, y), grad(p[B], x - 1, y)), lerp(u, grad(p[A + 1], x, y - 1), grad(p[B + 1], x - 1, y - 1)));
}

// フェード関数
float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// 線形補間関数
float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

// グラデーション関数
float grad(int hash, float x, float y) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14) ? x : 0;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

void Act(int sizeX, int sizeY, float frequency) {

}