#include <stdio.h>
#include <stdbool.h>

#define NONOGL_IMPLEMENTATION
#include "../nonogl/ngl.h"

nnImage image;
nnPixmap *pixmap;
nnPixmap *pixmapRot;
nnPixmap *copyDown;
nnPixmap *copyRight;
nnPixmap *copyRightDown;

void render()
{
    nnSetWindowTitle("NonoGL Window  FPS: %d", nnFPS);
    int mx = nnMousePosition().x;
    int my = nnMousePosition().y;

    int x = 000;
    int y = 000;

    nnDrawPixmap(pixmap, x, y);
    nnDrawPixmap(copyDown, x, y + 34);
    nnDrawPixmap(copyRight, x + 91, y);
    nnDrawPixmap(copyRightDown, x + 91, y + 34);

    nnDrawPixmap(pixmapRot, 250, 350);
    nnRotatePixmap(pixmapRot, pixmapRot->angle + 10.0f * nnDT);

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

    image = nnLoadImage("./assets/opengl.png");
    pixmap = nnCreatePixmapFromImage(image);
    pixmapRot = nnCreatePixmapFromImage(image);

    copyDown = nnCopyPixmap(pixmap);
    nnFlipPixmap(copyDown, false, true);
    copyRight = nnCopyPixmap(pixmap);
    nnFlipPixmap(copyRight, true, false);
    copyRightDown = nnCopyPixmap(pixmap);
    nnFlipPixmap(copyRightDown, true, true);

    nnColorf pixel = nnReadPixel(pixmap, 18, 14);

    printf("pixmap pixel: %.2f, %.2f, %.2f, %.2f\n", pixel.r, pixel.g, pixel.b, pixel.a);
    printf("pixmap size: %d x %d\n", pixmap->width, pixmap->height);

    nnSetRenderFunc(render); // Set the render callback
    nnRun();                 // Start the rendering loop

    nnFreeImage(image);
    nnFreePixmap(pixmap);
    nnFreePixmap(copyDown);
    nnFreePixmap(copyRight);
    nnFreePixmap(copyRightDown);
    nnDestroyWindow(); // Cleanup resources
    return 0;
}
