# MiniG - Mini Graphics Library

MiniG (Mini Graphics Library) is a lightweight and simple graphics library for C, designed for easy and quick setup of 2D rendering using OpenGL and FreeGLUT. It provides basic functionalities for window creation, image loading, text rendering, basic collision handling, and mouse and keyboard input handling.

## Features

- **Window Management**: Easy creation and management of OpenGL windows using FreeGLUT.
- **Image Loading**: Load and render PNG images using stb_image.
- **Collision Handling**: Basic functions to check point-rectangle, rectangle-rectangle, point-circle, and rectangle-circle overlaps.
- **Mouse and Keyboard Input Handling**: Simple functions to check for mouse and keyboard button presses.

## Installation

1. **Clone the repository**:
    ```sh
    git clone git@github.com:RednibCoding/miniG.git
    ```

2. **Copy the 'minig' folder into your project**.

3. **Compile your project with FreeGLUT and OpenGL**:
    Ensure you link against FreeGLUT and OpenGL libraries. (Comes with minig)

    Example for Linux:
    ```sh
    gcc main.c -o main -Lminig/internal/lib/x64 -l:libfreeglut_static.a -lGL -lGLU -lm -lpthread
    ```

    Example for Windows:
    ```sh
    gcc main.c -o main.exe -Lminig/internal/lib/x64 -l:libfreeglut_static.a -lopengl32 -lwinmm -lgdi32 -lglu32
    ```

4. **Include the MiniG header**:
    ```c
    #include "minig/mg.h"
    ```

## Example Usage

Here is a basic example of using MiniG to create a window and render images and text:

```c
#include <stdio.h>
#include <stdbool.h>

#define MG_IMPL
#include "minig/mg.h"

mgImage image;

void display()
{
    mgCls();

    mgDrawImage(image, (mgPointf){20.0f, 20.0f});

    mgDrawImagePortion(image, (mgPointf){200.0f, 200.0f}, (mgRecf){0.0f, 0.0f, 50.0f, 50.0f});

    mgSetColor((mgColorf){0.5f, 0.5f, 0.8f, 1.0f});
    mgDrawText("Hello World", (mgPointf){200.0f, 300.0f});

    mgSetColor((mgColorf){1.5f, 0.5f, 0.8f, 1.0f});
    mgDrawText("Hello Color", (mgPointf){200.0f, 340.0f});

    mgSetColor((mgColorf){1.0f, 1.0f, 1.0f, 1.0f});
    mgDrawText("FPS: %d", (mgPointf){200.0f, 360.0f}, mgFPS);
    mgDrawText("Delta Time: %f", (mgPointf){200.0f, 400.0f}, mgDT);

    mgFlip();
}

int main()
{
    bool success = mgCreateWindow("MiniG Window", 1024, 600, true, true);
    if (!success)
        return -1;

    image = mgLoadImage("./assets/opengl.png");

    mgSetDisplayLoop(display);
    mgRun();

    mgDestroyWindow();
    return 0;
}
```

## API Reference

### Window Management

- **`bool mgCreateWindow(char *title, int width, int height, bool scalable, bool filtered)`**
  Creates a window with the specified title, width, and height. Scalable determines wether the content should be resized when the window resizes. Filterd determines wether images should be filtered or pixelated.

- **`void mgDestroyWindow()`**
  Destroys the created window and cleans up resources.

- **`void mgSetWindowTitle(const char *format, ...)`**
  Sets the window title with formatted text.

- **`void mgSetClearColor(mgColorf color)`**
  Sets the background color for clearing the window.

- **`void mgSetColor(mgColorf color)`**
  Sets the current drawing color.

- **`void mgCls()`**
  Clears the screen with the current clear color.

- **`void mgFlip()`**
  Swaps the front and back buffers.

- **`void mgSetDisplayLoop(void (*callback)(void))`**
  Sets the display loop function to handle rendering.

- **`void mgRun()`**
  Starts the main rendering loop.

### Image Loading and Drawing

- **`mgImage mgLoadImage(const char *filepath)`**
  Loads an image from the specified file path. (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)

- **`mgImage mgLoadImageMem(const unsigned char *data, int size)`**
  Loads an image from memory. (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)

- **`void mgDrawImage(mgImage image, mgPointf pos)`**
  Draws the loaded image at the specified position.

- **`void mgDrawImagePortion(mgImage image, mgPointf pos, mgRecf srcRec)`**
  Draws a portion of the loaded image.

### Text Rendering

- **`void mgDrawText(const char *format, mgPointf pos, ...)`**
  Renders formatted text at the specified position.

### Collision Detection

- **`bool mgPointRecOverlaps(mgPointf point, mgRecf rect)`**
  Checks if a point overlaps with a rectangle.

- **`bool mgRecsOverlap(mgRecf rec1, mgRecf rec2)`**
  Checks if two rectangles overlap.

- **`bool mgPointCircleOverlaps(mgPointf point, mgPointf circlecenter, float circleradius)`**
  Checks if a point overlaps with a circle.

- **`bool mgRecCircleOverlaps(mgRecf rec, mgPointf circlecenter, float circleradius)`**
  Checks if a rectangle overlaps with a circle.

- **`bool mgCirclesOverlaps(mgPointf circle1center, float circle1radius, mgPointf circle2center, float circle2radius)`**
  Checks if two circles overlap.

### Utility Functions

- **`unsigned char *mgLoadFileBytes(const char *filepath, int *size)`**
  Loads a file into a buffer and returns its pointer.

- **`void mgFreeFileBytes(unsigned char *buffer)`**
  Frees the memory allocated by `mgLoadFileBytes`.

- **`int mgFPS`**
  Holds the current frames per second.

- **`float mgDT`**
  Holds the delta time (time elapsed since the last frame).

### Structs
- **`mgPointf`** 
```c
  typedef struct
  {
      float x;
      float y;
  } mgPointf;
  ```

- **`mgRecf`**
```c
  typedef struct
  {
      float x;
      float y;
      float width;
      float height;
  } mgRecf;
```
- **`mgColorf`**
```c
typedef struct
{
    float r;
    float g;
    float b;
    float a;
} mgColorf;
```
- **`mgImage`**
```c
typedef struct
{
    unsigned int id;
    int width;
    int height;
} mgImage;
```

## License

MiniG is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

