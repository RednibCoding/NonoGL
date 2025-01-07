#include <stdio.h>
#include <stdbool.h>

#define MG_IMPL
#include "../minig/mg.h"

mgImage image;
mgFont *font;

void display()
{
    mgSetWindowTitle("MiniG Window  FPS: %d", mgFPS);
    mgCls();

    mgDrawImage(image, 200, 120);

    mgDrawImagePortion(image, 200, 200, (mgRecf){0.0f, 0.0f, 50.0f, 50.0f});

    mgDrawText("Hello World", 200, 300);

    mgSetColor((mgColorf){1.0f, 0.5f, 0.8f, 1.0f});
    mgDrawText("Hello Color", 200, 340);

    mgSetColor((mgColorf){1.0f, 1.0f, 0.0f, 1.0f});
    mgDrawText("FPS: %d", 200, 370, mgFPS);
    mgDrawText("Delta Time: %f", 200, 400, mgDT);
    mgResetColor();

    if (mgKeyHit('a'))
    {
        printf("Key 'A' was just pressed!\n");
    }

    if (mgKeyDown('a'))
    {
        printf("Key 'A' is being held down!\n");
    }

    if (mgKeyReleased('a'))
    {
        printf("Key 'A' was just released!\n");
    }

    if (mgMouseHit(0))
    {
        printf("Left mouse button was just clicked!\n");
    }

    if (mgMouseDown(0))
    {
        printf("Left mouse button is being held down!\n");
    }

    if (mgMouseReleased(0))
    {
        printf("Left mouse button was just released!\n");
    }

    int wheelDelta = mgMouseWheelDelta();
    if (wheelDelta > 0)
    {
        printf("Mouse wheel moved up!\n");
    }
    else if (wheelDelta < 0)
    {
        printf("Mouse wheel moved down!\n");
    }

    mgVec2 pos = mgGetMousePosition();
    mgVec2 motionDelta = mgMouseMotionDelta();
    if (motionDelta.x != 0 || motionDelta.y != 0)
    {
        printf("Mouse moved: %d, %d | %d, %d \n", motionDelta.x, motionDelta.y, pos.x, pos.y);
    }

    mgFlip();
}

int main()
{
    bool success = mgCreateWindow("MiniG Window", 1024, 600, true, true);
    if (!success)
        return -1;

    font = mgLoadFont("./assets/Roboto-Regular.ttf", 24.0f);
    if (!font)
    {
        printf("Failed to load font.\n");
        return -1;
    }

    mgSetFont(font);

    image = mgLoadImage("./assets/opengl.png");

    mgSetDisplayLoop(display);
    mgRun();

    mgFreeFont(font);
    mgFreeImage(image);
    mgDestroyWindow();
}