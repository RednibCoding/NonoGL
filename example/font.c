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

    nnSetColor((nnColorf){0.8, 0.2, 0.6, 1.0});
    nnDrawText("Hello NonoGL!", 200, 200);
    nnResetColor();
    nnDrawText("Hello World!", 200, 240);

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
