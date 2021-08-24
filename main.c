#define UNICODE
#define _UNICODE

#include <windows.h>
#include <stdint.h>
#include <stdio.h>

//4 byte pointer to memory
typedef uint32_t Pixel;
//4 byte color value
typedef uint32_t Color32;

typedef struct Point2D{
    int X;
    int Y;
} Point2D;

Point2D player;
int running = 1;
int clientWidth = 0;
int clientHeight = 0;
void* memory;
BITMAPINFO bitmapInfo;

int FillScreen(Color32 color){
    Pixel *pixel = (Pixel *)memory;
    int i;

    for(i=0;i<clientWidth*clientHeight;i++){
        *pixel++=color;
    }
}
int IsPointInBoundaries(Point2D point2d){
    if(point2d.X>=clientWidth||point2d.Y>=clientHeight||point2d.X<0||point2d.Y<0){
        printf("Target point out of bounds. Height: %d, Y: %d, Width: %d, X: %d",clientHeight,point2d.Y,clientWidth,point2d.X);
        return 0;
    }
    return 1;
}

int PaintPixel(Point2D point2d, Color32 color){

    //Consider change this to modulo operator for wrapping around?
    if(!IsPointInBoundaries(point2d))
        return -1;
    Pixel* pixel = (Pixel *)memory;
    
    pixel+= point2d.Y*clientWidth + point2d.X;
    *pixel = color;
    return 0;
}


int FillRectangle(Point2D point2d, int rectangleWidth, int rectangleHeight, Color32 color){
    if(!IsPointInBoundaries(point2d))
        return -1;
    Pixel *pixel = (Pixel *)memory;

    pixel+= point2d.Y*clientWidth + point2d.X;

    int i,j;
    for(i=0;i<rectangleWidth&&i<clientWidth;i++){
        for(j=0;j<rectangleHeight&&j<clientHeight;j++){
            *pixel++=color;
        }
        pixel+=clientWidth - rectangleWidth;
    }
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
        case WM_KEYDOWN:
        {
            switch(wParam)
            {
                case VK_RIGHT:
                {
                    player.X++;
                }break;
                case VK_LEFT:
                {
                    player.X--;
                }break;
                case VK_UP:
                {
                    player.Y++;
                }break;
                case VK_DOWN:
                {
                    player.Y--;
                }

            }
        }
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
    #pragma region CREATING WINDOW
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
    clientWidth = rect.right-rect.left;
    clientHeight = rect.bottom-rect.top;
    //Each pixel is 32 bits, therefore 4 bytes, thats why we multiply width and height by 4
    memory = VirtualAlloc(0, clientWidth*clientHeight*4, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    Point2D point;
    point.X = 33;
    point.Y = 33;
    player.X = 0;
    player.Y = 0;

    bitmapInfo.bmiHeader;

    bitmapInfo.bmiHeader.biSize =sizeof(bitmapInfo.bmiHeader);
    bitmapInfo.bmiHeader.biWidth = clientWidth;
    bitmapInfo.bmiHeader.biHeight = clientHeight;
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

            FillScreen(0x000000);
            FillRectangle(player,100,100,0xFF0000);


            int resultStretchDIBits = StretchDIBits(deviceContext,
                0,0,
                clientWidth, clientHeight,
                0,0,
                clientWidth,clientHeight,
                memory,
                &bitmapInfo,
                DIB_RGB_COLORS,
                SRCCOPY
                );
        }
    }
    return 0;
}