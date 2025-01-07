# MiniG - Mini Graphics Library

MiniG (Mini Graphics Library) is a lightweight and simple graphics library for C, designed for easy and quick setup of 2D rendering using OpenGL and FreeGLUT. It provides basic functionalities for window creation, image loading, text rendering, basic collision handling, and mouse and keyboard input handling.

## Features

- **Window Management**: Easy creation and management of OpenGL windows using FreeGLUT.
- **Image Loading**: Load and render images. (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
- **Collision Handling**: Basic functions to check point-rectangle, rectangle-rectangle, point-circle, and rectangle-circle overlaps.
- **Mouse and Keyboard Input Handling**: Simple functions to check for mouse and keyboard button presses.

## Installation

1. **Clone the repository**:
    ```sh
    git clone git@github.com:RednibCoding/miniG.git
    ```

2. **Copy the 'minig' folder into your project**.

3. **Compile your project with FreeGLUT and OpenGL**:
    > Note: freeGlut comes with miniG, so there is no need to install it manually

    Use the following GCC command to compile your project:

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
  Loads an image from the specified file path.

- **`mgImage mgLoadImageMem(const unsigned char *data, int size)`**
  Loads an image from memory.

- **`void mgDrawImage(mgImage image, mgPointf pos)`**
  Draws the loaded image at the specified position.

- **`void mgDrawImagePortion(mgImage image, mgPointf pos, mgRecf srcRec)`**
  Draws a portion of the loaded image.

### Pixelbuffer Management and Drawing

- **`mgPixelBuffer *mgCreatePixelBuffer(int width, int height);`**
  Creates a pixel buffer with the given width and height.

- **`void mgPutPixel(mgPixelBuffer *buffer, int x, int y, mgColorf color);`**
  Writes a pixel at x, y location with the given color to the given pixel buffer.

- **`void mgUpdatePixelBuffer(mgPixelBuffer *buffer);`**
  Update pixels that have changed in the given pixel buffer.

- **`void mgDrawPixelBuffer(mgPixelBuffer *buffer);`**
  Draws the pixel buffer to the screen.

- **`void mgFreePixelBuffer(mgPixelBuffer *buffer);`**
  Frees the given pixel buffer.

- **`void drawPixel(float x, float y, mgColorf color);`**
  Draw an individual pixel to the screen (use a pixel buffer for large chunks of pixels instead)

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

### Input Handling

- **Keyboard Input**
  - **`bool mgKeyHit(int key)`**
    Returns `true` if the specified key was pressed since the last frame.

  - **`bool mgKeyDown(int key)`**
    Returns `true` if the specified key is currently being held down.

  - **`bool mgKeyReleased(int key)`**
    Returns `true` if the specified key was released since the last frame.

- **Mouse Input**
  - **`bool mgMouseHit(int button)`**
    Returns `true` if the specified mouse button was clicked since the last frame.

  - **`bool mgMouseDown(int button)`**
    Returns `true` if the specified mouse button is currently being held down.

  - **`bool mgMouseReleased(int button)`**
    Returns `true` if the specified mouse button was released since the last frame.

  - **`int mgMouseWheelDelta()`**
    Returns the direction of the mouse wheel movement: `1` for up, `-1` for down, and `0` if no movement occurred.

  - **`mgPointf mgGetMousePosition()`**
    Returns the current position of the mouse cursor as an `mgPointf` struct.

  - **`mgPointf mgMouseMotionDelta()`**
    Returns the mouse motion delta (change in position) since the last frame.

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
  Represents a 2D point.
  ```c
  typedef struct {
      float x; // X-coordinate
      float y; // Y-coordinate
  } mgPointf;
  ```

- **`mgRecf`**
  Represents a 2D rectangle.
  ```c
  typedef struct {
      float x;      // X-coordinate of the rectangle's top-left corner
      float y;      // Y-coordinate of the rectangle's top-left corner
      float width;  // Width of the rectangle
      float height; // Height of the rectangle
  } mgRecf;
  ```

- **`mgColorf`**
  Represents an RGBA color.
  ```c
  typedef struct {
      float r; // Red component (0.0 to 1.0)
      float g; // Green component (0.0 to 1.0)
      float b; // Blue component (0.0 to 1.0)
      float a; // Alpha (transparency) component (0.0 to 1.0)
  } mgColorf;
  ```

- **`mgImage`**
  Represents an image loaded into OpenGL.
  ```c
  typedef struct {
      unsigned int id; // OpenGL texture ID
      int width;       // Width of the image
      int height;      // Height of the image
  } mgImage;
  ```

- **`mgPixelBuffer`**
  Represents a 2D buffer of colors (`mgColorf`)
  ```c
  typedef struct
  {
    unsigned int textureID; // OpenGL texture ID
    int width;              // Width of the buffer
    int height;             // Height of the buffer
    mgColorf *pixels;       // CPU-side pixel buffer
  } mgPixelBuffer;
  ```

## License

MiniG is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.