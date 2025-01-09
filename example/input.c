#include <stdio.h>
#include <stdbool.h>

#define NONOGL_IMPLEMENTATION
#include "../nonogl/ngl.h"

void render()
{
    nnSetWindowTitle("NonoGL Window  FPS: %d", nnFPS);

    if (nnKeyHit('a'))
    {
        printf("Key 'A' was just pressed!\n");
    }

    if (nnKeyDown('a'))
    {
        printf("Key 'A' is being held down!\n");
    }

    if (nnKeyReleased('a'))
    {
        printf("Key 'A' was just released!\n");
    }

    if (nnMouseHit(0))
    {
        printf("Left mouse button was just clicked!\n");
    }

    if (nnMouseDown(0))
    {
        printf("Left mouse button is being held down!\n");
    }

    if (nnMouseReleased(0))
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

    nnPos pos = nnGetMousePosition();
    nnPos motionDelta = nnMouseMotionDelta();
    if (motionDelta.x != 0 || motionDelta.y != 0)
    {
        printf("Mouse moved: %d, %d | %d, %d \n", motionDelta.x, motionDelta.y, pos.x, pos.y);
    }
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
