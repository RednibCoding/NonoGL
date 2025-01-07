#include <stdio.h>
#include <stdbool.h>

#define MG_IMPL
#include "../minig/mg.h"

mgImage image;

void display()
{
    mgSetWindowTitle("MiniG Window  FPS: %d", mgFPS);
    mgCls();

    mgDrawImage(image, (mgPointf){200.0f, 120.0f});

    mgDrawImagePortion(image, (mgPointf){200.0f, 200.0f}, (mgRecf){0.0f, 0.0f, 50.0f, 50.0f});

    mgSetColor((mgColorf){0.5f, 0.5f, 0.8f, 1.0f});

    mgDrawText("Hello World", (mgPointf){200.0f, 300.0f});

    mgSetColor((mgColorf){1.5f, 0.5f, 0.8f, 1.0f});

    mgDrawText("Hello Color", (mgPointf){200.0f, 340.0f});

    mgSetColor((mgColorf){1.0f, 1.0f, 1.0f, 1.0f});

    mgDrawText("FPS: %d", (mgPointf){200.0f, 370.0f}, mgFPS);
    mgDrawText("Delta Time: %f", (mgPointf){200.0f, 400.0f}, mgDT);

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

    mgPointf pos = mgGetMousePosition();
    mgPointf motionDelta = mgMouseMotionDelta();
    if (motionDelta.x != 0 || motionDelta.y != 0)
    {
        printf("Mouse moved: %.2f,  %.2f | %.2f %.2f \n", motionDelta.x, motionDelta.y, pos.x, pos.y);
    }

    mgFlip();
}

int main()
{
    bool success = mgCreateWindow("MiniG Window", 1024, 600, true, true);
    if (!success)
        return -1;

    image = mgLoadImage("./assets/opengl.png");

    mgSetDisplayLoop(display);
    mgRun();

    mgDestroyWindow();
}