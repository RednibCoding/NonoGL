#include <stdio.h>
#include <stdbool.h>

#define TINYGL_IMPLEMENTATION
#include "../tinygl/tgl.h"

tgImage image;
tgFont *font;

void display()
{
    tgSetWindowTitle("TinyGL Window  FPS: %d", tgFPS);
    tgCls();

    tgDrawImage(image, 200, 120);

    tgDrawImagePortion(image, 200, 200, (tgRecf){0.0f, 0.0f, 50.0f, 50.0f});

    tgDrawText("Hello World", 200, 300);

    tgSetColor((tgColorf){1.0f, 0.5f, 0.8f, 1.0f});
    tgDrawText("Hello Color", 200, 340);

    tgSetColor((tgColorf){1.0f, 1.0f, 0.0f, 1.0f});
    tgDrawText("FPS: %d", 200, 370, tgFPS);
    tgDrawText("Delta Time: %f", 200, 400, tgDT);
    tgResetColor();

    if (tgKeyHit('a'))
    {
        printf("Key 'A' was just pressed!\n");
    }

    if (tgKeyDown('a'))
    {
        printf("Key 'A' is being held down!\n");
    }

    if (tgKeyReleased('a'))
    {
        printf("Key 'A' was just released!\n");
    }

    if (tgMouseHit(0))
    {
        printf("Left mouse button was just clicked!\n");
    }

    if (tgMouseDown(0))
    {
        printf("Left mouse button is being held down!\n");
    }

    if (tgMouseReleased(0))
    {
        printf("Left mouse button was just released!\n");
    }

    int wheelDelta = tgMouseWheelDelta();
    if (wheelDelta > 0)
    {
        printf("Mouse wheel moved up!\n");
    }
    else if (wheelDelta < 0)
    {
        printf("Mouse wheel moved down!\n");
    }

    tgVec2 pos = tgGetMousePosition();
    tgVec2 motionDelta = tgMouseMotionDelta();
    if (motionDelta.x != 0 || motionDelta.y != 0)
    {
        printf("Mouse moved: %d, %d | %d, %d \n", motionDelta.x, motionDelta.y, pos.x, pos.y);
    }

    tgFlip();
}

int main()
{
    bool success = tgCreateWindow("TinyGL Window", 1024, 600, true, false);
    if (!success)
        return -1;

    font = tgLoadFont("./assets/Roboto-Regular.ttf", 24.0f);
    if (!font)
    {
        printf("Failed to load font.\n");
        return -1;
    }

    tgSetFont(font);

    image = tgLoadImage("./assets/opengl.png");

    tgSetDisplayFunc(display);
    tgRun();

    tgFreeFont(font);
    tgFreeImage(image);
    tgDestroyWindow();
}