
# MiniG - Mini Graphics Library

MiniG (Mini Graphics Library) is a lightweight and simple graphics library for C, designed for easy and quick setup of 2D rendering using OpenGL and GLFW. It provides basic functionalities for window creation, image loading, text rendering, basic collisions handling and mouse and keyboard input handling.

## Features

- **Window Management**: Easy creation and management of OpenGL windows using GLFW.
- **Image Loading**: Load and render PNG images using stb_image.
- **Collision handling**: Basic functions to check point-rectangle, rectangle-rectangle, point-circle, and rectangle-circle overlaps.
- **Mouse and Keyboard input handling**: Simple functions to check for mouse and keyboard button presses.

## Installation

1. **Clone the repository**:
    ```sh
    git clone <repository_url>
    cd minig
    ```

2. **Copy the 'minig' folder into your project**

3. **Compile your project with GLFW and OpenGL**:
    Make sure to link against the GLFW (see the lib directory) and OpenGL libraries.

    ```sh
    gcc -o main main.c -lglfw -lGL -I./minig/lib/<your-arch>
    ```

4. **Include the MiniG header**:
    ```c
    #include "minig/mg.h"
    ```

## Example Usage

Here's a basic example of using MiniG to create a window and render text:

```c
#include <stdio.h>
#include <stdbool.h>

#define MG_IMPL
#include "minig/mg.h"

int main() {
    bool success = mgCreateWindow("MiniG Window", 1024, 600, true, false);
    if (!success) return -1;

    mgImage image = mgLoadImage("assets/GLFW.png");

    while (!mgWindowShouldClose()) {
        mgClear();

        mgDrawImage(image, (mgPointf){20.0f, 20.0f});
        mgDrawImagePortion(image, (mgPointf){200.0f, 200.0f}, (mgRecf){0.0f, 0.0f, 50.0f, 50.0f});

        mgSwap();
    }

    mgDestroyWindow();
}
```

## API Reference

### Window Management

- `bool mgCreateWindow(char *title, int width, int height, bool scaleable, bool filtered)`
  Creates a window with the specified title, width, and height.

- `void mgDestroyWindow()`
  Destroys the created window and cleans up resources.

- `bool mgWindowShouldClose()`
  Returns `true` if the window should close, `false` otherwise.

- `void mgClear()`
  Clears the window with the specified background color.

- `void mgSwap()`
  Swaps the front and back buffers.

### Image Loading and Drawing

- `mgImage mgLoadImage(const char* filepath)`
  Loads a PNG image from the specified file path.

- `void mgDrawImage(mgImage image, mgPointf pos)`
  Draws the loaded image at the specified position.

- `void mgDrawImagePortion(mgImage image, mgPointf pos, mgRecf srcRec)`
  Draws a portion of the loaded image.

### Text Rendering
- TODO

### Collision

- `bool mgPointRecOverlaps(mgPointf point, mgRecf rec)`
  Checks if a point overlaps with a rectangle.

- `bool mgRecsOverlap(mgRecf rec1, mgRecf rec2)`
  Checks if two rectangles overlap.

- `bool mgPointCircleOverlaps(mgPointf point, mgPointf circleCenter, float circleRadius)`
  Checks if a point overlaps with a circle.

- `bool mgRecCircleOverlaps(mgRecf rec, mgPointf circleCenter, float circleRadius)`
  Checks if a rectangle overlaps with a circle.

- `bool mgCirclesOverlap(mgPointf circle1Center, float circle1Radius, mgPointf circle2Center, float circle2Radius)`
  Checks if two circles overlap.

## License

MiniG is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
