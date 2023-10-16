#pragma once
#include "resource.h"
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
    // �p�[�����m�C�Y�֐��̑O���錾
    float perlinNoise(float x, float y);
    // �t�F�[�h�֐��̑O���錾
    float fade(float t);
    // ���`��Ԋ֐��̑O���錾
    float lerp(float t, float a, float b);
    // �O���f�[�V�����֐��̑O���錾
    float grad(int hash, float x, float y);
    //���s�֐�
    void Act(int sizeX, int sizeY, float frequency = 0.1f);
    //������
    void Init();
    //�f�[�^�󂯎��
    void GetColor(int x, int y);
private:
    // �p�[�����m�C�Y�Ɏg�p���郉���_���Ȓl�̔z��
    int p[RAND_ARRAY_SIZE];
};


// �p�[�����m�C�Y�֐�
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

// �t�F�[�h�֐�
float PerlinNoise::fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// ���`��Ԋ֐�
float PerlinNoise::lerp(float t, float a, float b) {
    return a + t * (b - a);
}

// �O���f�[�V�����֐�
float PerlinNoise::grad(int hash, float x, float y) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : (h == 12 || h == 14) ? x : 0;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

//���s�֐�
void PerlinNoise::Act(int sizeX, int sizeY, float frequency) {
    Init();
}

//������
void PerlinNoise::Init() {
    for (int e = 0; e < RAND_ARRAY_SIZE; e++) {
        p[e] = rand() % 256;
    }
}

//�f�[�^�󂯎��
void PerlinNoise::GetColor(int x, int y) {

}