#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "Perlin.h"
#include <iostream>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define MAX_SIZE	(100)
#define START_HEIGHT    (10)

//グローバル変数
int rectWidth = 30;
int rectHeight = 30;
int scale = 1;  //倍率
PerlinNoise parlin;
float freqency = 0.1f;
HFONT hFont;
int pixelSize = 10;
int outputWidth = rectWidth * (scale / freqency);
int outputHeight = rectHeight * (scale / freqency);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ PSTR szCmdLine, _In_ int iCmdShow) {
    srand((unsigned)time(NULL));
    parlin.Act(rectWidth, rectHeight);
    static TCHAR szAppName[] = TEXT("PerlinNoise");
    HWND hwnd;
    MSG msg;
    WNDCLASSEX wndclass = {};

    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszClassName = szAppName;
    //フォントの作成
    hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));


    if (!RegisterClassEx(&wndclass)) {
        MessageBox(nullptr, TEXT("This program requires Windows NT!"),
            szAppName, MB_ICONERROR);
        return 0;
    }
    //ウィンドウを規定
    hwnd = CreateWindow(szAppName, TEXT("PerlinNoise"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        500, 500, nullptr, nullptr,
        hInstance, nullptr);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    DeleteObject(hFont);
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND wTextBox;   //幅
    static HWND hTextBox;   //高さ
    static HWND fTextBox;   //周波数
    switch (message) {
    case WM_CREATE: {
        // ボタンの作成
        //サイズ変更ボタン
        CreateWindow(TEXT("BUTTON"), TEXT("Decrease Size"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            150, START_HEIGHT + 20, 150, 20, hwnd, (HMENU)1, nullptr, nullptr);
        //ParlinNoise実行
        CreateWindow(TEXT("BUTTON"), TEXT("Let's Perlin"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            150, START_HEIGHT + 40, 150, 20, hwnd, (HMENU)2, nullptr, nullptr);
        //チェックボックス
        CreateWindow(L"BUTTON", L"CHECKBOX", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
            150, START_HEIGHT + 00, 150, 20, hwnd, (HMENU)3, nullptr, nullptr);
        // テキストボックスの作成
        //幅
        wTextBox = CreateWindow(TEXT("EDIT"), TEXT("30"), WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
            80, START_HEIGHT + 0, 30, 20, hwnd, (HMENU)4, nullptr, nullptr);
        //高さ
        hTextBox = CreateWindow(TEXT("EDIT"), TEXT("30"), WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
            80, START_HEIGHT + 20, 30, 20, hwnd, (HMENU)5, nullptr, nullptr);
        //周波数
        fTextBox = CreateWindow(TEXT("EDIT"), TEXT("0.1"), WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
            80, START_HEIGHT + 40, 30, 20, hwnd, (HMENU)6, nullptr, nullptr);
        // 初期の矩形描画
        RECT rect;
        GetClientRect(hwnd, &rect);
        rect.left = 10;
        rect.top = START_HEIGHT + 70;
        rect.right = rect.left + rectWidth;
        rect.bottom = rect.top + rectHeight;
        InvalidateRect(hwnd, &rect, TRUE);
        return 0;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == 1) {
            // テキストボックスの内容が変更されたとき
            char buffer[10];
            GetWindowTextA(wTextBox, buffer, sizeof(buffer));
            int newWidth = atoi(buffer);
            GetWindowTextA(hTextBox, buffer, sizeof(buffer));
            int newHeight = atoi(buffer);
            if (newWidth > MAX_SIZE || newHeight > MAX_SIZE) {
                MessageBox(NULL, TEXT("100より大きいサイズにしないで"), TEXT("警告"), MB_ICONWARNING | MB_OK);
            }
            else if (newWidth > 0) {

                // 矩形を再描画前にクリア
                RECT rectToClear;
                GetClientRect(hwnd, &rectToClear);
                rectToClear.left = 10;
                rectToClear.top = START_HEIGHT + 70;
                rectToClear.right = rectToClear.left + rectWidth;
                rectToClear.bottom = rectToClear.top + rectHeight;
                InvalidateRect(hwnd, &rectToClear, FALSE);
                // 矩形を再描画
                RECT rectToRedraw;
                GetClientRect(hwnd, &rectToRedraw);
                rectToRedraw.left = 10;
                rectToRedraw.top = START_HEIGHT + 70;
                rectToRedraw.right = rectToRedraw.left + newWidth;
                rectToRedraw.bottom = rectToRedraw.top + newHeight;
                InvalidateRect(hwnd, &rectToRedraw, TRUE);
                rectWidth = newWidth;
                rectHeight = newHeight;
            }
        }
        if (LOWORD(wParam) == 2) {
            parlin.Act(rectWidth, rectHeight);
            InvalidateRect(hwnd, NULL, TRUE);
            // 矩形を再描画前にクリア
            RECT rectToClear;
            GetClientRect(hwnd, &rectToClear);
            rectToClear.left = 10;
            rectToClear.top = START_HEIGHT + 70;
            rectToClear.right = rectToClear.left + rectWidth;
            rectToClear.bottom = rectToClear.top + rectHeight;
            InvalidateRect(hwnd, &rectToClear, FALSE);
        }
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); // 白でクリア
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 10, START_HEIGHT + 70, 10 + rectWidth, 70 + rectHeight);

        // テキストを描画
        SelectObject(hdc, hFont);  // フォントを選択

        TextOut(hdc, 10, START_HEIGHT + 0, TEXT("幅"), 1);
        TextOut(hdc, 10, START_HEIGHT + 0, TEXT("高さ"), 1);

        for (int x = 0; x < outputWidth; x += pixelSize) {
            for (int y = 0; y < outputHeight; y += pixelSize) {
                int color = RGB(255, 0, 0); // 赤色 (BGR オーダー)
                RECT rect = { x, y, x + pixelSize, y + pixelSize };
                HBRUSH hBrush = CreateSolidBrush(color);
                FillRect(hdc, &rect, hBrush);
                DeleteObject(hBrush);
            }
        }

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
}
