#include <stdio.h>
#include <stdbool.h>

#define NONOGL_IMPLEMENTATION
#include "../nonogl/ngl.h"

void render()
{
    nnSetWindowTitle("NonoGL Window  FPS: %d", nnFPS);
    int mx = nnMousePosition().x;
    int my = nnMousePosition().y;

    if (nnKeyHit(nnVK_A))
    {
        printf("Key 'A' was just pressed!\n");
    }

    if (nnKeyDown(nnVK_A))
    {
        printf("Key 'A' is being held down!\n");
    }

    if (nnKeyReleased(nnVK_A))
    {
        printf("Key 'A' was just released!\n");
    }

    if (nnMouseHit(nnVK_MOUSE_LEFT))
    {
        printf("Left mouse button was just clicked!\n");
    }

    if (nnMouseDown(nnVK_MOUSE_LEFT))
    {
        printf("Left mouse button is being held down!\n");
    }

    if (nnMouseReleased(nnVK_MOUSE_LEFT))
    {
        printf("Left mouse button was just released!\n");
    }

    int wheelDelta = nnMouseWheelDelta();
    if (wheelDelta > 0)
    {
        printf("Mouse wheel moved up!\n");
    }
    else if (wheelDelta < 0)
    {
        printf("Mouse wheel moved down!\n");
    }

    nnPos pos = nnMousePosition();
    nnPos motionDelta = nnMouseMotionDelta();
    if (motionDelta.x != 0 || motionDelta.y != 0)
    {
        printf("Mouse moved: %d, %d | %d, %d \n", motionDelta.x, motionDelta.y, pos.x, pos.y);
    }

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

    nnSetRenderFunc(render); // Set the render callback
    nnRun();                 // Start the rendering loop

    nnDestroyWindow(); // Cleanup resources
    return 0;
}
