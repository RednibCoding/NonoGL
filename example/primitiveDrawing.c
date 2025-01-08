#include <stdio.h>
#include <stdbool.h>

#define NONOGL_IMPLEMENTATION
#include "../nonogl/ngl.h"

void render()
{
    nnSetWindowTitle("NonoGL Window  FPS: %d", nnFPS);

    nnCls();

    // Create a pixmap
    nnPixmap *pixmap = nnCreatePixmap(800, 600);

    if (!pixmap)
    {
        printf("Failed to create pixmap.\n");
        return;
    }

    // Set a background color
    nnColorf backgroundColor = {0.1f, 0.1f, 0.1f, 1.0f};
    for (int y = 0; y < pixmap->height; y++)
    {
        for (int x = 0; x < pixmap->width; x++)
        {
            nnDrawPixel(pixmap, x, y, backgroundColor);
        }
    }

    // Draw a red line
    nnColorf red = {1.0f, 0.0f, 0.0f, 1.0f};
    nnDrawLine(pixmap, 50, 50, 300, 200, red);

    // Draw a green rectangle (filled)
    nnColorf green = {0.0f, 1.0f, 0.0f, 1.0f};
    nnDrawRect(pixmap, 100, 300, 200, 100, green, true);

    // Draw a blue rectangle (outline)
    nnColorf blue = {0.0f, 0.0f, 1.0f, 1.0f};
    nnDrawRect(pixmap, 400, 300, 200, 100, blue, false);

    // Draw a yellow triangle (filled)
    nnColorf yellow = {1.0f, 1.0f, 0.0f, 1.0f};
    nnDrawTriangle(pixmap, 500, 100, 700, 150, 600, 300, yellow, true);

    // Draw a cyan triangle (outline)
    nnColorf cyan = {0.0f, 1.0f, 1.0f, 1.0f};
    nnDrawTriangle(pixmap, 150, 400, 250, 450, 200, 550, cyan, false);

    // Draw a magenta oval (filled)
    nnColorf magenta = {1.0f, 0.0f, 1.0f, 1.0f};
    nnDrawOval(pixmap, 600, 400, 100, 150, magenta, true);

    // Draw a white oval (outline)
    nnColorf white = {1.0f, 1.0f, 1.0f, 1.0f};
    nnDrawOval(pixmap, 200, 200, 150, 100, white, false);

    // Upload the pixmap to the GPU
    nnUpdatePixmap(pixmap);

    // Draw the pixmap on the screen
    nnDrawPixmap(pixmap, 0, 0);

    // Free the pixmap
    nnFreePixmap(pixmap);

    nnFlip(); // Swap buffers to display the scene
}

int main()
{
    // Create a window
    if (!nnCreateWindow("NonoGL Example", 800, 600, false, false))
    {
        printf("Failed to create window.\n");
        return -1;
    }

    nnSetRenderFunc(render); // Set the render callback
    nnRun();                 // Start the rendering loop

    nnDestroyWindow(); // Cleanup resources
    return 0;
}
