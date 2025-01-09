#include <stdio.h>
#include <stdbool.h>

#define NONOGL_IMPLEMENTATION
#include "../nonogl/ngl.h"

nnImage image;

void render()
{
    nnSetWindowTitle("NonoGL Window  FPS: %d", nnFPS);

    nnDrawImage(image, 200, 120);
    nnDrawImagePortion(image, 200, 200, (nnRecf){0.0f, 0.0f, image.width / 2, image.height});

    static unsigned int lastTime = 0;
    if (nnMS - lastTime >= 1000)
    {
        nnFlipImage(&image, true, false);
        lastTime = nnMS;
    }

    nnRotateImage(&image, image.angle + 20.0f * nnDT);
}

int main()
{
    // Create a window
    if (!nnCreateWindow("NonoGL Example", 800, 600, true, true))
    {
        printf("Failed to create window.\n");
        return -1;
    }

    image = nnLoadImage("./assets/opengl.png");

    nnSetRenderFunc(render); // Set the render callback
    nnRun();                 // Start the rendering loop

    nnFreeImage(image);
    nnDestroyWindow(); // Cleanup resources
    return 0;
}
