#include <stdio.h>
#include <stdbool.h>

#define NONOGL_IMPLEMENTATION
#include "../nonogl/ngl.h"

nnImage image;
nnPixmap *pixmap;

void render()
{
    nnSetWindowTitle("NonoGL Window  FPS: %d", nnFPS);

    nnCls();

    nnDrawPixmap(pixmap, 100, 100);

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

    image = nnLoadImage("./assets/opengl.png");
    pixmap = nnCreatePixmapFromImage(image);

    nnColorf pixel = nnReadPixel(pixmap, 18, 14);

    printf("pixmap pixel: %.2f, %.2f, %.2f, %.2f\n", pixel.r, pixel.g, pixel.b, pixel.a);
    printf("pixmap size: %d x %d\n", pixmap->width, pixmap->height);

    nnSetRenderFunc(render); // Set the render callback
    nnRun();                 // Start the rendering loop

    nnFreeImage(image);
    nnFreePixmap(pixmap);
    nnDestroyWindow(); // Cleanup resources
    return 0;
}
