# NonoGL - A Tiny Graphics Library

NonoGL is a lightweight and simple 2D graphics library for C, designed for easy and quick setup of 2D rendering using OpenGL and FreeGLUT. It provides basic functionalities for window creation, image loading, text rendering, basic collision handling, and mouse and keyboard input handling.

## Features

- **Window Management**: Easy window creation and management.
- **Image Loading**: Load and render images. (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
- **Truetype Font Loading**: Load and render Truetype fonts.
- **Collision Handling**: Basic functions to check point-rectangle, rectangle-rectangle, point-circle, rectangle-circle and circle-circle collisions.
- **Mouse and Keyboard Input Handling**: Simple functions to check for mouse and keyboard button presses.

## Installation

1. **Clone the repository**:

   ```sh
   git clone git@github.com:RednibCoding/nonogl.git
   ```

2. **Copy the 'nonogl' folder into your project**.

3. **Compile your project with FreeGLUT and OpenGL**:

   > Note: NonoGL comes bundled with freeGlut, so there is no need to install freeGlut manually

   Use the following GCC command to compile your project:

   Example for Linux:

   ```sh
   gcc main.c -o main -nonogl/internal/lib/linux/x64 -l:libfreeglut_static.a -lGL -lGLU -lm -lpthread
   ```

   Example for Windows:

   ```sh
   gcc main.c -o main.exe -Lnonogl/internal/lib/win32/x64 -l:libfreeglut_static.a -lopengl32 -lwinmm -lgdi32 -lglu32
   ```

4. **Include the NonoGL header**:
   ```c
   #define NONOGL_IMPLEMENTATION
   #include "nonogl/ngl.h"
   ```
   Do this before you include this file in _one_ C file to create the implementation.
   In all other files just include `nonogl` without creating the implementations.
   ```c
   #include "nonogl/ngl.h"
   ```

## Example Usage

Here is a minimal example of using NonoGL to create a window and a render function:

```c
#include <stdio.h>
#include <stdbool.h>

#define NONOGL_IMPLEMENTATION // Also create the implementations
#include "nonogl/ngl.h"

void render()
{
    nnSetWindowTitle("NonoGL Window  FPS: %d", nnFPS);

    nnCls();

    // Draw stuff here

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

    nnSetRenderFunc(render); // Set the render callback
    nnRun();                 // Start the rendering loop

    nnDestroyWindow(); // Cleanup resources
    return 0;
}
```

## API Reference

### Structs

- **`nnPos`**
  Represents a 2D position.

  ```c
  typedef struct {
      int x; // X-coordinate
      int y; // Y-coordinate
  } nnPos;
  ```

- **`nnRecf`**
  Represents a 2D rectangle.

  ```c
  typedef struct {
      float x;      // X-coordinate of the rectangle's top-left corner
      float y;      // Y-coordinate of the rectangle's top-left corner
      float width;  // Width of the rectangle
      float height; // Height of the rectangle
  } nnRecf;
  ```

- **`nnColorf`**
  Represents an RGBA color.

  ```c
  typedef struct {
      float r; // Red component (0.0 to 1.0)
      float g; // Green component (0.0 to 1.0)
      float b; // Blue component (0.0 to 1.0)
      float a; // Alpha (transparency) component (0.0 to 1.0)
  } nnColorf;
  ```

- **`nnImage`**
  Represents an image loaded into OpenGL.

  ```c
  typedef struct {
      unsigned int textureID; // OpenGL texture ID
      int width;              // Width of the image
      int height;             // Height of the image
  } nnImage;
  ```

- **`nnPixmap`**
  Represents a 2D buffer of colors (`nnColorf`).

  ```c
  typedef struct
  {
    unsigned int textureID; // OpenGL texture ID
    int width;              // Width of the buffer
    int height;             // Height of the buffer
    nnColorf *pixels;       // CPU-side pixel buffer
  } nnPixmap;
  ```

- **`nnFont`**
  Represents a font loaded with stb_truetype.
  ```c
  typedef struct
  {
      stbtt_fontinfo fontInfo;
      unsigned char *fontBuffer;    // Holds the font file data
      unsigned int textureID;       // OpenGL texture for the font atlas
      int atlasWidth;               // Width of the font atlas
      int atlasHeight;              // Height of the font atlas
      float scale;                  // Font scaling factor
      stbtt_bakedchar charData[96]; // Holds character data for ASCII 32-127
  } nnFont;
  ```

### Window Management

- **`bool nnCreateWindow(char *title, int width, int height, bool scalable, bool filtered)`**
  Creates a window with the specified title, width and height. Scalable determines wether the drawn content on the screen should be resized when the window resizes.
  Filterd determines wether a smoothing filter should be applied to images and text. Otherwise they stay pixelated (good for pixel art).

- **`void nnSetRenderFunc(void (*callback)(void));`**
  Set the render callback function that NonoGL will call every frame

- **`void nnDestroyWindow()`**
  Destroys the created window and cleans up resources.

- **`void nnSetWindowTitle(const char *format, ...)`**
  Sets the window title with formatted text.

- **`void nnSetClearColor(nnColorf color)`**
  Sets the background color for clearing the window.

- **`void nnSetColor(nnColorf color)`**
  Sets the current drawing color.

- **`void nnResetColor()`**
  // Resets the color to default (1.0, 1.0, 1.0)

- **`void nnCls()`**
  Clears the screen with the current clear color.

- **`void nnFlip()`**
  Swaps the front- and backbuffer.

- **`void nnRun()`**
  Starts the main rendering loop.

### Image Loading and Drawing

- **`nnImage nnLoadImage(const char *filepath)`**
  Loads an image from the specified file path.

- **`nnImage nnLoadImageMem(const unsigned char *data, int size)`**
  Loads an image from memory.

- **`void nnDrawImage(nnImage image, int x, int y)`**
  Draws the loaded image at the specified position.

- **`void nnDrawImagePortion(nnImage image, int x, int y, nnRecf srcRec)`**
  Draws a portion of the loaded image.

- **`void nnFreeImage(nnImage image)`**
  Frees the given image

### Pixmap Management and Drawing

- **`nnPixmap *nnCreatePixmap(int width, int height)`**
  Creates a Pixmap with the given width and height.

- **`nnPixmap *nnCreatePixmapFromImage(nnImage image);`**
  Creates a Pixmap from an image.

- **`nnColorf nnReadPixel(nnPixmap *pixmap, int x, int y);`**
  Read a pixel from the given pixmap.

- **`void nnUpdatePixmap(nnPixmap *buffer)`**
  Update pixels that have changed in the given Pixmap. Only updates pixels that have actually changed.

- **`void nnDrawPixmap(nnPixmap *buffer, int x, int y)`**
  Draws the Pixmap to the screen at the given coordinates.

- **`void nnFreePixmap(nnPixmap *buffer)`**
  Frees the given Pixmap.

### Primitives drawing

- **`void nnPutPixel(float x, float y)`**
  Draws an individual pixel to the screen.

  > Note: When drawing large chunks of pixels, it is recommended to use a Pixmap instead for performance reasons.

- **`void nnDrawPixel(nnPixmap *buffer, int x, int y, nnColorf color)`**
  Writes a pixel at x, y location with the given color to the given Pixmap.

- **`void nnDrawPixel(nnPixmap *buffer, int x, int y, nnColorf color)`**
  Writes a pixel at x, y location with the given color to the given Pixmap.

- **`void nnDrawOval(nnPixmap *buffer, int x, int y, int width, int height, nnColorf color, bool filled)`**
  Draws an oval with its center at x, y.

- **`void nnDrawTriangle(nnPixmap *buffer, int x1, int y1, int x2, int y2, int x3, int y3, nnColorf color, bool filled)`**
  Draws a triangle.

- **`void nnDrawRect(nnPixmap *buffer, int x, int y, int width, int height, nnColorf color, bool filled)`**
  Draws a rectangle where x and y is the top left corner.

### Text Rendering

- **`nnFont *nnLoadFont(const char *filepath, float fontSize)`**
  Load a font from a .ttf file.

- **`void nnSetFont(nnFont *font)`**
  Set font for text rendering, if not set, a default non-scalable font will be used.

- **`void nnDrawText(const char *format, int x, int y, ...)`**
  Render the given formatted text using the font set with `nnSetFont`. If no font has been set, the default non-scalable font will be used.

- **`void nnFreeFont(nnFont *font)`**
  Free the given font.

### Collision Handling

- **`bool nnPosRecOverlaps(int x, int y, nnRecf rect)`**
  Checks if a point overlaps with a rectangle.

- **`bool nnRecsOverlap(nnRecf rec1, nnRecf rec2)`**
  Checks if two rectangles overlap.

- **`bool nnPosCircleOverlaps(int x, int y, int cx, int cy, float circleradius)`**
  Checks if a point overlaps with a circle.

- **`bool nnRecCircleOverlaps(nnRecf rec, int cx, int cy, float circleradius)`**
  Checks if a rectangle overlaps with a circle.

- **`bool nnCirclesOverlaps(int cx1, int cy1, float circle1radius, int cx2, int cy2, float circle2radius)`**
  Checks if two circles overlap.

### Input Handling

- **Keyboard Input**

  - **`bool nnKeyHit(int key)`**
    Returns `true` if the specified key was pressed since the last frame.

  - **`bool nnKeyDown(int key)`**
    Returns `true` if the specified key is currently being held down.

  - **`bool nnKeyReleased(int key)`**
    Returns `true` if the specified key was released since the last frame.

- **Mouse Input**

  - **`bool nnMouseHit(int button)`**
    Returns `true` if the specified mouse button was clicked since the last frame.

  - **`bool nnMouseDown(int button)`**
    Returns `true` if the specified mouse button is currently being held down.

  - **`bool nnMouseReleased(int button)`**
    Returns `true` if the specified mouse button was released since the last frame.

  - **`int nnMouseWheelDelta()`**
    Returns the direction of the mouse wheel movement: `positive number` means up, `negative number` means down, and `0` if no movement occurred.

  - **`nnPos nnGetMousePosition()`**
    Returns the current position of the mouse cursor as an `nnPos` struct.

  - **`nnPos nnMouseMotionDelta()`**
    Returns the mouse motion delta (change in position) since the last frame.

### Utility

- **`unsigned char *nnLoadFileBytes(const char *filepath, int *size)`**
  Loads a file into a buffer and returns its pointer.

- **`void nnFreeFileBytes(unsigned char *buffer)`**
  Frees the memory allocated by `nnLoadFileBytes`.

- **`int nnFPS`**
  Holds the current frames per second.

- **`float nnDT`**
  Holds the delta time (time elapsed since the last frame).

## License

NonoGL is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
