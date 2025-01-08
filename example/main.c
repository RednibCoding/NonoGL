#include <stdio.h>
#include <stdbool.h>

#define NONOGL_IMPLEMENTATION
#include "../nonogl/ngl.h"

nnImage image;
nnFont *font;
nnPixmap *pixmap;

void display()
{
    nnSetWindowTitle("NonoGL Window  FPS: %d", nnFPS);
    nnCls();

    nnDrawImage(image, 200, 120);

    nnDrawImagePortion(image, 200, 200, (nnRecf){0.0f, 0.0f, 50.0f, 50.0f});

    nnDrawText("Hello World", 200, 300);

    nnSetColor((nnColorf){1.0f, 0.5f, 0.8f, 1.0f});
    nnDrawText("Hello Color", 200, 340);

    nnSetColor((nnColorf){1.0f, 1.0f, 0.0f, 1.0f});
    nnDrawText("FPS: %d", 200, 370, nnFPS);
    nnDrawText("Delta Time: %f", 200, 400, nnDT);
    nnResetColor();

    nnDrawPixmap(pixmap, 10, 10);

    if (nnKeyHit('a'))
    {
        printf("Key 'A' was just pressed!\n");
    }

    if (nnKeyDown('a'))
    {
        printf("Key 'A' is being held down!\n");
    }

    if (nnKeyReleased('a'))
    {
        printf("Key 'A' was just released!\n");
    }

    if (nnMouseHit(0))
    {
        printf("Left mouse button was just clicked!\n");
    }

    if (nnMouseDown(0))
    {
        printf("Left mouse button is being held down!\n");
    }

    if (nnMouseReleased(0))
    {
        printf("Left mouse button was just released!\n");
    }

    int wheelDelta = nnMouseWheelDelta();
    if (wheelDelta > 0)
    {
        printf("Mouse wheel moved up!\n");
    }
    else if (wheelDelta < 0)
    {
        printf("Mouse wheel moved down!\n");
    }

    nnPos pos = nnGetMousePosition();
    nnPos motionDelta = nnMouseMotionDelta();
    if (motionDelta.x != 0 || motionDelta.y != 0)
    {
        printf("Mouse moved: %d, %d | %d, %d \n", motionDelta.x, motionDelta.y, pos.x, pos.y);
    }

    nnFlip();
}

int main()
{
    bool success = nnCreateWindow("NonoGL Window", 1024, 600, true, false);
    if (!success)
        return -1;

    font = nnLoadFont("./assets/Roboto-Regular.ttf", 24.0f);
    if (!font)
    {
        printf("Failed to load font.\n");
        return -1;
    }

    nnSetFont(font);

    image = nnLoadImage("./assets/opengl.png");
    pixmap = nnCreatePixmapFromImage(image);

    nnColorf pixel = nnReadPixel(pixmap, 18, 14);

    printf("pixmap pixel: %.2f, %.2f, %.2f, %.2f\n", pixel.r, pixel.g, pixel.b, pixel.a);
    printf("pixmap size: %d x %d\n", pixmap->width, pixmap->height);

    nnSetDisplayFunc(display);
    nnRun();

    nnFreeFont(font);
    nnFreeImage(image);
    nnDestroyWindow();
}