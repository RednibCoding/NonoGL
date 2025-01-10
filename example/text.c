#include <stdio.h>
#include <stdbool.h>

#define NONOGL_IMPLEMENTATION
#include "../nonogl/ngl.h"

nnFont *font;

void render()
{
    nnSetWindowTitle("NonoGL Window  FPS: %d", nnFPS);
    int mx = nnMousePosition().x;
    int my = nnMousePosition().y;

    nnSetColor((nnColorf){1.0f, 0.5f, 0.8f, 1.0f});
    nnDrawText("NonoGL", 200, 200);

    nnSetColor((nnColorf){1.0f, 1.0f, 0.0f, 1.0f});
    nnDrawText("FPS: %d", 200, 370, nnFPS);
    nnDrawText("Delta Time: %f", 200, 400, nnDT);
    nnResetColor();

    nnDrawDebugText("mx: %d, my: %d", 10, 20, mx, my);
}

int main()
{
    nnSetDebugMode(true);

    // Create a window
    if (!nnCreateWindow("NonoGL Example", 800, 600, false, false))
    {
        printf("Failed to create window.\n");
        return -1;
    }

    font = nnLoadFont("./assets/Roboto-Regular.ttf", 24.0f);
    if (!font)
    {
        printf("Failed to load font.\n");
        return -1;
    }

    nnSetFont(font);

    nnSetRenderFunc(render); // Set the render callback
    nnRun();                 // Start the rendering loop

    nnFreeFont(font);
    nnDestroyWindow(); // Cleanup resources
    return 0;
}
