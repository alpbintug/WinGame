#include <stdio.h>

//4 byte pointer to memory
typedef uint32_t Pixel;
//4 byte color value
typedef uint32_t Color32;

extern int clientWidth,  clientHeight;
extern void *memory;

typedef struct Point2D{
    int X;
    int Y;
} Point2D;

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


int FillRectangle(Point2D startingPoint, int rectangleWidth, int rectangleHeight, Color32 color){
    Point2D endingPoint;
    endingPoint.X = startingPoint.X + rectangleWidth;
    endingPoint.Y = startingPoint.Y + rectangleHeight;
    if(!IsPointInBoundaries(startingPoint)||!IsPointInBoundaries(endingPoint))
    {
        if(startingPoint.X<0){
            rectangleWidth += startingPoint.X;
            startingPoint.X = 0;
        }
        
        if(startingPoint.Y<0){
            rectangleHeight += startingPoint.Y;
            startingPoint.Y = 0;
        }
        if(startingPoint.X + rectangleWidth>=clientWidth){
            rectangleWidth =  clientWidth - startingPoint.X ;
        }
        if(startingPoint.Y + rectangleHeight>=clientHeight){
            rectangleHeight = clientHeight- startingPoint.Y;
        }
    }
    Pixel *pixel = (Pixel *)memory;
    pixel+= startingPoint.Y*clientWidth + startingPoint.X;
    int i,j;
    for(i=0;i<rectangleHeight&&i<clientHeight;i++){
        for(j=0;j<rectangleWidth&&j<clientWidth;j++){
            *pixel++=color;
        }
        pixel+=clientWidth - rectangleWidth;
    }
    return 0;
}

int DrawLine(Point2D startingPoint, Point2D endingPoint, int thickness, Color32 color){

}

int DrawRectangle(Point2D startingPoint, int rectangleWidth, int rectangleHeight, int thickness, Color32 color){

}
