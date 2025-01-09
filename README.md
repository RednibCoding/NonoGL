# NonoGL - A Tiny Game Library

NonoGL is a lightweight and simple 2D game library for C, designed for easy and quick setup of 2D rendering using OpenGL and FreeGLUT. It provides basic functionalities for window creation, image loading, text rendering, basic collision handling, and mouse and keyboard input handling.

## Features

- **Window Management**: Easy window creation and management.
- **Image Loading**: Load and render images. (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
- **Truetype Font Loading**: Load and render Truetype fonts.
- **Collision Handling**: Basic functions to check point-rectangle, rectangle-rectangle, point-circle, rectangle-circle and circle-circle collisions.
- **Mouse and Keyboard Input Handling**: Simple functions to check for mouse and keyboard button presses.
- **Basic GUI Elements**: Comes with a small list of themable gui elements.

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
   gcc main.c -o main -lnonogl/internal/lib/linux/x64 -llibfreeglut_static -lGL -lGLU -lm -lpthread
   ```

   Example for Windows:

   ```sh
   gcc gui.c -o main.exe -Lnonogl/internal/lib/win32/x64 -lfreeglut_static -lopengl32 -lwinmm -lgdi32 -lglu32
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
    // Draw stuff here
}

int main()
{
    // Create a window
    if (!nnCreateWindow("NonoGL Window", 800, 600, false, false))
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
  typedef struct
  {
    unsigned int textureID; // OpenGL texture ID
    int width;              // Width of the image
    int height;             // Height of the buffer
    float scaleX;           // Scale in x axies
    float scaleY;           // Scale in x axies
    bool isFlippedX;        // Is x-axies flipped
    bool isFlippedY;        // Is y-axies flipped
    float angle;            // Rotation angle in degrees
  } nnImage;
  ```

- **`nnPixmap`**
  Represents a 2D buffer of colors (`nnColorf`), for fast pixel manipulations.

  ```c
  typedef struct
  {
    unsigned int textureID; // OpenGL texture ID
    int width;              // Width of the pixmap
    int height;             // Height of the pixmap
    float scaleX;           // Scale in x axies
    float scaleY;           // Scale in x axies
    bool isFlippedX;        // Is x-axies flipped
    bool isFlippedY;        // Is y-axies flipped
    float angle;            // Rotation angle in degrees
    nnColorf *pixels;       // CPU-side pixel pixmap
  } nnPixmap;
  ```

- **`nnFont`**
  Represents a font loaded with stb_truetype.

  ```c
  typedef struct
  {
      stbtt_fontinfo fontInfo;
      float glyphHeight;            // Height of the loaded font
      unsigned char *fontBuffer;    // Holds the font file data
      unsigned int textureID;       // OpenGL texture for the font atlas
      int atlasWidth;               // Width of the font atlas
      int atlasHeight;              // Height of the font atlas
      float scale;                  // Font scaling factor
      stbtt_bakedchar charData[96]; // Holds character data for ASCII 32-127
  } nnFont;
  ```

  - **`nnTheme`**
    Theme used by gui elements

  ```c
  typedef struct
  {
      nnColorf primaryColor;         // Main color for elements
      nnColorf primaryColorAccent;   // Accent color for primary elements
      nnColorf secondaryColor;       // Secondary elements (e.g., backgrounds)
      nnColorf secondaryColorAccent; // Accent color for secondary elements
      nnColorf borderColor;          // Border color
      nnColorf borderColorAccent;    // Accent border color
      nnColorf textPrimaryColor;     // Primary text color
      nnColorf textSecondaryColor;   // Secondary text color
  } nnTheme;
  ```

### Window Management

- **`bool nnCreateWindow(char *title, int width, int height, bool virtual, bool filtered)`**
  Creates a window with the specified title, width and height. Virtual determines whether to use a fixed, virtual coordinate system for rendering, regardless of the actual size of the window.
  Filterd determines wether a smoothing filter should be applied to images and text. Otherwise they stay pixelated (good for pixel art).

- **`void nnSetRenderFunc(void (*callback)(void));`**
  Set the render callback function that NonoGL will call every frame

- **`void nnDestroyWindow()`**
  Destroys the created window and cleans up resources.

- **`void nnSetTargetFPS(int fps)`**
  Sets the target FPS.

- **`void nnSetWindowTitle(const char *format, ...)`**
  Sets the window title with formatted text.

- **`void nnSetClearColor(nnColorf color)`**
  Sets the background color for clearing the window.

- **`void nnSetColor(nnColorf color)`**
  Sets the current drawing color.

- **`nnColorf nnGetColor()`**
  Get the current draw color.

- **`void nnResetColor()`**
  Resets the color to default (1.0, 1.0, 1.0, 1.0)

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
  Draws a portion of the loaded image at the specified position.

- **`void nnFlipImage(nnImage image, bool flipX, bool flipY)`**
  Flips the given image on the given axies.

- **`void nnRotateImage(nnImage *image, float degrees)`**
  Sets the rotation of the given image to the given degrees.

- **`void nnScaleImage(nnImage *image, float scaleX, float scaleY)`**
  Sets the scale of the given image to the given scale values.

- **`void nnFreeImage(nnImage image)`**
  Frees the given image.

### Pixmap Management and Drawing

- **`nnPixmap *nnCreatePixmap(int width, int height)`**
  Creates a pixmap with the given width and height.

- **`nnPixmap *nnCreatePixmapFromImage(nnImage image);`**
  Creates a pixmap from an image.

- **`nnColorf nnReadPixel(nnPixmap *pixmap, int x, int y);`**
  Read a pixel from the given pixmap.

- **`void nnUpdatePixmap(nnPixmap *pixmap)`**
  Update pixels that have changed in the given pixmap. Only updates pixels that have actually changed.

- **`void nnDrawPixmap(nnPixmap *pixmap, int x, int y)`**
  Draws the pixmap to the screen at the given coordinates.

- **`void nnFlipPixmap(nnPixmap *pixmap, bool flipX, bool flipY)`**
  Flips the given pixmap on the given axies.

- **`void nnRotatePixmap(nnPixmap *pixmap, float angle)`**
  Sets the rotateation of the given pixmap to the given degrees.

- **`void nnScalePixmap(nnPixmap *pixmap, float scaleX, float scaleY)`**
  Sets the scale of the given pixmap to the given scale values.

- **`nnPixmap *nnCopyPixmap(nnPixmap *pixmap)`**
  Returns a copy of the given pixmap.

- **`void nnFreePixmap(nnPixmap *pixmap)`**
  Frees the given pixmap.

### Primitives Drawing

- **`void nnPutPixel(float x, float y)`**
  Draws an individual pixel to the screen.

  > Note: When drawing large chunks of pixels, it is recommended to use a pixmap instead for performance reasons.

- **`void nnDrawPixel(nnPixmap *pixmap, int x, int y, nnColorf color)`**
  Writes a pixel at x, y location with the given color to the given pixmap.

- **`void nnDrawLine(nnPixmap *pixmap, int x0, int y0, int x1, int y1, nnColorf color)`**
  Draws a line from x0, y0 to x1, y1

- **`void nnDrawOval(nnPixmap *pixmap, int x, int y, int width, int height, nnColorf color, bool filled)`**
  Draws an oval with its center at x, y.

- **`void nnDrawTriangle(nnPixmap *pixmap, int x1, int y1, int x2, int y2, int x3, int y3, nnColorf color, bool filled)`**
  Draws a triangle.

- **`void nnDrawRect(nnPixmap *pixmap, int x, int y, int width, int height, nnColorf color, bool filled)`**
  Draws a rectangle where x and y is the top left corner.

### Text Rendering

- **`nnFont *nnLoadFont(const char *filepath, float fontSize)`**
  Load a ttf font from a .ttf file.

- **`nnFont *nnLoadFontMem(const unsigned char *data, size_t dataSize, float fontSize);`**
  Load a ttf font from memory.

- **`void nnSetFont(nnFont *font)`**
  Set font for text rendering.

- **`nnFont *nnGetFont()`**
  Get the font that is currently set.

- **`void nnDrawText(const char *format, int x, int y, ...)`**
  Render the given formatted text using the font that has been set with `nnSetFont`. If no font has been set, the internal default font will be used.

- **`float nnTextWidth(const char *format, ...)`**
  Returns the width in pixels of the given formatted text regarding the current font.

- **`float nnTextHeight()`**
  Returns the height in pixels of the current font.

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

- **`float nnLerp(float min, float max, float speed, float ease)`**
  Lerps between min and max by the given speed. Use ease to define a smooth transition when changing direction.

- **`int nnFPS`**
  Holds the current frames per second.

- **`float nnDT`**
  Holds the delta time (time elapsed since the last frame).

- **`float nnMS`**
  Time in milliseconds since the application started.

### GUI

- **`bool nnButton(const char *format, int x, int y, int width, int height, ...)`**
  Button that returns `true` when it has been clicked, otherwise `false`.

- **`bool nnCheckbox(const char *format, bool isChecked, int x, int y, ...)`**
  Checkbox that returns `true` when it is checked, otherwise `false`

- **`float nnHSlider(float min, float max, float initial, float step, int x, int y, int height)`**
  Horizontal slider that returns the current set value.

- **`float nnVSlider(float min, float max, float initial, float step, int x, int y, int height)`**
  Vertical slider that returns the current set value.

- **`int nnHProgressbar(float min, float max, float deltaFillState, int x, int y, int width)`**
  Horizontal progress bar that returns the current fill state in percent.
  `deltaFillState` determines how much the fillstate should increase/decrease.

- **`int nnVProgressbar(float min, float max, float deltaFillState, int x, int y, int width)`**
  Vertical progress bar that returns the current fill state in percent.
  `deltaFillState` determines how much the fillstate should increase/decrease.

- **`int nnDropdown(const char \*buttonText, const char \*\*options, int numOptions, int x, int y, int width, int height)`**
  A dropdown with a list of options to choose from. Returns the selected index.

## License

NonoGL is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
