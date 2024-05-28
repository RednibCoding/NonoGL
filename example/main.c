#include <stdio.h>
#include <stdbool.h>

#define MG_IMPL
#include "../minig/mg.h"

int main() {
    bool success = mgCreateWindow("MiniG Window", 1024, 600, true, false);
    if (!success) return -1;

    mgImage image = mgLoadImage("./assets/GLFW.png");


    while (!mgWindowShouldClose()) {
        mgClear();

        mgDrawImage(image, (mgPointf){20.0f, 20.0f});

        mgDrawImagePortion(image, (mgPointf){200.0f, 200.0f}, (mgRecf){0.0f, 0.0f, 50.0f, 50.0f});

        mgSwap();
    }

    mgDestroyWindow();
}