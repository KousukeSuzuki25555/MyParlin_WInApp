#pragma once
#include "resource.h"
#include <windows.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <array>
#include <fstream>
#include <vector>

#define RAND_ARRAY_SIZE (512)

class PerlinNoise {
public:
    //コンストラクタ
    PerlinNoise();
    // パーリンノイズ関数の前方宣言
    float perlinNoise(float x, float y);
    // フェード関数の前方宣言
    float fade(float t);
    // 線形補間関数の前方宣言
    float lerp(float t, float a, float b);
    // グラデーション関数の前方宣言
    float grad(int hash, float x, float y);
    //実行関数
    void Act(float frequency = 0.1f);
    //初期化
    void Init();
    //データ受け取り
    //色
    COLORREF GetColor(int x, int y,int);
    //データセット
    void SetRectSize(std:: pair<int, int>temp);

private:
    // パーリンノイズに使用するランダムな値の配列
    int p[RAND_ARRAY_SIZE];
    float freqency; //周波数
    int rectWidth;  //矩形のサイズ
    int rectHeight;
    int outputWidth;
    int outputHeight;
    std::vector<std::vector< COLORREF>>colorArray;
};

//コンストラクタ
PerlinNoise::PerlinNoise() {
    std::fill(p, p + RAND_ARRAY_SIZE, 0);
    rectWidth = 30;
    rectHeight = 30;
    freqency = 0.1f;
    outputWidth = 0;
    outputHeight = 0;
}

// パーリンノイズ関数
float PerlinNoise::perlinNoise(float x, float y) {
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
float PerlinNoise::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// 線形補間関数
float PerlinNoise::lerp(float t, float a, float b) {
    return a + t * (b - a);
}

// グラデーション関数
float PerlinNoise::grad(int hash, float x, float y) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14) ? x : 0;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

//実行関数
void PerlinNoise::Act(float freqency) {
    Init();
    this->freqency = freqency;
    outputWidth = rectWidth * freqency;
    outputHeight = rectHeight * freqency;

    //vectorサイズを変更
    colorArray.resize(outputHeight, std::vector<COLORREF>(outputWidth, RGB(0, 0, 0)));

    for (int y = 0; y < outputHeight; y++) {
        for (int x = 0; x < outputWidth; x++) {
            perlinNoise(x + freqency, y * freqency);
        }
    }
}

//初期化
void PerlinNoise::Init() {
    for (int e = 0; e < RAND_ARRAY_SIZE; e++) {
        p[e] = rand() % 256;
    }
    colorArray.clear();
}

//データ受け取り
COLORREF PerlinNoise::GetColor(int tempX, int tempY,int outputScale) {  //引数はoutputSizeに対応した座標
    if (tempX >= outputWidth || tempY >= outputHeight) {
        return RGB(0, 0, 0);
    }
    int sizeX = outputWidth / outputScale;
    int sizeY = outputHeight / outputScale;
    int r = 0;
    int g = 0;
    int b = 0;

    for (int y = tempY - sizeY; y < tempY; y++) {
        for (int x = tempX - sizeX; x < tempX; x++) {
            r += GetRValue(colorArray[y][x]);
            g += GetGValue(colorArray[y][x]);
            b += GetBValue(colorArray[y][x]);
        }
    }

    return RGB(r / (sizeX * sizeY), g / (sizeX * sizeY), b / (sizeX * sizeY));
}

void PerlinNoise::SetRectSize(std::pair<int, int>temp) {
    rectWidth = temp.first;
    rectHeight = temp.second;
}