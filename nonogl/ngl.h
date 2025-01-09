#define FREEGLUT_STATIC

#ifndef NONO_GL_H
#define NONO_GL_H

#include <stdbool.h>
#include "internal/include/GL/freeglut.h"
#include "internal/include/stb_image.h"
#include "internal/include/stb_truetype.h"

/******************************************************************************************************************************/

/*  Public API */

/******************************************************************************************************************************/

// Represents a 2D position.
typedef struct
{
    int x; // X-coordinate
    int y; // Y-coordinate
} nnPos;

// Represents a 2D rectangle.
typedef struct
{
    float x;      // X-coordinate of the rectangle's top-left corner
    float y;      // Y-coordinate of the rectangle's top-left corner
    float width;  // Width of the rectangle
    float height; // Height of the rectangle
} nnRecf;

// Represents an RGBA color.
typedef struct
{
    float r; // Red component (0.0 to 1.0)
    float g; // Green component (0.0 to 1.0)
    float b; // Blue component (0.0 to 1.0)
    float a; // Alpha (transparency) component (0.0 to 1.0)
} nnColorf;

// Represents an image loaded into OpenGL.
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

// Represents a 2D buffer of colors (`nnColorf`), for fast pixel manipulations.
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

// Represents a font loaded with stb_truetype.
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

// Theme used by gui elements
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

/*
 * Window Management
 */

// Creates a window with the specified title, width and height. Scalable: determines wether the drawn content on the screen should be resized when the window resizes.
// Filterd: determines wether a smoothing filter should be applied to images and text. Otherwise they stay pixelated (good for pixel art).
bool nnCreateWindow(char *title, int width, int height, bool scalable, bool filtered);

// Set the display/render callback function that NonoGL will call every frame.
void nnSetRenderFunc(void (*callback)(void));

// Release resources and free memory.
void nnDestroyWindow();

// Sets the target FPS.
void nnSetTargetFPS(int fps);

// Set the window title.
void nnSetWindowTitle(const char *format, ...);

// Set the clear color.
void nnSetClearColor(nnColorf color);

// Set the draw color.
void nnSetColor(nnColorf color);

// Get the current draw color.
nnColorf nnGetColor();

// Resets the color to default (1.0, 1.0, 1.0).
void nnResetColor();

// Starts the main rendering loop.
void nnRun();

/*
 * Image Loading and Drawing
 */

// Loads an image from the specified file path.
nnImage nnLoadImage(const char *filepath);

// Load an image from memory.
nnImage nnLoadImageMem(const unsigned char *data, int size);

// Draw an image on the screen.
void nnDrawImage(nnImage image, int x, int y);

// Draw a portion of an image.
void nnDrawImagePortion(nnImage image, int x, int y, nnRecf srcRec);

// Flips the given image on the given axies.
void nnFlipImage(nnImage *image, bool flipX, bool flipY);

// Sets the rotateation of the given image to the given degrees.
void nnRotateImage(nnImage *image, float degrees);

// Sets the scale of the given image to the given scale values.
void nnScaleImage(nnImage *image, float scaleX, float scaleY);

// Frees the given image.
void nnFreeImage(nnImage image);

/*
 * Pixmap Management and Drawing
 */

// Create a pixel buffer with the given width and height.
nnPixmap *nnCreatePixmap(int width, int height);

// Creates a Pixmap from an image.
nnPixmap *nnCreatePixmapFromImage(nnImage image);

// Read a pixel from the given Pixmap.
nnColorf nnReadPixel(nnPixmap *pixmap, int x, int y);

// Update pixels that have changed in the Pixmap. Only updates pixels that have actually changed.
void nnUpdatePixmap(nnPixmap *pixmap);

// Draw the Pixmap to the screen.
void nnDrawPixmap(nnPixmap *pixmap, int x, int y);

// Flips the given pixmap on the given axies.
void nnFlipPixmap(nnPixmap *pixmap, bool flipX, bool flipY);

// Rotates the given pixmap by the amount of given degrees.
void nnRotatePixmap(nnPixmap *pixmap, float angle);

// Scales the given pixmap by the given amount.
void nnScalePixmap(nnPixmap *pixmap, float scaleX, float scaleY);

// Returns a copy of the given pixmap.
nnPixmap *nnCopyPixmap(nnPixmap *pixmap);

// Free the given Pixmap
void nnFreePixmap(nnPixmap *pixmap);

//// Primitives Drawing

// Draw an individual pixel to the screen (When drawing large chunks of pixels, it is recommended to use a Pixmap instead for performance reasons)
void nnPutPixel(float x, float y);

// Writes a pixel at x, y location with the given color to the given Pixmap.
void nnDrawPixel(nnPixmap *pixmap, int x, int y, nnColorf color);

// Draws a line from x0, y0 to x1, y1
void nnDrawLine(nnPixmap *pixmap, int x0, int y0, int x1, int y1, nnColorf color);

// Draws an oval with its center at x, y.
void nnDrawOval(nnPixmap *pixmap, int x, int y, int width, int height, nnColorf color, bool filled);

// Draws a triangle.
void nnDrawTriangle(nnPixmap *pixmap, int x1, int y1, int x2, int y2, int x3, int y3, nnColorf color, bool filled);

// Draws a rectangle where x and y is the top left corner.
void nnDrawRect(nnPixmap *pixmap, int x, int y, int width, int height, nnColorf color, bool filled);

/*
 * Text Rendering
 */

// Load a font from a .ttf file
nnFont *nnLoadFont(const char *filepath, float fontSize);

// Load a font from a .ttf file
nnFont *nnLoadFontMem(const unsigned char *data, size_t dataSize, float fontSize);

// Set font for text rendering.
void nnSetFont(nnFont *font);

// Get the font that is currently set.
nnFont *nnGetFont();

// Render the given formatted text using the font set with `nnSetFont`. If no font has been set, the default non-scalable font will be used.
void nnDrawText(const char *format, int x, int y, ...);

// Returns the width in pixels of the given string regarding the current font.
float nnTextWidth(const char *format, ...);

// Returns the height in pixels of the the current font.
float nnTextHeight();

// Free the given font.
void nnFreeFont(nnFont *font);

/*
 * Collision Handling
 */

// Checks if a point overlaps with a rectangle.
bool nnPosRecOverlaps(int x, int y, nnRecf rect);

// Checks if two rectangles overlap.
bool nnRecsOverlap(nnRecf rec1, nnRecf rec2);

// Check if a point overlaps with a circle.
bool nnPosCircleOverlaps(int x, int y, int cx, int cy, float circleradius);

// Check if a rectangle overlaps with a circle.
bool nnRecCircleOverlaps(nnRecf rec, int cx, int cy, float circleradius);

// Check if two cirles overlap.
bool nnCirclesOverlaps(int cx1, int cy1, float circle1radius, int cx2, int cy2, float circle2radius);

/*
 * Input Handling
 */

//// Keyboard Input

// Returns `true` if the specified key was pressed since the last frame.
bool nnKeyHit(int key);

// Returns `true` if the specified key is currently being held down.
bool nnKeyDown(int key);

// Returns `true` if the specified key was released since the last frame.
bool nnKeyReleased(int key);

//// Mouse Input

// Returns `true` if the specified mouse button was clicked since the last frame.
bool nnMouseHit(int button);

// Returns `true` if the specified mouse button is currently being held down.
bool nnMouseDown(int button);

// Returns `true` if the specified mouse button was released since the last frame.
bool nnMouseReleased(int button);

// Returns the direction of the mouse wheel movement: `positive number` means up, `negative number` means down, and `0` if no movement occurred.
int nnMouseWheelDelta();

// Returns the current position of the mouse cursor as an `nnPos` struct.
nnPos nnGetMousePosition();

// Returns the mouse motion delta (change in position) since the last frame.
nnPos nnMouseMotionDelta();

// Load a file as bytes into a buffer and return the pointer to that buffer.
unsigned char *nnLoadFileBytes(const char *filepath, int *size);

// Free the buffer allocated by nnLoadFileBytes.
void nnFreeFileBytes(unsigned char *buffer);

// Lerps between min and max by the given speed. Use ease to define a smooth transition when changing direction.
float nnLerp(float min, float max, float speed, float ease);

// Holds the current frames per second.
int nnFPS;

// Holds the delta time (time elapsed since the last frame).
float nnDT;

// Time in milliseconds since the application started
unsigned int nnMS;

/******************************************************************************************************************************/
/*  End of Public API */
/******************************************************************************************************************************/

#endif // NONO_GL_H

/******************************************************************************************************************************/

/*  IMPLEMENTATION */

/******************************************************************************************************************************/
#ifdef NONOGL_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include "internal/include/stb_image.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "internal/include/stb_truetype.h"

#include "internal/include/default_font.h"

#define _NN_MAX_KEYS 256
#define _NN_MAX_MOUSE_BUTTONS 3

typedef struct
{
    char *title;
    int initialWidth;
    int initialHeight;
    int width;
    int height;
    bool scalable;
    bool filtered;
    float deltaTime;
    double lastTime;
    int currentFPS;
    int targetFPS;
    int window;
    nnFont *font;
    nnColorf currentDrawColor;

    // Keyboard state
    bool keys[_NN_MAX_KEYS];
    bool keysPressed[_NN_MAX_KEYS];
    bool keysReleased[_NN_MAX_KEYS];

    // Mouse state
    bool mouseButtons[_NN_MAX_MOUSE_BUTTONS];
    bool mouseButtonsPressed[_NN_MAX_MOUSE_BUTTONS];
    bool mouseButtonsReleased[_NN_MAX_MOUSE_BUTTONS];
    int mouseWheelDelta;
    nnPos mousePosition;
    nnPos mouseMotionDelta;

    // Function pointer for the display callback
    void (*displayCallback)(void);

} _nnState;

static nnTheme _nnCurrentTheme = {
    .primaryColor = {0.2f, 0.5f, 0.8f, 1.0f},         // Blue
    .primaryColorAccent = {0.3f, 0.6f, 0.9f, 1.0f},   // Light Blue
    .secondaryColor = {0.1f, 0.1f, 0.1f, 1.0f},       // Dark Gray
    .secondaryColorAccent = {0.2f, 0.2f, 0.2f, 1.0f}, // Gray
    .borderColor = {0.5f, 0.5f, 0.5f, 1.0f},          // Medium Gray
    .borderColorAccent = {0.7f, 0.7f, 0.7f, 1.0f},    // Light Gray
    .textPrimaryColor = {1.0f, 1.0f, 1.0f, 1.0f},     // White
    .textSecondaryColor = {0.8f, 0.8f, 0.8f, 1.0f},   // Light Gray
};

_nnState _nnstate;

// Define custom exit handlers
void _customExitFunction(int status)
{
    exit(status);
}

// Wrapper display function that calls the function pointer
void _nnDisplayCallbackWrapper()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (_nnstate.displayCallback != NULL)
    {
        _nnstate.displayCallback();
    }

    glutSwapBuffers();
    glutMainLoopEvent();
}

void _customExitFunctionWithMessage(const char *msg, int status)
{
    if (msg)
    {
        fprintf(stderr, "%s\n", msg);
    }
    exit(status);
}

void _nnSpecialKeyCallback(int key, int x, int y)
{
    // Handle special keys here
}

void _nnSetupOrthoProjection(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void _nnSetScale(float scaleX, float scaleY)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(scaleX, scaleY, 1.0f);
}

void _nnUpdateScale(int width, int height, int shouldUpdate)
{
    if (shouldUpdate != 0)
    {
        float scaleX = (float)width / _nnstate.initialWidth;
        float scaleY = (float)height / _nnstate.initialHeight;
        _nnSetScale(scaleX, scaleY);
    }
}

void _nnFramebufferSizeCallback(int width, int height)
{
    glViewport(0, 0, width, height);
    _nnSetupOrthoProjection(width, height);
    _nnUpdateScale(width, height, _nnstate.scalable);
}

void _nnTimerCallback(int value)
{
    // Parameters for FPS averaging
#define _NN_FPS_BUFFER_SIZE 20                          // Number of frames to average over
    static double fpsBuffer[_NN_FPS_BUFFER_SIZE] = {0}; // Circular buffer for FPS values
    static int fpsIndex = 0;                            // Current index in the buffer
    static double fpsSum = 0;                           // Sum of all FPS values in the buffer
    static int fpsCount = 0;                            // Number of frames added to the buffer

    // Calculate deltaTime and instantaneous FPS
    double elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    double currentTime = elapsedTime / 1000.0; // Get time in seconds
    _nnstate.deltaTime = currentTime - _nnstate.lastTime;
    double instantaneousFPS = (_nnstate.deltaTime > 0) ? (1.0 / _nnstate.deltaTime) : 0;
    _nnstate.lastTime = currentTime;

    // Update the FPS buffer
    fpsSum -= fpsBuffer[fpsIndex];          // Remove the old value from the sum
    fpsBuffer[fpsIndex] = instantaneousFPS; // Add the new value to the buffer
    fpsSum += instantaneousFPS;             // Add the new value to the sum

    fpsIndex = (fpsIndex + 1) % _NN_FPS_BUFFER_SIZE; // Move to the next index
    if (fpsCount < _NN_FPS_BUFFER_SIZE)
    {
        fpsCount++; // Increase the count until the buffer is full
    }

    // Calculate the average FPS
    _nnstate.currentFPS = fpsSum / fpsCount;

    // Update global variables
    nnFPS = _nnstate.currentFPS;
    nnDT = _nnstate.deltaTime;
    nnMS = (unsigned int)elapsedTime;

    // Trigger display refresh
    glutPostRedisplay();

    // Re-register the timer callback to get targetFPS
    glutTimerFunc(1000 / _nnstate.targetFPS, _nnTimerCallback, 0);
}

void _nnKeyDownCallback(unsigned char key, int x, int y)
{
    if (!_nnstate.keys[key])
    {
        _nnstate.keysPressed[key] = true;
    }
    _nnstate.keys[key] = true;
}

void _nnKeyUpCallback(unsigned char key, int x, int y)
{
    _nnstate.keys[key] = false;
    _nnstate.keysReleased[key] = true;
}

void _nnMouseButtonCallback(int button, int state, int x, int y)
{
    if (button < _NN_MAX_MOUSE_BUTTONS)
    {
        if (state == GLUT_DOWN)
        {
            if (!_nnstate.mouseButtons[button])
            {
                _nnstate.mouseButtonsPressed[button] = true;
            }
            _nnstate.mouseButtons[button] = true;
        }
        else if (state == GLUT_UP)
        {
            _nnstate.mouseButtons[button] = false;
            _nnstate.mouseButtonsReleased[button] = true;
        }
    }
}

void _nnMouseWheelFunc(int wheel, int direction, int x, int y)
{
    _nnstate.mouseWheelDelta = direction; // +1 for up, -1 for down
}

void _nnMouseMotionFunc(int x, int y)
{
    // Motion delta
    nnPos newMousePosition = {(float)x, (float)y};
    _nnstate.mouseMotionDelta.x = newMousePosition.x - _nnstate.mousePosition.x;
    _nnstate.mouseMotionDelta.y = newMousePosition.y - _nnstate.mousePosition.y;
    _nnstate.mousePosition = newMousePosition;
}

void _nnDrawTextFallback(const char *format, int x, int y, ...)
{
    if (!format)
        return;

    char buffer[1024];
    va_list args;
    va_start(args, y);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    glRasterPos2i(x, y);

    for (char *c = buffer; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
}

static nnFont *_nnLoadFont(const unsigned char *fontBuffer, size_t bufferSize, float fontSize)
{
    nnFont *font = malloc(sizeof(nnFont));
    if (!font)
    {
        printf("Failed to allocate memory for font.\n");
        return NULL;
    }

    // Allocate and copy the font data into font buffer
    font->fontBuffer = malloc(bufferSize);
    if (!font->fontBuffer)
    {
        printf("Failed to allocate memory for font buffer.\n");
        free(font);
        return NULL;
    }
    memcpy(font->fontBuffer, fontBuffer, bufferSize);

    // Initialize font info
    if (!stbtt_InitFont(&font->fontInfo, font->fontBuffer, 0))
    {
        printf("Failed to initialize font.\n");
        free(font->fontBuffer);
        free(font);
        return NULL;
    }

    // Calculate font scale
    font->scale = stbtt_ScaleForPixelHeight(&font->fontInfo, fontSize);

    // Calculate height from actual glyphs
    int minY, maxY;
    stbtt_GetCodepointBox(&font->fontInfo, 'A', NULL, &minY, NULL, &maxY); // Example for 'A' glyph
    font->glyphHeight = font->scale * (maxY - minY);

    // Create the font atlas
    font->atlasWidth = 512;
    font->atlasHeight = 512;

    unsigned char *bitmap = calloc(1, font->atlasWidth * font->atlasHeight);
    if (!bitmap)
    {
        printf("Failed to allocate memory for font atlas.\n");
        free(font->fontBuffer);
        free(font);
        return NULL;
    }

    // Bake the font
    stbtt_BakeFontBitmap(font->fontBuffer, 0, fontSize, bitmap, font->atlasWidth, font->atlasHeight, 32, 96, font->charData);

    // Upload the atlas to OpenGL
    glGenTextures(1, &font->textureID);
    glBindTexture(GL_TEXTURE_2D, font->textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, font->atlasWidth, font->atlasHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap);

    if (!_nnstate.filtered)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    free(bitmap);

    return font;
}

/*******************************************************************************************************/
// Public api function implementations
/*******************************************************************************************************/

/*
 * Window Management
 */
bool nnCreateWindow(char *title, int width, int height, bool scalable, bool filtered)
{
    _nnstate.displayCallback = NULL;

    _nnstate.scalable = scalable;
    _nnstate.filtered = filtered;

    _nnstate.title = title;
    _nnstate.initialWidth = width;
    _nnstate.initialHeight = height;
    _nnstate.width = width;
    _nnstate.height = height;

    _nnstate.deltaTime = 0.0f;
    _nnstate.lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    _nnstate.currentFPS = 0;

    // Set default FPS if not already set
    if (_nnstate.targetFPS <= 0)
    {
        _nnstate.targetFPS = 60;
    }

    nnFPS = 0;
    nnDT = 0.0f;
    nnMS = 0.0f;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    int argc = 1;
    char *argv[1] = {(char *)"Something"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    _nnstate.window = glutCreateWindow(title);

    // Load a default font
    _nnstate.font = _nnLoadFont(_nnRoboto_Regular_ttf_arr, _nnRoboto_Regular_ttf_arr_len, 24);

    /* Set the callbacks */
    glutKeyboardFunc(_nnKeyDownCallback);
    glutKeyboardUpFunc(_nnKeyUpCallback);
    glutMouseFunc(_nnMouseButtonCallback);
    glutMouseWheelFunc(_nnMouseWheelFunc);
    glutMotionFunc(_nnMouseMotionFunc);
    glutPassiveMotionFunc(_nnMouseMotionFunc);
    glutSpecialFunc(_nnSpecialKeyCallback);
    glutReshapeFunc(_nnFramebufferSizeCallback);

    /* Set up the orthographic projection */
    _nnSetupOrthoProjection(_nnstate.width, _nnstate.height);
    _nnUpdateScale(_nnstate.width, _nnstate.height, _nnstate.scalable);

    /* Set initial scale */
    _nnSetScale(1.0f, 1.0f);

    /* Start the timer for limiting FPS */
    glutTimerFunc(1000 / _nnstate.targetFPS, _nnTimerCallback, 0);

    nnResetColor();

    return true;
}

void nnSetRenderFunc(void (*callback)(void))
{
    _nnstate.displayCallback = callback;
    glutDisplayFunc(_nnDisplayCallbackWrapper);
}

void nnDestroyWindow()
{
    glutDestroyWindow(_nnstate.window);
}

void nnSetTargetFPS(int fps)
{
    if (fps <= 0)
    {
        printf("Invalid FPS value. FPS must be greater than 0.\n");
        return;
    }
    _nnstate.targetFPS = fps;
}

void nnSetWindowTitle(const char *format, ...)
{
    char buffer[256];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    glutSetWindowTitle(buffer);
}

void nnSetClearColor(nnColorf color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void nnSetColor(nnColorf color)
{
    _nnstate.currentDrawColor = color;
    glColor4f(color.r, color.g, color.b, color.a);
}

nnColorf nnGetColor()
{
    return _nnstate.currentDrawColor;
}

void nnResetColor()
{
    _nnstate.currentDrawColor = (nnColorf){1.0f, 1.0f, 1.0f, 1.0f};
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void nnRun()
{
    if (_nnstate.displayCallback == NULL)
    {
        printf("DisplayCallback not set, shutting down...");
        exit(-1);
    }
    glutMainLoop();
}

/*
 * Image Loading and Drawing
 */

nnImage nnLoadImage(const char *filepath)
{
    nnImage image;
    image.isFlippedX = false;
    image.isFlippedY = false;
    image.scaleX = 1.0f;
    image.scaleY = 1.0f;
    image.angle = 0.0f;

    // stbi_set_flip_vertically_on_load(1);
    unsigned char *imageData = stbi_load(filepath, &image.width, &image.height, 0, 4);
    if (!imageData)
    {
        printf("Failed to load image:\n%s\n", filepath);
        image.textureID = 0;
        return image;
    }

    glGenTextures(1, &image.textureID);
    glBindTexture(GL_TEXTURE_2D, image.textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    if (!_nnstate.filtered)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(imageData);

    return image;
}

nnImage nnLoadImageMem(const unsigned char *data, int size)
{
    nnImage image;
    image.isFlippedX = false;
    image.isFlippedX = false;
    image.scaleX = 1.0f;
    image.scaleY = 1.0f;
    image.angle = 0.0f;

    unsigned char *imageData = stbi_load_from_memory(data, size, &image.width, &image.height, 0, 4);
    if (!imageData)
    {
        printf("Failed to load image from memory\n");
        image.textureID = 0;
        return image;
    }

    glGenTextures(1, &image.textureID);
    glBindTexture(GL_TEXTURE_2D, image.textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    if (_nnstate.filtered)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(imageData);

    return image;
}

void nnDrawImage(nnImage image, int x, int y)
{
    if (image.textureID == 0)
    {
        printf("Failed to draw image\n");
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, image.textureID);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();

    // Translate to position
    glTranslatef(x + image.width / 2.0f, y + image.height / 2.0f, 0);

    // Apply rotation if any
    if (image.angle != 0.0f)
    {
        glRotatef(image.angle, 0, 0, 1);
    }

    // Apply flipping
    float flipX = image.isFlippedX ? -1.0f : 1.0f;
    float flipY = image.isFlippedY ? -1.0f : 1.0f;

    // Apply scaling
    glScalef(image.scaleX * flipX, image.scaleY * flipY, 1.0f);

    // Render the quad centered at the origin
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-image.width / 2.0f, -image.height / 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(image.width / 2.0f, -image.height / 2.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(image.width / 2.0f, image.height / 2.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-image.width / 2.0f, image.height / 2.0f);
    glEnd();

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void nnDrawImagePortion(nnImage image, int x, int y, nnRecf srcRec)
{
    if (image.textureID == 0)
    {
        printf("Failed to draw image portion\n");
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, image.textureID);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();

    // Translate to position
    glTranslatef(x + srcRec.width / 2.0f, y + srcRec.height / 2.0f, 0);

    // Apply rotation if any
    if (image.angle != 0.0f)
    {
        glRotatef(image.angle, 0, 0, 1);
    }

    // Scale for flipping
    float scaleX = image.isFlippedX ? -1.0f : 1.0f;
    float scaleY = image.isFlippedY ? -1.0f : 1.0f;
    glScalef(scaleX, scaleY, 1.0f);

    // Adjust texture coordinates for the specified portion
    float texLeft = srcRec.x / image.width;
    float texRight = (srcRec.x + srcRec.width) / image.width;
    float texTop = srcRec.y / image.height;
    float texBottom = (srcRec.y + srcRec.height) / image.height;

    // Render the portion centered at the origin
    glBegin(GL_QUADS);
    glTexCoord2f(texLeft, texTop);
    glVertex2f(-srcRec.width / 2.0f, -srcRec.height / 2.0f);
    glTexCoord2f(texRight, texTop);
    glVertex2f(srcRec.width / 2.0f, -srcRec.height / 2.0f);
    glTexCoord2f(texRight, texBottom);
    glVertex2f(srcRec.width / 2.0f, srcRec.height / 2.0f);
    glTexCoord2f(texLeft, texBottom);
    glVertex2f(-srcRec.width / 2.0f, srcRec.height / 2.0f);
    glEnd();

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void nnFlipImage(nnImage *image, bool flipX, bool flipY)
{
    if (image->textureID == 0)
    {
        printf("Failed to flip image\n");
        return;
    }
    if (flipX)
        image->isFlippedX = !image->isFlippedX;

    if (flipY)
        image->isFlippedX = !image->isFlippedY;
}

void nnRotateImage(nnImage *image, float angle)
{
    if (image->textureID == 0)
    {
        printf("Failed to rotate image\n");
        return;
    }

    image->angle = fmod(angle, 360.0f);
    if (image->angle < 0.0f)
    {
        image->angle += 360.0f;
    }
}

void nnScaleImage(nnImage *image, float scaleX, float scaleY)
{
    if (image->textureID == 0)
    {
        printf("Failed to scale image\n");
        return;
    }

    image->scaleX = scaleX;
    image->scaleY = scaleY;
}

void nnFreeImage(nnImage image)
{
    if (!image.textureID == 0)
        return;

    glDeleteTextures(1, &image.textureID);
}

/*
 * Pixmap Management and Drawing
 */

nnPixmap *nnCreatePixmap(int width, int height)
{
    nnPixmap *pixmap = malloc(sizeof(nnPixmap));
    pixmap->isFlippedX = false;
    pixmap->isFlippedY = false;
    pixmap->scaleX = 1.0f;
    pixmap->scaleY = 1.0f;
    pixmap->angle = 0.0f;

    if (!pixmap)
    {
        printf("Failed to create pixmap");
        return NULL;
    }

    pixmap->width = width;
    pixmap->height = height;

    // Allocate pixel memory
    pixmap->pixels = calloc(width * height, sizeof(nnColorf));
    if (!pixmap->pixels)
    {
        printf("Failed to create pixmap pixel buffer");
        free(pixmap);
        return NULL;
    }

    // Create OpenGL texture
    glGenTextures(1, &pixmap->textureID);
    glBindTexture(GL_TEXTURE_2D, pixmap->textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, pixmap->pixels);

    // Set texture parameters
    if (!_nnstate.filtered)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return pixmap;
}

nnPixmap *nnCreatePixmapFromImage(nnImage image)
{
    if (image.textureID == 0 || image.width <= 0 || image.height <= 0)
    {
        printf("Failed to create pixmap from image");
        return NULL; // Invalid image
    }

    // Allocate a new pixmap
    nnPixmap *pixmap = nnCreatePixmap(image.width, image.height);
    if (!pixmap)
    {
        return NULL; // Failed to allocate pixmap
    }

    // Allocate a buffer to read the image data from the GPU
    unsigned char *pixelData = malloc(image.width * image.height * 4); // 4 bytes per pixel (RGBA)
    if (!pixelData)
    {
        nnFreePixmap(pixmap);
        return NULL; // Memory allocation failed
    }

    // Read the image data from the GPU
    glBindTexture(GL_TEXTURE_2D, image.textureID);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Copy the pixel data into the pixmap
    for (int y = 0; y < image.height; y++)
    {
        for (int x = 0; x < image.width; x++)
        {
            int index = (y * image.width + x) * 4; // Calculate the pixel index
            nnColorf color = {
                pixelData[index] / 255.0f,
                pixelData[index + 1] / 255.0f,
                pixelData[index + 2] / 255.0f,
                pixelData[index + 3] / 255.0f};
            nnDrawPixel(pixmap, x, y, color);
        }
    }

    // Upload the pixel data to the pixmap's GPU texture
    nnUpdatePixmap(pixmap);

    // Free the temporary pixel data buffer
    free(pixelData);

    return pixmap;
}

void nnUpdatePixmap(nnPixmap *buffer)
{
    if (!buffer || !buffer->pixels)
        return;

    glBindTexture(GL_TEXTURE_2D, buffer->textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, buffer->width, buffer->height, GL_RGBA, GL_FLOAT, buffer->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void nnDrawPixmap(nnPixmap *pixmap, int x, int y)
{
    if (!pixmap || !pixmap->pixels)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pixmap->textureID);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();

    // Translate to position
    glTranslatef(x + pixmap->width / 2.0f, y + pixmap->height / 2.0f, 0);

    // Apply rotation if any
    if (pixmap->angle != 0.0f)
    {
        glRotatef(pixmap->angle, 0, 0, 1);
    }

    // Apply flipping
    float flipX = pixmap->isFlippedX ? -1.0f : 1.0f;
    float flipY = pixmap->isFlippedY ? -1.0f : 1.0f;

    // Apply scaling
    glScalef(pixmap->scaleX * flipX, pixmap->scaleY * flipY, 1.0f);

    // Render the quad centered at the origin
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-pixmap->width / 2.0f, -pixmap->height / 2.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(pixmap->width / 2.0f, -pixmap->height / 2.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(pixmap->width / 2.0f, pixmap->height / 2.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-pixmap->width / 2.0f, pixmap->height / 2.0f);
    glEnd();

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void nnFlipPixmap(nnPixmap *pixmap, bool flipX, bool flipY)
{
    if (pixmap->textureID == 0)
    {
        printf("Failed to flip pixmap\n");
        return;
    }
    if (flipX)
        pixmap->isFlippedX = !pixmap->isFlippedX;

    if (flipY)
        pixmap->isFlippedY = !pixmap->isFlippedY;
}

void nnRotatePixmap(nnPixmap *pixmap, float angle)
{
    if (pixmap->textureID == 0)
    {
        printf("Failed to rotate pixmap\n");
        return;
    }

    pixmap->angle = fmod(angle, 360.0f);
    if (pixmap->angle < 0.0f)
    {
        pixmap->angle += 360.0f;
    }
}

void nnScalePixmap(nnPixmap *pixmap, float scaleX, float scaleY)
{
    if (pixmap->textureID == 0)
    {
        printf("Failed to scale pixmap\n");
        return;
    }

    pixmap->scaleX = scaleX;
    pixmap->scaleY = scaleY;
}

nnPixmap *nnCopyPixmap(nnPixmap *pixmap)
{
    if (!pixmap || !pixmap->pixels)
        return NULL;

    // Allocate a new pixmap
    nnPixmap *newPixmap = nnCreatePixmap(pixmap->width, pixmap->height);
    if (!newPixmap)
        return NULL;

    // Copy pixel data from the original pixmap
    memcpy(newPixmap->pixels, pixmap->pixels, pixmap->width * pixmap->height * sizeof(nnColorf));

    // Update the texture on the GPU
    nnUpdatePixmap(newPixmap);

    return newPixmap;
}

void nnFreePixmap(nnPixmap *buffer)
{
    if (!buffer)
        return;

    if (buffer->pixels)
        free(buffer->pixels);
    glDeleteTextures(1, &buffer->textureID);
    free(buffer);
}

//// Primitives

void nnPutPixel(float x, float y)
{
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void nnDrawPixel(nnPixmap *pixmap, int x, int y, nnColorf color)
{
    if (!pixmap || x < 0 || y < 0 || x >= pixmap->width || y >= pixmap->height)
        return;

    y = pixmap->height - 1 - y; // Adjust y-coordinate for top-left origin
    pixmap->pixels[y * pixmap->width + x] = color;
}

void nnDrawLine(nnPixmap *pixmap, int x0, int y0, int x1, int y1, nnColorf color)
{
    if (!pixmap)
        return;

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true)
    {
        nnDrawPixel(pixmap, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void nnDrawOval(nnPixmap *pixmap, int x, int y, int width, int height, nnColorf color, bool filled)
{
    if (!pixmap)
        return;

    int a = width / 2;
    int b = height / 2;
    int x0 = x - a, y0 = y - b;

    if (filled)
    {
        for (int i = 0; i <= height; i++)
        {
            for (int j = 0; j <= width; j++)
            {
                int px = x0 + j;
                int py = y0 + i;
                if (((j - a) * (j - a)) * b * b + ((i - b) * (i - b)) * a * a <= a * a * b * b)
                {
                    nnDrawPixel(pixmap, px, py, color);
                }
            }
        }
    }
    else
    {
        int x1 = 0, y1 = b;
        int a2 = a * a, b2 = b * b;
        int fa2 = 4 * a2, fb2 = 4 * b2;
        int sigma = 2 * b2 + a2 * (1 - 2 * b);

        while (b2 * x1 <= a2 * y1)
        {
            nnDrawPixel(pixmap, x + x1, y + y1, color);
            nnDrawPixel(pixmap, x - x1, y + y1, color);
            nnDrawPixel(pixmap, x + x1, y - y1, color);
            nnDrawPixel(pixmap, x - x1, y - y1, color);

            if (sigma >= 0)
            {
                sigma += fa2 * (1 - y1);
                y1--;
            }
            sigma += b2 * ((4 * x1) + 6);
            x1++;
        }

        x1 = a;
        y1 = 0;
        sigma = 2 * a2 + b2 * (1 - 2 * a);

        while (a2 * y1 <= b2 * x1)
        {
            nnDrawPixel(pixmap, x + x1, y + y1, color);
            nnDrawPixel(pixmap, x - x1, y + y1, color);
            nnDrawPixel(pixmap, x + x1, y - y1, color);
            nnDrawPixel(pixmap, x - x1, y - y1, color);

            if (sigma >= 0)
            {
                sigma += fb2 * (1 - x1);
                x1--;
            }
            sigma += a2 * ((4 * y1) + 6);
            y1++;
        }
    }
}

void nnDrawTriangle(nnPixmap *pixmap, int x1, int y1, int x2, int y2, int x3, int y3, nnColorf color, bool filled)
{
    if (!pixmap)
        return;

    if (filled)
    {
        // Sorting vertices by y-coordinate
        if (y1 > y2)
        {
            int tmp;
            tmp = y1;
            y1 = y2;
            y2 = tmp;
            tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        if (y1 > y3)
        {
            int tmp;
            tmp = y1;
            y1 = y3;
            y3 = tmp;
            tmp = x1;
            x1 = x3;
            x3 = tmp;
        }
        if (y2 > y3)
        {
            int tmp;
            tmp = y2;
            y2 = y3;
            y3 = tmp;
            tmp = x2;
            x2 = x3;
            x3 = tmp;
        }

        int dx1 = x2 - x1, dy1 = y2 - y1, dx2 = x3 - x1, dy2 = y3 - y1, dx3 = x3 - x2, dy3 = y3 - y2;
        float slope1 = dy1 ? (float)dx1 / dy1 : 0;
        float slope2 = dy2 ? (float)dx2 / dy2 : 0;
        float slope3 = dy3 ? (float)dx3 / dy3 : 0;

        for (int y = y1; y <= y2; y++)
        {
            int x_start = x1 + (y - y1) * slope1;
            int x_end = x1 + (y - y1) * slope2;
            if (x_start > x_end)
            {
                int tmp = x_start;
                x_start = x_end;
                x_end = tmp;
            }
            for (int x = x_start; x <= x_end; x++)
            {
                nnDrawPixel(pixmap, x, y, color);
            }
        }
        for (int y = y2; y <= y3; y++)
        {
            int x_start = x2 + (y - y2) * slope3;
            int x_end = x1 + (y - y1) * slope2;
            if (x_start > x_end)
            {
                int tmp = x_start;
                x_start = x_end;
                x_end = tmp;
            }
            for (int x = x_start; x <= x_end; x++)
            {
                nnDrawPixel(pixmap, x, y, color);
            }
        }
    }
    else
    {
        nnDrawLine(pixmap, x1, y1, x2, y2, color);
        nnDrawLine(pixmap, x2, y2, x3, y3, color);
        nnDrawLine(pixmap, x3, y3, x1, y1, color);
    }
}

void nnDrawRect(nnPixmap *pixmap, int x, int y, int width, int height, nnColorf color, bool filled)
{
    if (!pixmap)
        return;

    if (filled)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                nnDrawPixel(pixmap, x + j, y + i, color);
            }
        }
    }
    else
    {
        nnDrawLine(pixmap, x, y, x + width, y, color);                   // Top
        nnDrawLine(pixmap, x, y + height, x + width, y + height, color); // Bottom
        nnDrawLine(pixmap, x, y, x, y + height, color);                  // Left
        nnDrawLine(pixmap, x + width, y, x + width, y + height, color);  // Right
    }
}

nnColorf nnReadPixel(nnPixmap *pixmap, int x, int y)
{
    if (!pixmap || x < 0 || y < 0 || x >= pixmap->width || y >= pixmap->height)
    {
        return (nnColorf){0, 0, 0, 0}; // Return transparent color for out-of-bounds or invalid pixmap
    }

    return pixmap->pixels[y * pixmap->width + x];
}

/*
 * Text Rendering
 */

nnFont *nnLoadFont(const char *filepath, float fontSize)
{
    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        printf("Failed to open font file: %s\n", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = malloc(size);
    if (!buffer)
    {
        printf("Failed to allocate memory for font file: %s\n", filepath);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, size, file);
    fclose(file);

    nnFont *font = _nnLoadFont(buffer, size, fontSize);
    free(buffer);
    return font;
}

nnFont *nnLoadFontMem(const unsigned char *data, size_t dataSize, float fontSize)
{
    return _nnLoadFont(data, dataSize, fontSize);
}

void nnSetFont(nnFont *font)
{
    if (!font)
        return;

    _nnstate.font = font;
}

nnFont *nnGetFont()
{
    if (!_nnstate.font)
        return NULL;

    return _nnstate.font;
}

void nnDrawText(const char *format, int x, int y, ...)
{
    if (!format)
        return;

    if (!_nnstate.font)
    {
        _nnDrawTextFallback(format, x, y);
        return;
    }

    char buffer[1024];
    va_list args;
    va_start(args, y);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    nnFont *font = _nnstate.font;
    if (!font)
        return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, font->textureID);

    glBegin(GL_QUADS);

    float cursorX = (float)x;
    float cursorY = (float)(y + font->glyphHeight); // Adjust for font height

    for (char *c = buffer; *c != '\0'; c++)
    {
        if (*c < 32 || *c >= 128)
            continue;

        stbtt_aligned_quad quad;
        stbtt_GetBakedQuad(font->charData, font->atlasWidth, font->atlasHeight, *c - 32, &cursorX, &cursorY, &quad, 1);

        glTexCoord2f(quad.s0, quad.t1);
        glVertex2f(quad.x0, quad.y1);
        glTexCoord2f(quad.s1, quad.t1);
        glVertex2f(quad.x1, quad.y1);
        glTexCoord2f(quad.s1, quad.t0);
        glVertex2f(quad.x1, quad.y0);
        glTexCoord2f(quad.s0, quad.t0);
        glVertex2f(quad.x0, quad.y0);
    }

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

float nnTextWidth(const char *format, ...)
{
    if (!format)
    {
        return 0.0f; // Return 0 for null text
    }

    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    nnFont *font = nnGetFont();
    if (!font)
    {
        printf("No font set. Returning default text width as 0.\n");
        return 0.0f;
    }

    float width = 0.0f;
    for (const char *c = buffer; *c != '\0'; c++)
    {
        if (*c < 32 || *c >= 128)
        {
            continue; // Skip non-ASCII characters
        }

        int advanceWidth, leftSideBearing;
        stbtt_GetCodepointHMetrics(&font->fontInfo, *c, &advanceWidth, &leftSideBearing);

        // Add the scaled advanceWidth to the total width
        width += font->scale * advanceWidth;
    }

    return width;
}

float nnTextHeight()
{
    nnFont *font = nnGetFont();
    if (!font)
    {
        printf("No font set. Returning default glyph height as 0.\n");
        return 0.0f;
    }
    return font->glyphHeight;
}

void nnFreeFont(nnFont *font)
{
    if (!font)
        return;

    if (font->fontBuffer)
        free(font->fontBuffer);

    glDeleteTextures(1, &font->textureID);
    free(font);
}

/*
 * Collision Handling
 */

bool nnPosRecOverlaps(int x, int y, nnRecf rec)
{
    return (x >= rec.x && x <= rec.x + rec.width && y >= rec.y && y <= rec.y + rec.height) ? true : false;
}

bool nnRecsOverlap(nnRecf rec1, nnRecf rec2)
{
    return (rec1.x < rec2.x + rec2.width && rec1.x + rec1.width > rec2.x && rec1.y < rec2.y + rec2.height && rec1.y + rec1.height > rec2.y) ? true : false;
}

bool nnPosCircleOverlaps(int x, int y, int cx, int cy, float circleRadius)
{
    float dx = x - cx;
    float dy = y - cy;
    return (dx * dx + dy * dy <= circleRadius * circleRadius) ? true : false;
}

bool nnRecCircleOverlaps(nnRecf rec, int cx, int cy, float circleRadius)
{
    // Find the closest point on the rectangle to the circle center
    float closestX = (cx < rec.x) ? rec.x : (cx > rec.x + rec.width) ? rec.x + rec.width
                                                                     : cx;
    float closestY = (cy < rec.y) ? rec.y : (cy > rec.y + rec.height) ? rec.y + rec.height
                                                                      : cy;

    // Calculate the distance between the circle center and this closest point
    float dx = cx - closestX;
    float dy = cy - closestY;

    return (dx * dx + dy * dy <= circleRadius * circleRadius) ? true : false;
}

bool nnCirclesOverlaps(int cx1, int cy1, float circle1Radius, int cx2, int cy2, float circle2Radius)
{
    float dx = cx1 - cx2;
    float dy = cy1 - cy2;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = circle1Radius + circle2Radius;
    return (distanceSquared <= radiusSum * radiusSum) ? true : false;
}

/*
 * Input Handling
 */
//// Keyboard Input
bool nnKeyHit(int key)
{
    bool wasPressed = _nnstate.keysPressed[key];
    _nnstate.keysPressed[key] = false; // Reset after checking
    return wasPressed;
}

bool nnKeyDown(int key)
{
    return _nnstate.keys[key];
}

bool nnKeyReleased(int key)
{
    bool wasReleased = _nnstate.keysReleased[key];
    _nnstate.keysReleased[key] = false; // Reset after checking
    return wasReleased;
}

//// Mouse Input

bool nnMouseHit(int button)
{
    bool wasPressed = _nnstate.mouseButtonsPressed[button];
    _nnstate.mouseButtonsPressed[button] = false; // Reset after checking
    return wasPressed;
}

bool nnMouseDown(int button)
{
    return _nnstate.mouseButtons[button];
}

bool nnMouseReleased(int button)
{
    bool wasReleased = _nnstate.mouseButtonsReleased[button];
    _nnstate.mouseButtonsReleased[button] = false; // Reset after checking
    return wasReleased;
}

int nnMouseWheelDelta()
{
    int delta = _nnstate.mouseWheelDelta;
    _nnstate.mouseWheelDelta = 0; // Reset after checking
    return delta;
}

nnPos nnGetMousePosition()
{
    return _nnstate.mousePosition;
}

nnPos nnMouseMotionDelta()
{
    nnPos delta = _nnstate.mouseMotionDelta;
    _nnstate.mouseMotionDelta.x = 0; // Reset after checking
    _nnstate.mouseMotionDelta.y = 0;
    return delta;
}

/*
 * Utility
 */

unsigned char *nnLoadFileBytes(const char *filepath, int *size)
{
    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        printf("Failed to open file:\n%s\n", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(*size);
    if (!buffer)
    {
        printf("Failed to allocate memory for file: %s\n", filepath);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, *size, file);
    fclose(file);

    return buffer;
}

void nnFreeFileBytes(unsigned char *buffer)
{
    if (buffer)
    {
        free(buffer);
    }
}

float nnLerp(float min, float max, float speed, float ease)
{
    // Static variables to maintain state between calls
    static float currentValue = 0.0f;
    static float direction = 1.0f; // 1.0f for increasing, -1.0f for decreasing

    // Initialize currentValue on the first call
    if (currentValue == 0.0f)
    {
        currentValue = min;
    }

    // Calculate the change per frame
    float delta = speed * direction;

    // Update the current value
    currentValue += delta;

    // Check bounds and reverse direction if necessary
    if (currentValue > max)
    {
        currentValue = max;
        direction = -1.0f;
        currentValue -= delta * ease; // Smooth transition when changing direction
    }
    else if (currentValue < min)
    {
        currentValue = min;
        direction = 1.0f;
        currentValue += delta * 0.5f; // Smooth transition when changing direction
    }

    return currentValue;
}

/*
 * GUI
 */

bool nnButton(const char *format, int x, int y, int width, int height, ...)
{
    // Get mouse state
    nnPos mousePos = nnGetMousePosition();
    bool hovered = nnPosRecOverlaps(mousePos.x, mousePos.y, (nnRecf){x, y, width, height});
    bool pressed = hovered && nnMouseDown(0);      // Left mouse button down
    bool released = hovered && nnMouseReleased(0); // Left mouse button released

    // Determine button color based on state
    nnColorf bgColor = pressed ? _nnCurrentTheme.primaryColorAccent
                               : (hovered ? _nnCurrentTheme.secondaryColorAccent
                                          : _nnCurrentTheme.primaryColor);
    nnColorf borderColor = _nnCurrentTheme.borderColor;
    nnColorf textColor = _nnCurrentTheme.textPrimaryColor;

    // Draw button background quad
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glColor4f(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw button border
    glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw button text
    if (format)
    {
        char buffer[1024];
        va_list args;
        va_start(args, height);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        int tw = (int)nnTextWidth(buffer);
        int th = (int)nnTextHeight();

        // Calculate the true centered position
        int textX = x + (width / 2 - tw / 2);
        int textY = y + (height / 2 - th / 2);

        // Draw the text
        glColor4f(textColor.r, textColor.g, textColor.b, textColor.a);
        nnDrawText(buffer, textX, textY);
    }

    glDisable(GL_BLEND);

    // Return true if the button was just released while hovered
    return released;
}

#endif // NONOGL_IMPLEMENTATION