#define UNICODE
#define _UNICODE

#include <windows.h>
#include <stdint.h>

#define Pixel uint32_t

int running = 1;
int width = 0;
int height = 0;
void* memory;
BITMAPINFO bitmapInfo;

int PaintPixel(int X, int Y, Pixel color){
    Pixel* pixel = (Pixel *)memory;
    
    pixel+=+ Y*width + X;
    *pixel = color;
}

LRESULT CALLBACK WindowProc(HWND handleWindow, UINT message, WPARAM wParam, LPARAM lParam){

    switch(message){
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }break;
        case WM_CLOSE:
        {
            running = 0;
            return 0;
        }break;
        default:
        {
            return DefWindowProc(handleWindow,message,wParam,lParam);
        }break;
        return 0;
    }
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
    #pragma region VARIABLES
    WNDCLASS windowClass = {0};

    const wchar_t CLASS_NAME[] = L"WinGame";
    const wchar_t WINDOW_NAME[] = L"WinGame";
    windowClass.lpszClassName = CLASS_NAME;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    RegisterClass(&windowClass);


    HWND handleWindow;
    handleWindow = CreateWindowEx(0,
                                CLASS_NAME,
                                 WINDOW_NAME,
                                  WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                                  0,
                                  0,
                                  640,
                                  480,
                                  0,
                                  0,
                                  hInstance,
                                  0);

    if(handleWindow == NULL){
        return -1;
    }
#pragma endregion
    RECT rect;
    GetClientRect(handleWindow, &rect);
    width = rect.right-rect.left;
    height = rect.bottom-rect.top;
    //Each pixel is 32 bits, therefore 4 bytes, thats why we multiply width and height by 4
    memory = VirtualAlloc(0, width*height*4, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);


    PaintPixel(10,0,0xFFFFFF);


    bitmapInfo.bmiHeader;

    bitmapInfo.bmiHeader.biSize =sizeof(bitmapInfo.bmiHeader);
    bitmapInfo.bmiHeader.biWidth = width;
    bitmapInfo.bmiHeader.biHeight = height;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;


    ShowWindow(handleWindow,nCmdShow);


    MSG message = {0};
    HDC deviceContext = GetDC(handleWindow);

    while(running){
        while(PeekMessage(&message,handleWindow,0,0,PM_REMOVE)){
            if(message.message == WM_QUIT){
                running = 0;
            }
            TranslateMessage(&message);
            DispatchMessage(&message);
            int resultStretchDIBits = StretchDIBits(deviceContext,
                0,0,
                width, height,
                0,0,
                width,height,
                memory,
                &bitmapInfo,
                DIB_RGB_COLORS,
                SRCCOPY
                );
        }
    }
}