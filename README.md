# NonoGL - A Tiny Game Library

NonoGL is a lightweight and simple 2D game library for C, designed for easy and quick setup of 2D rendering using OpenGL and FreeGLUT. It provides basic functionalities for window creation, image loading, text rendering, basic collision handling, and mouse and keyboard input handling, and a small set of ready to use themable GUI elements.

## Features

- **Window Management**: Easy window creation and management.
- **Image Loading**: Load and render images. (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC)
- **Truetype Font Loading**: Load and render Truetype fonts.
- **Collision Handling**: Basic functions to check point-rectangle, rectangle-rectangle, point-circle, rectangle-circle and circle-circle collisions.
- **Mouse and Keyboard Input Handling**: Simple functions to check for mouse and keyboard button presses.
- **Basic GUI Elements**: Comes with a small set of common and themable gui elements.

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

- **nnCreateWindow**

  ```c
  bool nnCreateWindow(char *title, int width, int height, bool virtual, bool filtered);
  ```

  Creates a window with the specified title, width and height. `virtual` determines whether to use a fixed, virtual coordinate system for rendering, regardless of the actual size of the window.
  `filtered` determines whether a smoothing filter should be applied to images and text. Otherwise, they stay pixelated (good for pixel art).

- **nnSetRenderFunc**

  ```c
  void nnSetRenderFunc(void (*callback)(void));
  ```

  Set the render callback function that NonoGL will call every frame.

- **nnDestroyWindow**

  ```c
  void nnDestroyWindow();
  ```

  Destroys the created window and cleans up resources.

- **nnSetTargetFPS**

  ```c
  void nnSetTargetFPS(int fps);
  ```

  Sets the target FPS.

- **nnSetWindowTitle**

  ```c
  void nnSetWindowTitle(const char *format, ...);
  ```

  Sets the window title with formatted text.

- **nnSetClearColor**

  ```c
  void nnSetClearColor(nnColorf color);
  ```

  Sets the background color for clearing the window.

- **nnSetColor**

  ```c
  void nnSetColor(nnColorf color);
  ```

  Sets the current drawing color.

- **nnGetColor**

  ```c
  nnColorf nnGetColor();
  ```

  Gets the current drawing color.

- **nnResetColor**

  ```c
  void nnResetColor();
  ```

  Resets the color to default (1.0, 1.0, 1.0, 1.0).

- **nnRun**

  ```c
  void nnRun();
  ```

  Starts the main rendering loop.

### Image Loading and Drawing

- **nnLoadImage**

  ```c
  nnImage nnLoadImage(const char *filepath);
  ```

  Loads an image from the specified file path.

- **nnLoadImageMem**

  ```c
  nnImage nnLoadImageMem(const unsigned char *data, int size);
  ```

  Loads an image from memory.

- **nnDrawImage**

  ```c
  void nnDrawImage(nnImage image, int x, int y);
  ```

  Draws the loaded image at the specified position.

- **nnDrawImagePortion**

  ```c
  void nnDrawImagePortion(nnImage image, int x, int y, nnRecf srcRec);
  ```

  Draws a portion of the loaded image at the specified position.

- **nnFlipImage**

  ```c
  void nnFlipImage(nnImage image, bool flipX, bool flipY);
  ```

  Flips the given image on the given axes.

- **nnRotateImage**

  ```c
  void nnRotateImage(nnImage *image, float degrees);
  ```

  Sets the rotation of the given image to the specified degrees.

- **nnScaleImage**

  ```c
  void nnScaleImage(nnImage *image, float scaleX, float scaleY);
  ```

  Sets the scale of the given image to the specified scale values.

- **nnFreeImage**

  ```c
  void nnFreeImage(nnImage image);
  ```

  Frees the given image.

### Pixmap Management and Drawing

- **nnCreatePixmap**

  ```c
  nnPixmap *nnCreatePixmap(int width, int height);
  ```

  Creates a pixmap with the given width and height.

- **nnCreatePixmapFromImage**

  ```c
  nnPixmap *nnCreatePixmapFromImage(nnImage image);
  ```

  Creates a pixmap from an image.

- **nnReadPixel**

  ```c
  nnColorf nnReadPixel(nnPixmap *pixmap, int x, int y);
  ```

  Reads a pixel from the given pixmap.

- **nnUpdatePixmap**

  ```c
  void nnUpdatePixmap(nnPixmap *pixmap);
  ```

  Updates pixels that have changed in the given pixmap. Only updates pixels that have actually changed.

- **nnDrawPixmap**

  ```c
  void nnDrawPixmap(nnPixmap *pixmap, int x, int y);
  ```

  Draws the pixmap to the screen at the specified coordinates.

- **nnFlipPixmap**

  ```c
  void nnFlipPixmap(nnPixmap *pixmap, bool flipX, bool flipY);
  ```

  Flips the given pixmap on the specified axes.

- **nnRotatePixmap**

  ```c
  void nnRotatePixmap(nnPixmap *pixmap, float angle);
  ```

  Sets the rotation of the given pixmap to the specified degrees.

- **nnScalePixmap**

  ```c
  void nnScalePixmap(nnPixmap *pixmap, float scaleX, float scaleY);
  ```

  Sets the scale of the given pixmap to the specified scale values.

- **nnCopyPixmap**

  ```c
  nnPixmap *nnCopyPixmap(nnPixmap *pixmap);
  ```

  Returns a copy of the given pixmap.

- **nnFreePixmap**

  ```c
  void nnFreePixmap(nnPixmap *pixmap);
  ```

  Frees the given pixmap.

### Primitives Drawing

- **nnPutPixel**

  ```c
  void nnPutPixel(float x, float y);
  ```

  Draws an individual pixel to the screen.

  > Note: When drawing large chunks of pixels, it is recommended to use a pixmap instead for performance reasons.

- **nnDrawPixel**

  ```c
  void nnDrawPixel(nnPixmap *pixmap, int x, int y, nnColorf color);
  ```

  Writes a pixel at the specified location with the given color to the given pixmap.

- **nnDrawLine**

  ```c
  void nnDrawLine(nnPixmap *pixmap, int x0, int y0, int x1, int y1, nnColorf color);
  ```

  Draws a line between the specified points.

- **nnDrawOval**

  ```c
  void nnDrawOval(nnPixmap *pixmap, int x, int y, int width, int height, nnColorf color, bool filled);
  ```

  Draws an oval with its center at the specified coordinates.

- **nnDrawTriangle**

  ```c
  void nnDrawTriangle(nnPixmap *pixmap, int x1, int y1, int x2, int y2, int x3, int y3, nnColorf color, bool filled);
  ```

  Draws a triangle.

- **nnDrawRect**

  ```c
  void nnDrawRect(nnPixmap *pixmap, int x, int y, int width, int height, nnColorf color, bool filled);
  ```

  Draws a rectangle where the specified coordinates are the top-left corner.

### Text Rendering

- **nnLoadFont**

  ```c
  nnFont *nnLoadFont(const char *filepath, float fontSize);
  ```

  Loads a TTF font from a file.

- **nnLoadFontMem**

  ```c
  nnFont *nnLoadFontMem(const unsigned char *data, size_t dataSize, float fontSize);
  ```

  Loads a TTF font from memory.

- **nnSetFont**

  ```c
  void nnSetFont(nnFont *font);
  ```

  Sets the font for text rendering.

- **nnGetFont**

  ```c
  nnFont *nnGetFont();
  ```

  Gets the currently set font.

- **nnDrawText**

  ```c
  void nnDrawText(const char *format, int x, int y, ...);
  ```

  Renders the given formatted text using the font set with `nnSetFont`. If no font is set, the internal default font will be used.

- **nnDrawTextZ**

  ```c
  void nnDrawTextZ(const char *format, int x, int y, float zIndex, ...);
  ```

  Renders the given formatted text with a custom z-index (-1.0 to 1.0).

- **nnDrawDebugText**

  ```c
  void nnDrawDebugText(const char *format, int x, int y, ...);
  ```

  Draws text when debug mode is enabled.

- **nnTextWidth**

  ```c
  float nnTextWidth(const char *format, ...);
  ```

  Returns the width in pixels of the given formatted text.

- **nnTextHeight**

  ```c
  float nnTextHeight();
  ```

  Returns the height in pixels of the current font.

- **nnFreeFont**

  ```c
  void nnFreeFont(nnFont *font);
  ```

  Frees the given font.

### Collision Handling

- **nnPosRecOverlaps**

  ```c
  bool nnPosRecOverlaps(int x, int y, nnRecf rect);
  ```

  Checks if a point overlaps with a rectangle.

- **nnRecsOverlap**

  ```c
  bool nnRecsOverlap(nnRecf rec1, nnRecf rec2);
  ```

  Checks if two rectangles overlap.

- **nnPosCircleOverlaps**

  ```c
  bool nnPosCircleOverlaps(int x, int y, int cx, int cy, float circleRadius);
  ```

  Checks if a point overlaps with a circle.

- **nnRecCircleOverlaps**

  ```c
  bool nnRecCircleOverlaps(nnRecf rec, int cx, int cy, float circleRadius);
  ```

  Checks if a rectangle overlaps with a circle.

- **nnCirclesOverlap**

  ```c
  bool nnCirclesOverlap(int cx1, int cy1, float circle1Radius, int cx2, int cy2, float circle2Radius);
  ```

  Checks if two circles overlap.

### Input Handling

#### Keyboard Input

- **nnKeyHit**

  ```c
  bool nnKeyHit(int key);
  ```

  Returns `true` if the specified key was pressed since the last frame.

- **nnKeyDown**

  ```c
  bool nnKeyDown(int key);
  ```

  Returns `true` if the specified key is currently being held down.

- **nnKeyReleased**

  ```c
  bool nnKeyReleased(int key);
  ```

  Returns `true` if the specified key was released since the last frame.

- **nnFlushKeys**

  ```c
  void nnFlushKeys();
  ```

  Resets all key states.

#### Mouse Input

- **nnMouseHit**

  ```c
  bool nnMouseHit(int button);
  ```

  Returns `true` if the specified mouse button was clicked since the last frame.

- **nnMouseDown**

  ```c
  bool nnMouseDown(int button);
  ```

  Returns `true` if the specified mouse button is currently being held down.

- **nnMouseReleased**

  ```c
  bool nnMouseReleased(int button);
  ```

  Returns `true` if the specified mouse button was released since the last frame.

- **nnMouseWheelDelta**

  ```c
  int nnMouseWheelDelta();
  ```

  Returns the direction of the mouse wheel movement.

- **nnGetMousePosition**

  ```c
  nnPos nnGetMousePosition();
  ```

  Returns the current position of the mouse cursor.

- **nnMouseMotionDelta**

  ```c
  nnPos nnMouseMotionDelta();
  ```

  Returns the mouse motion delta (change in position) since the last frame.

- **nnFlushMouse**

  ```c
  void nnFlushMouse();
  ```

  Resets all mouse button states.

### Utility

- **nnLoadFileBytes**

  ```c
  unsigned char *nnLoadFileBytes(const char *filepath, int *size);
  ```

  Loads a file into a buffer and returns its pointer.

- **nnFreeFileBytes**

  ```c
  void nnFreeFileBytes(unsigned char *buffer);
  ```

  Frees the memory allocated by `nnLoadFileBytes`.

- **nnLerp**

  ```c
  float nnLerp(float min, float max, float speed, float ease);
  ```

  Lerps between `min` and `max` by the given speed. Use `ease` for smooth transitions.

- **nnSetDebugMode**

  ```c
  void nnSetDebugMode(bool flag);
  ```

  Enables or disables debug mode.

- **nnIsDebugMode**

  ```c
  bool nnIsDebugMode();
  ```

  Returns whether debug mode is enabled.

- **nnFPS**

  ```c
  int nnFPS;
  ```

  Holds the current frames per second.

- **nnDT**

  ```c
  float nnDT;
  ```

  Holds the delta time (time elapsed since the last frame).

- **nnMS**

  ```c
  float nnMS;
  ```

  Time in milliseconds since the application started.

### GUI

- **nnPanel**

  ```c
  bool nnPanel(int x, int y, int width, int height);
  ```

  A panel element with background color and border. Returns `true` if the mouse is hovering over it, otherwise `false`.

- **nnLabel**

  ```c
  void nnLabel(const char *format, int x, int y, int width, int height, bool border, ...);
  ```

  Displays text with an optional border. Wraps text within the defined width and clips it if it exceeds the provided height.

- **nnButton**

  ```c
  bool nnButton(const char *format, int x, int y, int width, int height, ...);
  ```

  Creates a button that returns `true` when clicked.

- **nnCheckbox**

  ```c
  bool nnCheckbox(const char *format, bool isChecked, int x, int y, ...);
  ```

  Creates a checkbox that returns `true` when checked.

- **nnHSlider**

  ```c
  float nnHSlider(float min, float max, float initial, float step, int x, int y, int height);
  ```

  Creates a horizontal slider that returns the current value.

- **nnVSlider**

  ```c
  float nnVSlider(float min, float max, float initial, float step, int x, int y, int height);
  ```

  Creates a vertical slider that returns the current value.

- **nnHProgressbar**

  ```c
  int nnHProgressbar(float min, float max, float deltaFillState, int x, int y, int width);
  ```

  Creates a horizontal progress bar that returns the current fill state in percent.

- **nnVProgressbar**

  ```c
  int nnVProgressbar(float min, float max, float deltaFillState, int x, int y, int width);
  ```

  Creates a vertical progress bar that returns the current fill state in percent.

- **nnDropdown**

  ```c
  int nnDropdown(const char *buttonText, const char **options, int numOptions, int x, int y, int width, int height);
  ```

  Creates a dropdown that returns the selected index.

- **nnScrollableList**

  ```c
  int nnScrollableList(const char **items, int numItems, int x, int y, int width, int height);
  ```

  Creates a scrollable list that returns the selected index.

## License

NonoGL is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
