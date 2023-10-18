#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Perlin.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define MAX_SIZE	(100)
#define START_HEIGHT    (10)
#define DEFAULT_SIZE    (10)


enum WINDOW_NUM {   //buttonなどの番号
    C_ORIGINAL_PERLIN,
    B_SIZE_CHANGE,
    B_PERLIN_ACT,
    N_PERLIN_WIDTH,
    N_PERLIN_HEIGHT,
    N_PERLIN_FREQENCY,
    N_OUTPUT_SCALE,
    N_GRID_SPACING
};

//グローバル変数
PerlinNoise perlin;
HFONT hFont;
int rectSize;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
    _In_ PSTR szCmdLine, _In_ int iCmdShow) {
    srand((unsigned)time(NULL));
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

    rectSize = DEFAULT_SIZE;
    perlin.Act(rectSize,0.1f);

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
    static HWND sTextBox;   //表示倍率
    static HWND gTextBox;   //グリッド間隔

    

    switch (message) {
    case WM_CREATE: {
        // ボタンの作成
        //サイズ変更ボタン
        CreateWindow(TEXT("BUTTON"), TEXT("Decrease Size"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            220, START_HEIGHT + 20, 150, 20, hwnd, (HMENU)B_SIZE_CHANGE, nullptr, nullptr);
        //ParlinNoise実行
        CreateWindow(TEXT("BUTTON"), TEXT("Let's Perlin"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            220, START_HEIGHT + 40, 150, 20, hwnd, (HMENU)B_PERLIN_ACT, nullptr, nullptr);
        //チェックボックス
        CreateWindow(L"BUTTON", L"CHECKBOX", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
            220, START_HEIGHT + 00, 150, 20, hwnd, (HMENU)C_ORIGINAL_PERLIN, nullptr, nullptr);
        // テキストボックスの作成
        //幅
        wTextBox = CreateWindow(TEXT("EDIT"), TEXT("10"), WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
            60, START_HEIGHT + 0, 30, 20, hwnd, (HMENU)N_PERLIN_WIDTH, nullptr, nullptr);
        //高さ
        hTextBox = CreateWindow(TEXT("EDIT"), TEXT("10"), WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
            60, START_HEIGHT + 20, 30, 20, hwnd, (HMENU)N_PERLIN_HEIGHT, nullptr, nullptr);
        //周波数
        fTextBox = CreateWindow(TEXT("EDIT"), TEXT("0.1"), WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
            60, START_HEIGHT + 40, 30, 20, hwnd, (HMENU)N_PERLIN_FREQENCY, nullptr, nullptr);
        //倍率
        sTextBox= CreateWindow(TEXT("EDIT"), TEXT("10"), WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
            170, START_HEIGHT + 0, 30, 20, hwnd, (HMENU)N_OUTPUT_SCALE, nullptr, nullptr);
        gTextBox = CreateWindow(TEXT("EDIT"), TEXT("1"), WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT,
            170, START_HEIGHT + 20, 30, 20, hwnd, (HMENU)N_GRID_SPACING, nullptr, nullptr);
        // 初期の矩形描画
        RECT rect;
        GetClientRect(hwnd, &rect);
        rect.left = 10;
        rect.top = START_HEIGHT + 70;
        rect.right = rect.left + DEFAULT_SIZE;
        rect.bottom = rect.top + DEFAULT_SIZE;
        InvalidateRect(hwnd, &rect, TRUE);
        return 0;
        break;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == B_SIZE_CHANGE) {  //サイズチェンジボタンが押されたとき
            // テキストボックスの内容が変更されたとき
            char buffer[10];
            GetWindowTextA(wTextBox, buffer, sizeof(buffer));
            int newWidth = atoi(buffer);
            GetWindowTextA(hTextBox, buffer, sizeof(buffer));
            int newHeight = atoi(buffer);
            GetWindowTextA(sTextBox, buffer, sizeof(buffer));
            int newScale = atoi(buffer);
            if (newWidth > MAX_SIZE || newHeight > MAX_SIZE) {
                MessageBox(NULL, TEXT("100より大きいサイズにしないで"), TEXT("警告"), MB_ICONWARNING | MB_OK);
            }
            else if (newWidth > 0) {
                // 矩形を再描画範囲を指定
                RECT rectToRedraw;
                GetClientRect(hwnd, &rectToRedraw);
                rectToRedraw.left = 10;
                rectToRedraw.top = START_HEIGHT + 80;
                rectToRedraw.right = rectToRedraw.left + (newWidth* newScale);
                rectToRedraw.bottom = rectToRedraw.top + (newHeight* newScale);
                InvalidateRect(hwnd, &rectToRedraw, TRUE);
                rectSize = newWidth;
            }
        }
        if (LOWORD(wParam) == B_PERLIN_ACT){    //パーリンノイズを実行
            char buffer[10];
            GetWindowTextA(fTextBox, buffer, sizeof(buffer));
            float frequency = (float)atof(buffer);
            GetWindowTextA(sTextBox, buffer, sizeof(buffer));
            int newScale = atoi(buffer);
            perlin.Act(rectSize,frequency);
            // 矩形を再描画前にクリア
            RECT rectToClear;
            GetClientRect(hwnd, &rectToClear);
            rectToClear.left = 10;
            rectToClear.top = START_HEIGHT + 80;
            rectToClear.right = rectToClear.left + (rectSize * rectSize * newScale);
            rectToClear.bottom = rectToClear.top + (rectSize * rectSize * newScale);
            InvalidateRect(hwnd, &rectToClear, TRUE);
        }
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        //グリッドのサイズ
        char buffer[10];
        GetWindowTextA(gTextBox, buffer, sizeof(buffer));
        int newGrid = atoi(buffer);
        GetWindowTextA(sTextBox, buffer, sizeof(buffer));
        int newScale = atoi(buffer);

        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); // 白でクリア
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 10, START_HEIGHT + 80, 10 + rectSize * newScale, 90 + rectSize * newScale);

        // テキストを描画
        SelectObject(hdc, hFont);  // フォントを選択

        TextOut(hdc, 10, START_HEIGHT + 0, TEXT("幅"), 1);
        TextOut(hdc, 10, START_HEIGHT + 20, TEXT("高さ"), 2);
        TextOut(hdc, 10, START_HEIGHT + 40, TEXT("周波数"), 3);
        TextOut(hdc, 100, START_HEIGHT, TEXT("表示倍率"), 4);
        TextOut(hdc, 100, START_HEIGHT + 20, TEXT("グリッド倍率"), 6);

        int gridSize = perlin.GetGridSize(newGrid);
        int rectSize = perlin.GetRectSize();
        int outputSize = rectSize * newScale * perlin.GetFre();


        for (int y = 0; y < rectSize; y += gridSize) {
            for (int x = 0; x < rectSize; x += gridSize) {
                COLORREF color = perlin.GetColor(x, y,gridSize);
                RECT rect = { 10 + (x / gridSize)*(outputSize/gridSize), START_HEIGHT + 80 + (y / gridSize) * (outputSize / gridSize) ,
                    10 + ((x / gridSize)+1) * (outputSize / gridSize) , START_HEIGHT + 80 + ((y / gridSize)+1) * (outputSize / gridSize) };
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
