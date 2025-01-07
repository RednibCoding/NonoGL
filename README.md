# TinyGL - Tiny Graphics Library

TinyGL (Tiny Graphics Library) is a lightweight and simple graphics library for C, designed for easy and quick setup of 2D rendering using OpenGL and FreeGLUT. It provides basic functionalities for window creation, image loading, text rendering, basic collision handling, and mouse and keyboard input handling.

## Features

- **Window Management**: Easy creation and management.
- **Image Loading**: Load and render images. (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
- **Collision Handling**: Basic functions to check point-rectangle, rectangle-rectangle, point-circle, and rectangle-circle overlaps.
- **Mouse and Keyboard Input Handling**: Simple functions to check for mouse and keyboard button presses.

## Installation

1. **Clone the repository**:

   ```sh
   git clone git@github.com:RednibCoding/tinygl.git
   ```

2. **Copy the 'tinygl' folder into your project**.

3. **Compile your project with FreeGLUT and OpenGL**:

   > Note: freeGlut comes with TinyGL, so there is no need to install it manually

   Use the following GCC command to compile your project:

   Example for Linux:

   ```sh
   gcc main.c -o main -Ltinygl/internal/lib/x64 -l:libfreeglut_static.a -lGL -lGLU -lm -lpthread
   ```

   Example for Windows:

   ```sh
   gcc main.c -o main.exe -Ltinygl/internal/lib/x64 -l:libfreeglut_static.a -lopengl32 -lwinmm -lgdi32 -lglu32
   ```

4. **Include the TinyGL header**:
   ```c
   #define TINYGL_IMPLEMENTATION
   #include "tinygl/tgl.h"
   ```
   Do this before you include this file in _one_ C file to create the implementation.
   In all other files just include `tinygl` without creating the implementations.
   ```c
   #include "tinygl/tgl.h"
   ```

## Example Usage

Here is a basic example of using TinyGL to create a window and render images and text:

```c
#include <stdio.h>
#include <stdbool.h>

#define TINYGL_IMPLEMENTATION // Also create the implementations
#include "tinygl/tgl.h"

tgImage image;

void display()
{
    tgCls();

    tgDrawImage(image, 20, 20);

    tgDrawImagePortion(image, 200, 200}, (tgRecf){0.0f, 0.0f, 50.0f, 50.0f});

    tgSetColor((tgColorf){0.5f, 0.5f, 0.8f, 1.0f});
    tgDrawText("Hello World", 200, 300});

    tgSetColor((tgColorf){1.5f, 0.5f, 0.8f, 1.0f});
    tgDrawText("Hello Color", 200, 340});

    tgSetColor((tgColorf){1.0f, 1.0f, 1.0f, 1.0f});
    tgDrawText("FPS: %d", 200, 360}, tgFPS);
    tgDrawText("Delta Time: %f", 200, 400}, tgDT);

    tgFlip();
}

int main()
{
    bool success = tgCreateWindow("TinyGL Window", 1024, 600, true, true);
    if (!success)
        return -1;

    image = tgLoadImage("./assets/opengl.png");

    tgSetDisplayLoop(display);
    tgRun();

    tgDestroyWindow();
    return 0;
}
```

## API Reference

### Window Management

- **`bool tgCreateWindow(char *title, int width, int height, bool scalable, bool filtered)`**
  Creates a window with the specified title, width, and height. Scalable determines wether the content should be resized when the window resizes. Filterd determines wether images should be filtered or pixelated.

- **`void tgDestroyWindow()`**
  Destroys the created window and cleans up resources.

- **`void tgSetWindowTitle(const char *format, ...)`**
  Sets the window title with formatted text.

- **`void tgSetClearColor(tgColorf color)`**
  Sets the background color for clearing the window.

- **`void tgSetColor(tgColorf color)`**
  Sets the current drawing color.

- **`void tgResetColor()`**
  // Resets the color to default (1.0, 1.0, 1.0)

- **`void tgCls()`**
  Clears the screen with the current clear color.

- **`void tgFlip()`**
  Swaps the front- and backbuffer.

- **`void tgSetDisplayLoop(void (*callback)(void))`**
  Sets the display loop function to handle rendering.

- **`void tgRun()`**
  Starts the main rendering loop.

### Image Loading and Drawing

- **`tgImage tgLoadImage(const char *filepath)`**
  Loads an image from the specified file path.

- **`tgImage tgLoadImageMem(const unsigned char *data, int size)`**
  Loads an image from memory.

- **`void tgDrawImage(tgImage image, int x, int y)`**
  Draws the loaded image at the specified position.

- **`void tgDrawImagePortion(tgImage image, int x, int y, tgRecf srcRec)`**
  Draws a portion of the loaded image.

- **`void tgFreeImage(tgImage image)`**
  Frees the given image

### Pixmap (pixel buffer) Management and Drawing

- **`tgPixmap *tgCreatePixmap(int width, int height)`**
  Creates a Pixmap with the given width and height.

- **`void tgPutPixel(tgPixmap *buffer, int x, int y, tgColorf color)`**
  Writes a pixel at x, y location with the given color to the given Pixmap.

- **`void tgUpdatePixmap(tgPixmap *buffer)`**
  Update pixels that have changed in the given Pixmap.

- **`void tgDrawPixmap(tgPixmap *buffer)`**
  Draws the Pixmap to the screen.

- **`void tgFreePixmap(tgPixmap *buffer)`**
  Frees the given Pixmap.

- **`void drawPixel(float x, float y, tgColorf color)`**
  Draws an individual pixel to the screen.
  > Note: When drawing large chunks of pixels, it is recommended to use a Pixmap instead for performance reasons.

### Text Rendering

- **`tgFont *mgLoadFont(const char *filepath, float fontSize)`**
  Load a font from a file.

- **`void tgSetFont(tgFont *font)`**
  Set font for text rendering, if not set, a default non-scalable font will be used.

- **`void tgDrawText(const char *format, int x, int y, ...)`**
  Renders formatted text at the specified position.

- **`void tgFreeFont(tgFont *font)`**
  Free the font

### Collision Detection

- **`bool tgVec2RecOverlaps(int x, int y, tgRecf rect)`**
  Checks if a point overlaps with a rectangle.

- **`bool tgRecsOverlap(tgRecf rec1, tgRecf rec2)`**
  Checks if two rectangles overlap.

- **`bool tgVec2CircleOverlaps(int x, int y, int cx, int cy, float circleradius)`**
  Checks if a point overlaps with a circle.

- **`bool tgRecCircleOverlaps(tgRecf rec, int cx, int cy, float circleradius)`**
  Checks if a rectangle overlaps with a circle.

- **`bool tgCirclesOverlaps(int cx1, int cy1, float circle1radius, int cx2, int cy2, float circle2radius)`**
  Checks if two circles overlap.

### Input Handling

- **Keyboard Input**

  - **`bool tgKeyHit(int key)`**
    Returns `true` if the specified key was pressed since the last frame.

  - **`bool tgKeyDown(int key)`**
    Returns `true` if the specified key is currently being held down.

  - **`bool tgKeyReleased(int key)`**
    Returns `true` if the specified key was released since the last frame.

- **Mouse Input**

  - **`bool tgMouseHit(int button)`**
    Returns `true` if the specified mouse button was clicked since the last frame.

  - **`bool tgMouseDown(int button)`**
    Returns `true` if the specified mouse button is currently being held down.

  - **`bool tgMouseReleased(int button)`**
    Returns `true` if the specified mouse button was released since the last frame.

  - **`int tgMouseWheelDelta()`**
    Returns the direction of the mouse wheel movement: `1` for up, `-1` for down, and `0` if no movement occurred.

  - **`tgVec2 tgGetMousePosition()`**
    Returns the current position of the mouse cursor as an `tgVec2` struct.

  - **`tgVec2 tgMouseMotionDelta()`**
    Returns the mouse motion delta (change in position) since the last frame.

### Utility Functions

- **`unsigned char *tgLoadFileBytes(const char *filepath, int *size)`**
  Loads a file into a buffer and returns its pointer.

- **`void tgFreeFileBytes(unsigned char *buffer)`**
  Frees the memory allocated by `tgLoadFileBytes`.

- **`int tgFPS`**
  Holds the current frames per second.

- **`float tgDT`**
  Holds the delta time (time elapsed since the last frame).

### Structs

- **`tgVec2`**
  Represents a 2D point.

  ```c
  typedef struct {
      int x; // X-coordinate
      int y; // Y-coordinate
  } tgVec2;
  ```

- **`tgRecf`**
  Represents a 2D rectangle.

  ```c
  typedef struct {
      float x;      // X-coordinate of the rectangle's top-left corner
      float y;      // Y-coordinate of the rectangle's top-left corner
      float width;  // Width of the rectangle
      float height; // Height of the rectangle
  } tgRecf;
  ```

- **`tgColorf`**
  Represents an RGBA color.

  ```c
  typedef struct {
      float r; // Red component (0.0 to 1.0)
      float g; // Green component (0.0 to 1.0)
      float b; // Blue component (0.0 to 1.0)
      float a; // Alpha (transparency) component (0.0 to 1.0)
  } tgColorf;
  ```

- **`tgImage`**
  Represents an image loaded into OpenGL.

  ```c
  typedef struct {
      unsigned int id; // OpenGL texture ID
      int width;       // Width of the image
      int height;      // Height of the image
  } tgImage;
  ```

- **`tgPixmap`**
  Represents a 2D buffer of colors (`tgColorf`)
  ```c
  typedef struct
  {
    unsigned int textureID; // OpenGL texture ID
    int width;              // Width of the buffer
    int height;             // Height of the buffer
    tgColorf *pixels;       // CPU-side pixel buffer
  } tgPixmap;
  ```

## License

TinyG is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
