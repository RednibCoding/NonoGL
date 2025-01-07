#include <stdio.h>
#include <stdbool.h>

#define MG_IMPL
#include "../minig/mg.h"

mgImage image;

void display()
{
    mgSetWindowTitle("MiniG Window  FPS: %d", mgFPS);
    mgCls();

    mgDrawImage(image, (mgPointf){20.0f, 20.0f});

    mgDrawImagePortion(image, (mgPointf){200.0f, 200.0f}, (mgRecf){0.0f, 0.0f, 50.0f, 50.0f});

    mgSetColor((mgColorf){0.5f, 0.5f, 0.8f, 1.0f});

    mgDrawText("Hello World", (mgPointf){200.0f, 300.0f});

    mgSetColor((mgColorf){1.5f, 0.5f, 0.8f, 1.0f});

    mgDrawText("Hello Color", (mgPointf){200.0f, 340.0f});

    mgSetColor((mgColorf){1.0f, 1.0f, 1.0f, 1.0f});

    mgDrawText("FPS: %d", (mgPointf){200.0f, 370.0f}, mgFPS);
    mgDrawText("Delta Time: %f", (mgPointf){200.0f, 400.0f}, mgDT);

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