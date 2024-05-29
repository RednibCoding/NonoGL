#include <stdio.h>
#include <stdbool.h>

#define MG_IMPL
#include "../minig/mg.h"

mgImage image;

void display() {
    while (true) {
        mgClear();

        mgDrawImage(image, (mgPointf){20.0f, 20.0f});

        mgDrawImagePortion(image, (mgPointf){200.0f, 200.0f}, (mgRecf){0.0f, 0.0f, 50.0f, 50.0f});

        mgSetColor((mgColorf){0.5f, 0.5f, 0.8f, 1.0f});

        mgDrawText("Hello World", (mgPointf){200.0f, 300.0f});

        mgSetColor((mgColorf){1.5f, 0.5f, 0.8f, 1.0f});

        mgDrawText("Hello Color", (mgPointf){200.0f, 340.0f});

        mgSetColor((mgColorf){1.0f, 1.0f, 1.0f, 1.0f});

        mgSwap();
    }
}

int main() {
    bool success = mgCreateWindow("MiniG Window", 1024, 600, false, false);
    if (!success) return -1;

    mgImage image = mgLoadImage("./assets/GLFW.png");

    mgSetDisplayLoop(display);
    mgRun();

    mgDestroyWindow();
}