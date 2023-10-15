#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int rectWidth = 100;
int rectHeight = 100;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow) {
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

    if (!RegisterClassEx(&wndclass)) {
        MessageBox(nullptr, TEXT("This program requires Windows NT!"),
            szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName, TEXT("Resizable Rectangle"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        400, 300, nullptr, nullptr,
        hInstance, nullptr);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hTextBox;
    switch (message) {
    case WM_CREATE: {
        // ボタンの作成
        CreateWindow(TEXT("BUTTON"), TEXT("Decrease Size"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 30, 150, 30, hwnd, (HMENU)1, nullptr, nullptr);

        // テキストボックスの作成
        hTextBox = CreateWindow(TEXT("EDIT"), TEXT("100"), WS_CHILD | WS_VISIBLE | ES_NUMBER,
            10, 10, 50, 20, hwnd, (HMENU)2, nullptr, nullptr);

        // 初期の矩形描画
        RECT rect;
        GetClientRect(hwnd, &rect);
        rect.left = 10;
        rect.top = 40;
        rect.right = rect.left + rectWidth;
        rect.bottom = rect.top + rectHeight;
        InvalidateRect(hwnd, &rect, TRUE);
        return 0;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == 1) {
            // テキストボックスの内容が変更されたとき
            char buffer[10];
            GetWindowTextA(hTextBox, buffer, sizeof(buffer));
            int newWidth = atoi(buffer);

            if (newWidth > 0) {
                rectWidth = newWidth;

                // 矩形を再描画前にクリア
                RECT rect;
                GetClientRect(hwnd, &rect);
                rect.left = 10;
                rect.top = 50;
                rect.right = rect.left + rectWidth + 10; // +10 for extra clearance
                rect.bottom = rect.top + rectHeight + 10; // +10 for extra clearance
                InvalidateRect(hwnd, &rect, TRUE);

                // 矩形を再描画
               // RECT rect;
                GetClientRect(hwnd, &rect);
                rect.left = 10;
                rect.top = 40;
                rect.right = rect.left + rectWidth;
                rect.bottom = rect.top + rectHeight;
                InvalidateRect(hwnd, &rect, TRUE);
            }
        }
        
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); // 白でクリア
        SelectObject(hdc, hBrush);
        Rectangle(hdc, 10, 50, 10 + rectWidth, 60 + rectHeight);

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
