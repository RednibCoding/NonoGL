#include <stdio.h>
#include <stdbool.h>

#define NONOGL_IMPLEMENTATION
#include "../nonogl/ngl.h"

void render()
{
    nnSetWindowTitle("NonoGL Window  FPS: %d", nnFPS);
    int mx = nnMousePosition().x;
    int my = nnMousePosition().y;

    float fillAmount = 0.0f;

    if (nnButton("Reset Progressbar", 120, 120, 200, 35))
    {
        fillAmount = -9999.0f; // High number
    }

    if (nnButton("<", 370, 125, 26, 26))
    {
        fillAmount = -20.0f;
    }
    if (nnButton(">", 585, 125, 26, 26))
    {
        fillAmount = 20.0f;
    }

    nnCheckbox("Test", false, 380, 255);

    int fillStatePercentage = nnHProgressbar(0, 220, fillAmount, 400, 128, 180);
    float textLen = nnTextWidth("Progress %d%%", fillStatePercentage);
    nnVProgressbar(0, 220, fillAmount, 490 + textLen / 2 + 5, 95, 25);
    nnDrawText("Progress %d%%", 490 - textLen / 2, 100, fillStatePercentage);

    nnCheckbox("Checkbox 1", true, 120, 180);

    float hSliderVal = nnHSlider(0.0f, 100.0f, 50.0f, 5.0f, 120, 220, 200);
    float vSliderVal = nnVSlider(0.0f, 100.0f, 50.0f, 5.0f, 80, 120, 200);

    nnDrawText("%.0f", 330, 220, hSliderVal);
    nnDrawText("%.0f", 40, 220 - nnTextHeight() / 2, vSliderVal);

    const char *options[] = {"Option 1", "Option 2", "Option 3", "Option 4", "Option 5", "Option 6", "Option 7", "Option 8", "Option 9", "Option 10", "Option 11", "Option 12"};
    // const char *options[] = {"Option 1", "Option 2", "Option 3", "Option 4"};
    nnDropdown("Select Option", options, 12, 400, 160, 150, 30);

    const char *items[] = {
        "Item 1", "Item 2", "Item 3", "Item 4", "Item 5",
        "Item 6", "Item 7", "Item 8", "Item 9", "Item 10",
        "Item 11", "Item 12", "Item 13", "Item 14", "Item 15 Very Long Text"};
    nnScrollableList(items, 15, 640, 130, 200, 250);

    nnDrawDebugText("mx: %d, my: %d", 10, 20, mx, my);
}

int main()
{
    nnSetDebugMode(true);

    // Create a window
    if (!nnCreateWindow("NonoGL Example", 1024, 600, true, false))
    {
        printf("Failed to create window.\n");
        return -1;
    }

    nnSetClearColor((nnColorf){0.1, 0.1, 0.1, 1.0});

    nnSetRenderFunc(render); // Set the render callback
    nnRun();                 // Start the rendering loop

    nnDestroyWindow(); // Cleanup resources
    return 0;
}
