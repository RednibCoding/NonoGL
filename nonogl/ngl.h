#define FREEGLUT_STATIC

#ifndef NONO_GL_H
#define NONO_GL_H

#include <stdbool.h>
#include "internal/include/GL/freeglut.h"
#include "internal/include/stb_image.h"
#include "internal/include/stb_truetype.h"
#include "internal/include/key_definitions.h"

/******************************************************************************************************************************/

/*  Public Interface */

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
    nnColorf primaryColorMuted;    // Muted color for primary elements
    nnColorf secondaryColor;       // Secondary elements (e.g., backgrounds)
    nnColorf secondaryColorAccent; // Accent color for secondary elements
    nnColorf secondaryColorMuted;  // Muted color for secondary elements
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

// Render the given formatted text using the font set with `nnSetFont`. If no font has been set, the internal default font will be used.
void nnDrawText(const char *format, int x, int y, ...);

// Render the given formatted text using the font set with `nnSetFont` and a custom z-index (-1.0 to 1.0). If no font has been set, the internal default font will be used.
void nnDrawTextZ(const char *format, int x, int y, float zIndex, ...);

// Draws text when debug is enabled.
void nnDrawDebugText(const char *format, int x, int y, ...);

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
nnPos nnMousePosition();

// Returns the mouse motion delta (change in position) since the last frame.
nnPos nnMouseMotionDelta();

// Resets all key and mouse states.
void nnFlushKeys();

// Resets all mouse button states.
void nnFlushMouse();

/*
 * Utility
 */

// Load a file as bytes into a buffer and return the pointer to that buffer.
unsigned char *nnLoadFileBytes(const char *filepath, int *size);

// Free the buffer allocated by nnLoadFileBytes.
void nnFreeFileBytes(unsigned char *buffer);

// Lerps between min and max by the given speed. Use ease to define a smooth transition when changing direction.
float nnLerp(float min, float max, float speed, float ease);

// Enable/Disable debug mode.
void nnSetDebugMode(bool flag);

// Get wether debug mode in anabled or not.
bool nnIsDebugMode();

// Holds the current frames per second.
int nnFPS;

// Holds the delta time (time elapsed since the last frame).
float nnDT;

// Time in milliseconds since the application started.
unsigned int nnMS;

/*
 * GUI
 */

// A panel element with background color and border. Returns `true` if the mouse is hovering over it, otherwise `false`.
bool nnPanel(int x, int y, int width, int height);

// Displays text with an optional border. Wraps text within the defined width and clips it if it exceeds the provided height.
void nnLabel(const char *format, int x, int y, int width, int height, bool border, ...);

// A basic single line text input box.
int nnTextInput(char *buffer, int maxLength, int x, int y, int width, int height, const char *placeholder);

// Button that returns `true` when it has been clicked.
bool nnButton(const char *format, int x, int y, int width, int height, ...);

// Checkbox that returns `true` when it is checked, otherwise `false`
bool nnCheckbox(const char *format, bool isChecked, int x, int y, ...);

// Horizontal slider that returns the current set value.
float nnHSlider(float min, float max, float initial, float step, int x, int y, int height);

// Vertical slider that returns the current set value.
float nnVSlider(float min, float max, float initial, float step, int x, int y, int height);

// Horizontal progress bar that returns the current fill state in percent.
// `deltaFillState` determines how much the fillstate should increase/decrease on the next call.
int nnHProgressbar(float min, float max, float deltaFillState, int x, int y, int width);

// Vertical progress bar that returns the current fill state in percent.
// `deltaFillState` determines how much the fillstate should increase/decrease on the next call.
int nnVProgressbar(float min, float max, float deltaFillState, int x, int y, int width);

// A dropdown that accepts a list of options to choose from. Returns the selected index.
int nnDropdown(const char *buttonText, const char **options, int numOptions, int x, int y, int width, int height);

// A scrollable list that accepty items to choose from. Returns the selected index.
int nnScrollableList(const char **items, int numItems, int x, int y, int width, int height);

/******************************************************************************************************************************/
/*  End of Public Interface */
/******************************************************************************************************************************/

#endif // NONO_GL_H

/******************************************************************************************************************************/
/******************************************************************************************************************************/

/*  IMPLEMENTATION */

/******************************************************************************************************************************/
/******************************************************************************************************************************/
#ifdef NONOGL_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include "internal/include/stb_image.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "internal/include/stb_truetype.h"

#include "internal/include/key_definitions.h"
#include "internal/include/default_font.h"

#define _NN_MAX_KEYS 256
#define _NN_MAX_MOUSE_BUTTONS 3

typedef struct
{
    char *title;
    int initialWidth;
    int initialHeight;
    int windowWidth;
    int windowHeight;
    bool virtual;
    bool filtered;
    float deltaTime;
    double lastTime;
    int currentFPS;
    int targetFPS;
    int window;
    float windowScaleX;
    float windowScaleY;
    nnFont *font;
    nnColorf currentDrawColor;
    bool debugMode;

    // Keyboard state
    bool keys[_NN_MAX_KEYS];         // Normal key states
    bool keysPressed[_NN_MAX_KEYS];  // Normal key pressed states
    bool keysReleased[_NN_MAX_KEYS]; // Normal key released states

    bool skeys[_NN_MAX_KEYS];         // Special key states
    bool skeysPressed[_NN_MAX_KEYS];  // Special key pressed states
    bool skeysReleased[_NN_MAX_KEYS]; // Special key released states

    // Mouse state
    bool mouseButtons[_NN_MAX_MOUSE_BUTTONS];
    bool mouseButtonsPressed[_NN_MAX_MOUSE_BUTTONS];
    bool mouseButtonsReleased[_NN_MAX_MOUSE_BUTTONS];
    int mouseWheelDelta;
    nnPos mousePosition;
    nnPos mouseMotionDelta;

    // Gui
    bool isAnyPopupOpen;

    // Function pointer for the display callback
    void (*displayCallback)(void);

} _nnState;

static nnTheme _nnCurrentTheme = {
    .primaryColor = {0.2f, 0.5f, 0.8f, 1.0f},       // Blue
    .primaryColorAccent = {0.3f, 0.6f, 0.9f, 1.0f}, // Light Blue
    .primaryColorMuted = {0.6f, 0.7f, 0.8f, 1.0f},  // Muted Blue

    .secondaryColor = {0.1f, 0.1f, 0.1f, 1.0f},       // Dark Gray
    .secondaryColorAccent = {0.2f, 0.2f, 0.2f, 1.0f}, // Gray
    .secondaryColorMuted = {0.5f, 0.5f, 0.5f, 1.0f},  // Muted Gray

    .borderColor = {0.5f, 0.5f, 0.5f, 1.0f},       // Medium Gray
    .borderColorAccent = {0.7f, 0.7f, 0.7f, 1.0f}, // Light Gray

    .textPrimaryColor = {0.9f, 0.9f, 0.9f, 1.0f},   // Light Gray
    .textSecondaryColor = {0.4f, 0.4f, 0.4f, 1.0f}, // Dark Gray
};

#define _NN_TEXT_INPUT_MAX_LENGTH 1024
#define _NN_MAX_TEXTINPUT_STATES 128
typedef struct
{
    unsigned int id;
    char text[_NN_TEXT_INPUT_MAX_LENGTH];
    int cursorIndex;
    int selectionStart;
    int selectionEnd;
    bool hasFocus;
    float scrollOffsetX; // Horizontal scroll position
} _nnTextInputState;

static _nnTextInputState _nnTextInputStates[_NN_MAX_TEXTINPUT_STATES];
static int _nnTextInputStateCount = 0;

typedef struct
{
    unsigned int id;
    bool isChecked;
} _nnCheckboxState;

#define _NN_MAX_CHECKBOX_STATES 64
static _nnCheckboxState _nnCheckboxStates[_NN_MAX_CHECKBOX_STATES];
static int _nnCheckboxStateCount = 0;

typedef struct
{
    unsigned int id;
    float value;
} _nnSliderState;

#define _NN_MAX_SLIDER_STATES 64
static _nnSliderState _nnSliderStates[_NN_MAX_SLIDER_STATES];
static int _nnSliderStateCount = 0;

typedef struct
{
    unsigned int id;
    float fillState;
} _nnProgressbarState;

#define _NN_MAX_PROGRESSBAR_STATES 64
static _nnProgressbarState _nnProgressbarStates[_NN_MAX_PROGRESSBAR_STATES];
static int _nnProgressbarStateCount = 0;

typedef struct
{
    unsigned int id;
    bool initialized;  // Track initialization of the button text
    bool isOpen;       // Whether the dropdown list is open
    int selectedIndex; // Selected option index (-1 means no selection)
    int scrollOffset;  // Tracks the scroll offset in terms of options
    char selectedText[256];
} _nnDropdownState;

#define _NN_MAX_DROPDOWN_STATES 64
static _nnDropdownState _nnDropdownStates[_NN_MAX_DROPDOWN_STATES];
static int _nnDropdownStateCount = 0;

typedef struct
{
    unsigned int id;
    int selectedIndex;
    int scrollOffset;
} _nnScrollableListState;

#define _NN_MAX_SCROLLABLELIST_STATES 64
static _nnScrollableListState _nnScrollableListStates[_NN_MAX_SCROLLABLELIST_STATES];
static int _nnScrollableListStateCount = 0;

static _nnState _nnstate;

#define _NN_Z_INDEX_POPUP 0.1f
#define _NN_Z_INDEX_POPUP_TEXT 0.2f

// Generate a unique id based on two given integer values.
unsigned int _nnGenUID(int x, int y)
{
    unsigned int hash = 2166136261u; // FNV offset basis
    hash ^= (unsigned int)x;
    hash *= 16777619u;
    hash ^= (unsigned int)y;
    hash *= 16777619u;
    return hash;
}

// Wrapper display function that calls the function pointer
static void _nnDisplayCallbackWrapper()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (_nnstate.displayCallback != NULL)
    {
        _nnstate.displayCallback();
    }

    nnFlushKeys();
    nnFlushMouse();
    _nnstate.mouseWheelDelta = 0;
    _nnstate.mouseMotionDelta.x = 0;
    _nnstate.mouseMotionDelta.y = 0;

    glutSwapBuffers();
    glutMainLoopEvent();
}

static void _nnSetupOrthoProjection(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void _nnSetScale(float scaleX, float scaleY)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(scaleX, scaleY, 1.0f);
}

static void _nnUpdateScale(int width, int height, int shouldUpdate)
{
    if (shouldUpdate != 0)
    {
        _nnstate.windowScaleX = (float)width / _nnstate.initialWidth;
        _nnstate.windowScaleY = (float)height / _nnstate.initialHeight;
        _nnSetScale(_nnstate.windowScaleX, _nnstate.windowScaleY);
    }
}

static void _nnFramebufferSizeCallback(int width, int height)
{
    _nnstate.windowWidth = width;
    _nnstate.windowHeight = height;
    glViewport(0, 0, width, height);
    _nnSetupOrthoProjection(width, height);
    _nnUpdateScale(width, height, _nnstate.virtual);
}

static void _nnTimerCallback(int value)
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

static void _nnKeyDownCallback(unsigned char key, int x, int y)
{
    if (!_nnstate.keys[key]) // Normal keys (0-255)
    {
        _nnstate.keysPressed[key] = true;
    }
    _nnstate.keys[key] = true;
}

static void _nnKeyUpCallback(unsigned char key, int x, int y)
{
    _nnstate.keys[key] = false;
    _nnstate.keysReleased[key] = true;
}

static void _nnSpecialKeyCallback(int key, int x, int y)
{
    if (!_nnstate.skeys[key]) // Special keys (e.g., GLUT keys)
    {
        _nnstate.skeysPressed[key] = true;
    }
    _nnstate.skeys[key] = true;
}

static void _nnSpecialKeyUpCallback(int key, int x, int y)
{
    _nnstate.skeys[key] = false;
    _nnstate.skeysReleased[key] = true;
}

static void _nnMouseButtonCallback(int button, int state, int x, int y)
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

static void _nnMouseWheelFunc(int wheel, int direction, int x, int y)
{
    _nnstate.mouseWheelDelta = direction; // +1 for up, -1 for down
}

static void _nnMouseMotionFunc(int x, int y)
{
    // Motion delta
    nnPos newMousePosition = {(float)x / _nnstate.windowScaleX, (float)y / _nnstate.windowScaleY};
    _nnstate.mouseMotionDelta.x = newMousePosition.x - _nnstate.mousePosition.x;
    _nnstate.mouseMotionDelta.y = newMousePosition.y - _nnstate.mousePosition.y;
    _nnstate.mousePosition = newMousePosition;
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

    // Calculate height from actual glyphs (averaging between capital and lowercase letters)
    int minYCap, maxYCap, minYLower, maxYLower;

    // Get bounding box for a capital letter (e.g., 'A')
    stbtt_GetCodepointBox(&font->fontInfo, 'A', NULL, &minYCap, NULL, &maxYCap);

    // Get bounding box for a lowercase letter (e.g., 'x')
    stbtt_GetCodepointBox(&font->fontInfo, 'x', NULL, &minYLower, NULL, &maxYLower);

    // Scale and average the heights
    float capHeight = font->scale * (maxYCap - minYCap);
    float lowerHeight = font->scale * (maxYLower - minYLower);
    font->glyphHeight = (capHeight + lowerHeight) / 2.0f;

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

static void _nnDrawTextVA(const char *format, int x, int y, float zIndex, va_list args)
{
    if (!format)
        return;

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);

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
        glVertex3f(quad.x0, quad.y1, zIndex);
        glTexCoord2f(quad.s1, quad.t1);
        glVertex3f(quad.x1, quad.y1, zIndex);
        glTexCoord2f(quad.s1, quad.t0);
        glVertex3f(quad.x1, quad.y0, zIndex);
        glTexCoord2f(quad.s0, quad.t0);
        glVertex3f(quad.x0, quad.y0, zIndex);
    }

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

// static const char *_nnFetchTextInput()
// {
//     static char typedText[1024];
//     int index = 0;

//     // Clear the buffer before starting
//     typedText[0] = '\0';

//     // Iterate over printable ASCII characters (32-126)
//     for (int key = 32; key <= 126; key++)
//     {
//         bool pressed = _nnstate.keysPressed[key];
//         if (pressed) // Check if the key is pressed
//         {
//             typedText[index++] = (char)key; // Add the character to the buffer
//         }
//     }

//     // Handle backspace
//     // if (nnKeyHit(8)) // Backspace key code
//     // {
//     //     typedText[index++] = '\b'; // Add backspace marker
//     // }

//     // Handle enter (if needed for a text input)
//     // if (nnKeyHit(13)) // Enter key code
//     // {
//     //     typedText[index++] = '\n'; // Add newline marker
//     // }

//     // Null-terminate the string
//     typedText[index] = '\0';

//     return typedText;
// }

static const char *_nnFetchTextInput()
{
    static char typedText[1024];
    int index = 0;

    // Clear the buffer before starting
    typedText[0] = '\0';

    // Iterate over printable ASCII characters (32-126)
    for (int key = 32; key <= 126; key++)
    {
        if (_nnstate.keysPressed[key]) // Check only normal keys
        {
            typedText[index++] = (char)key; // Add the character to the buffer
        }
    }

    // Null-terminate the string
    typedText[index] = '\0';

    return typedText;
}

/*******************************************************************************************************/
/*******************************************************************************************************/

// Public Interface implementations

/*******************************************************************************************************/
/*******************************************************************************************************/

/*
 * Window Management
 */
bool nnCreateWindow(char *title, int width, int height, bool virtual, bool filtered)
{
    _nnstate.displayCallback = NULL;

    _nnstate.virtual = virtual;
    _nnstate.filtered = filtered;

    _nnstate.title = title;
    _nnstate.initialWidth = width;
    _nnstate.initialHeight = height;
    _nnstate.windowWidth = width;
    _nnstate.windowHeight = height;
    _nnstate.windowScaleX = 1.0f;
    _nnstate.windowScaleY = 1.0f;

    _nnstate.deltaTime = 0.0f;
    _nnstate.lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    _nnstate.currentFPS = 0;

    _nnstate.isAnyPopupOpen = false;

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
    glutSpecialFunc(_nnSpecialKeyCallback);
    glutSpecialUpFunc(_nnSpecialKeyUpCallback);
    glutMouseFunc(_nnMouseButtonCallback);
    glutMouseWheelFunc(_nnMouseWheelFunc);
    glutMotionFunc(_nnMouseMotionFunc);
    glutPassiveMotionFunc(_nnMouseMotionFunc);
    glutReshapeFunc(_nnFramebufferSizeCallback);

    /* Set up the orthographic projection */
    _nnSetupOrthoProjection(_nnstate.windowWidth, _nnstate.windowHeight);
    _nnUpdateScale(_nnstate.windowWidth, _nnstate.windowHeight, _nnstate.virtual);

    /* Set initial scale */
    _nnSetScale(_nnstate.windowScaleX, _nnstate.windowScaleY);

    /* Start the timer for limiting FPS */
    glutTimerFunc(1000 / _nnstate.targetFPS, _nnTimerCallback, 0);

    nnResetColor();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    return true;
}

void nnSetRenderFunc(void (*callback)(void))
{
    _nnstate.displayCallback = callback;
    glutDisplayFunc(_nnDisplayCallbackWrapper);
}

void nnDestroyWindow()
{
    glDisable(GL_DEPTH_TEST);
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
    char buffer[256] = {0};
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

int nnWindowWidth()
{
    return _nnstate.windowWidth;
}

int nnWindowHeight()
{
    return _nnstate.windowHeight;
}

int nnScreenWidth()
{
    if (_nnstate.virtual)
    {
        return _nnstate.initialWidth;
    }
    return _nnstate.windowWidth;
}

int nnScreenHeight()
{
    if (_nnstate.virtual)
    {
        return _nnstate.initialHeight;
    }
    return _nnstate.windowHeight;
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

    va_list args;
    va_start(args, y);
    _nnDrawTextVA(format, x, y, 0.0f, args);
    va_end(args);
}

void nnDrawTextZ(const char *format, int x, int y, float zIndex, ...)
{
    if (!format)
        return;

    va_list args;
    va_start(args, zIndex);
    _nnDrawTextVA(format, x, y, zIndex, args);
    va_end(args);
}

void nnDrawDebugText(const char *format, int x, int y, ...)
{
    if (!_nnstate.debugMode)
        return;

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
    if (key < _NN_MAX_KEYS) // Normal key
    {
        return _nnstate.keysPressed[key];
    }
    else if (key < _NN_MAX_KEYS + _NN_MAX_KEYS) // Special key
    {
        int specialKey = key - _NN_MAX_KEYS; // Map to special key range
        return _nnstate.skeysPressed[specialKey];
    }
    return false; // Invalid key
}

bool nnKeyDown(int key)
{
    if (key < _NN_MAX_KEYS) // Normal key
    {
        return _nnstate.keys[key];
    }
    else if (key < _NN_MAX_KEYS + _NN_MAX_KEYS) // Special key
    {
        int specialKey = key - _NN_MAX_KEYS; // Map to special key range
        return _nnstate.skeys[specialKey];
    }
    return false; // Invalid key
}

bool nnKeyReleased(int key)
{
    if (key < _NN_MAX_KEYS) // Normal key
    {
        return _nnstate.keysReleased[key];
    }
    else if (key < _NN_MAX_KEYS + _NN_MAX_KEYS) // Special key
    {
        int specialKey = key - _NN_MAX_KEYS; // Map to special key range
        return _nnstate.skeysReleased[specialKey];
    }
    return false; // Invalid key
}

//// Mouse Input

bool nnMouseHit(int button)
{
    bool wasPressed = _nnstate.mouseButtonsPressed[button];
    return wasPressed;
}

bool nnMouseDown(int button)
{
    return _nnstate.mouseButtons[button];
}

bool nnMouseReleased(int button)
{
    bool wasReleased = _nnstate.mouseButtonsReleased[button];
    return wasReleased;
}

int nnMouseWheelDelta()
{
    int delta = _nnstate.mouseWheelDelta;
    return delta;
}

nnPos nnMousePosition()
{
    return _nnstate.mousePosition;
}

nnPos nnMouseMotionDelta()
{
    nnPos delta = _nnstate.mouseMotionDelta;
    return delta;
}

void nnFlushKeys()
{
    for (int key = 0; key < _NN_MAX_KEYS; key++)
    {
        _nnstate.keysPressed[key] = false;
        _nnstate.keysReleased[key] = false;
        _nnstate.skeysPressed[key] = false;
        _nnstate.skeysReleased[key] = false;
    }
}

void nnFlushMouse()
{
    for (int button = 0; button < _NN_MAX_MOUSE_BUTTONS; button++)
    {
        _nnstate.mouseButtonsPressed[button] = false;
        _nnstate.mouseButtonsReleased[button] = false;
    }
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

void nnSetDebugMode(bool flag)
{
    _nnstate.debugMode = flag;
}

bool nnIsDebugMode()
{
    return _nnstate.debugMode;
}

/*
 * GUI
 */

bool nnPanel(int x, int y, int width, int height)
{
    // Define the background and border colors
    nnColorf bgColor = _nnCurrentTheme.secondaryColor;  // Background color
    nnColorf borderColor = _nnCurrentTheme.borderColor; // Border color

    nnPos mousePos = nnMousePosition();
    bool hovered = _nnstate.isAnyPopupOpen ? false : nnPosRecOverlaps(mousePos.x, mousePos.y, (nnRecf){x, y, width, height});

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw the background
    glBegin(GL_QUADS);
    glColor4f(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw the border
    glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Disable blending
    glDisable(GL_BLEND);

    return hovered;
}

void nnLabel(const char *format, int x, int y, int width, int height, bool border, ...)
{
    if (!format)
        return;

    // Format the text
    char buffer[1024];
    va_list args;
    va_start(args, border);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Prepare to draw
    nnColorf textColor = _nnCurrentTheme.textPrimaryColor;
    nnColorf borderColor = _nnCurrentTheme.borderColor;
    nnColorf bgColor = _nnCurrentTheme.secondaryColor;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw background and border if enabled
    if (border)
    {
        // Draw background
        glBegin(GL_QUADS);
        glColor4f(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();

        // Draw border
        glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
        glEnd();
    }

    // Split text into words for wrapping
    const char *current = buffer;
    char line[256];
    int lineHeight = nnTextHeight() + 12;
    int padding = 8;
    int maxLines = (height - padding) / lineHeight;

    int currentY = y;
    int linesDrawn = 0;

    while (*current != '\0' && linesDrawn < maxLines)
    {
        // Accumulate words into a line
        int lastBreak = 0;
        bool isLastLine = (linesDrawn == maxLines - 1);
        bool truncated = false;

        for (int i = 0; current[i] != '\0'; i++)
        {
            line[i] = current[i];
            line[i + 1] = '\0';

            // Break line on spaces or forced newlines
            if (current[i] == ' ' || current[i] == '\n')
                lastBreak = i;

            // If the line is too wide, break it
            if (nnTextWidth(line) > width - padding) // Account for padding
            {
                if (lastBreak == 0) // No break point, force truncation
                {
                    line[i] = '\0';
                    current += i;
                }
                else // Break at the last space or newline
                {
                    line[lastBreak] = '\0';
                    current += lastBreak + 1;
                }
                break;
            }

            // Handle end of text
            if (current[i + 1] == '\0')
            {
                current += i + 1;
                break;
            }
        }

        // If this is the last line and there's more text, truncate it and add "..."
        if (isLastLine && *current != '\0')
        {
            truncated = true;
            int len = strlen(line);
            while (len > 0 && nnTextWidth(line) + nnTextWidth("...") > width - padding)
            {
                line[--len] = '\0';
            }
            strcat(line, "...");
        }

        // Draw the line
        glColor4f(textColor.r, textColor.g, textColor.b, textColor.a);
        nnDrawText(line, x + padding, currentY + padding); // Add slight padding
        currentY += lineHeight;
        linesDrawn++;

        // Stop drawing if truncated
        if (truncated)
            break;
    }

    glDisable(GL_BLEND);
}

int nnTextInput(char *buffer, int maxLength, int x, int y, int width, int height, const char *placeholder)
{
    // Generate a unique ID based on position
    unsigned int id = _nnGenUID(x, y);

    // Find or initialize state
    _nnTextInputState *state = NULL;
    for (int i = 0; i < _nnTextInputStateCount; i++)
    {
        if (_nnTextInputStates[i].id == id)
        {
            state = &_nnTextInputStates[i];
            break;
        }
    }
    if (!state)
    {
        if (_nnTextInputStateCount >= _NN_MAX_TEXTINPUT_STATES)
        {
            printf("Error: Too many text inputs! Increase _NN_MAX_TEXTINPUT_STATES.\n");
            return -1;
        }
        state = &_nnTextInputStates[_nnTextInputStateCount++];
        state->id = id;
        state->cursorIndex = 0;
        state->selectionStart = 0;
        state->selectionEnd = 0;
        state->hasFocus = false;
        state->scrollOffsetX = 0.0f;
        state->text[0] = '\0';

        // Initialize the internal text state with the external buffer if provided
        if (buffer && buffer[0] != '\0')
        {
            strncpy(state->text, buffer, sizeof(state->text) - 1);
            state->text[sizeof(state->text) - 1] = '\0';
            state->cursorIndex = strlen(state->text);
        }
    }

    // Get mouse position and input states
    nnPos mousePos = nnMousePosition();
    bool hovering = nnPosRecOverlaps(mousePos.x, mousePos.y, (nnRecf){x, y, width, height});

    // Focus handling
    if (hovering && nnMouseReleased(0))
    {
        state->hasFocus = true;
    }
    else if (!hovering && nnMouseReleased(0))
    {
        state->hasFocus = false;
    }

    const int padding = 4;

    // Text input handling (only if focused)
    if (state->hasFocus)
    {
        const char *inputText = _nnFetchTextInput(); // Fetch input text
        if (inputText && *inputText)
        {
            // Insert text at cursor position
            int inputLength = strlen(inputText);
            int remainingSpace = maxLength - strlen(state->text) - 1;

            if (inputLength > remainingSpace)
                inputLength = remainingSpace;

            // Replace selected text or insert at cursor
            if (state->selectionStart != state->selectionEnd)
            {
                int selStart = state->selectionStart < state->selectionEnd ? state->selectionStart : state->selectionEnd;
                int selEnd = state->selectionStart > state->selectionEnd ? state->selectionStart : state->selectionEnd;
                memmove(&state->text[selStart], &state->text[selEnd], strlen(&state->text[selEnd]) + 1);
                state->cursorIndex = selStart;
            }

            // Insert new text
            memmove(&state->text[state->cursorIndex + inputLength], &state->text[state->cursorIndex], strlen(&state->text[state->cursorIndex]) + 1);
            memcpy(&state->text[state->cursorIndex], inputText, inputLength);
            state->cursorIndex += inputLength;
            state->selectionStart = state->selectionEnd = state->cursorIndex;
        }

        // Handle backspace
        if (nnKeyHit(nnVK_BACKSPACE))
        {
            if (state->selectionStart != state->selectionEnd)
            {
                // Delete selected text
                int selStart = state->selectionStart < state->selectionEnd ? state->selectionStart : state->selectionEnd;
                int selEnd = state->selectionStart > state->selectionEnd ? state->selectionStart : state->selectionEnd;
                memmove(&state->text[selStart], &state->text[selEnd], strlen(&state->text[selEnd]) + 1);
                state->cursorIndex = selStart;
            }
            else if (state->cursorIndex > 0)
            {
                // Delete one character
                memmove(&state->text[state->cursorIndex - 1], &state->text[state->cursorIndex], strlen(&state->text[state->cursorIndex]) + 1);
                state->cursorIndex--;
            }
            state->selectionStart = state->selectionEnd = state->cursorIndex;
        }

        // Handle delete
        if (nnKeyHit(nnVK_DELETE))
        {
            if (state->selectionStart != state->selectionEnd)
            {
                // Delete selected text
                int selStart = state->selectionStart < state->selectionEnd ? state->selectionStart : state->selectionEnd;
                int selEnd = state->selectionStart > state->selectionEnd ? state->selectionStart : state->selectionEnd;
                memmove(&state->text[selStart], &state->text[selEnd], strlen(&state->text[selEnd]) + 1);
                state->cursorIndex = selStart;
            }
            else if (state->cursorIndex < strlen(state->text))
            {
                // Delete one character to the right of the cursor
                memmove(&state->text[state->cursorIndex], &state->text[state->cursorIndex + 1], strlen(&state->text[state->cursorIndex + 1]) + 1);
            }
            state->selectionStart = state->selectionEnd = state->cursorIndex;
        }

        // Handle arrow keys
        if (nnKeyHit(nnVK_LEFT) && state->cursorIndex > 0)
        {
            state->cursorIndex--;
            state->selectionStart = state->selectionEnd = state->cursorIndex;

            // Ensure the cursor stays visible
            float cursorX = x + padding + nnTextWidth(&state->text[0]) - nnTextWidth(&state->text[state->cursorIndex]);
            if (cursorX < x + padding)
            {
                state->scrollOffsetX -= nnTextWidth(&state->text[state->cursorIndex]) - nnTextWidth(&state->text[state->cursorIndex - 1]);
                if (state->scrollOffsetX < 0)
                    state->scrollOffsetX = 0;
            }
        }

        if (nnKeyHit(nnVK_RIGHT) && state->cursorIndex < strlen(state->text))
        {
            state->cursorIndex++;
            state->selectionStart = state->selectionEnd = state->cursorIndex;

            // Ensure the cursor stays visible
            float cursorX = x + padding + nnTextWidth(&state->text[0]) - nnTextWidth(&state->text[state->cursorIndex]);
            if (cursorX > x + width - padding)
            {
                state->scrollOffsetX += nnTextWidth(&state->text[state->cursorIndex]) - nnTextWidth(&state->text[state->cursorIndex - 1]);
            }
        }

        // Handle Home/End keys
        if (nnKeyHit(nnVK_HOME))
        {
            state->cursorIndex = 0;
            state->selectionStart = state->selectionEnd = state->cursorIndex;
            state->scrollOffsetX = 0; // Scroll to the start
        }

        if (nnKeyHit(nnVK_END))
        {
            // Move cursor to the end of the actual text
            state->cursorIndex = strlen(state->text);
            state->selectionStart = state->selectionEnd = state->cursorIndex;

            // Scroll to the end of the text
            float textWidth = nnTextWidth(state->text);
            if (textWidth > width - padding * 2)
            {
                // If the text is wider than the visible width, scroll to the end
                state->scrollOffsetX = textWidth - (width - padding * 2);
            }
            else
            {
                // If text fits, no need to scroll
                state->scrollOffsetX = 0;
            }
        }
    }

    // Calculate text width and cursor position
    float cursorX = x + padding + nnTextWidth(&state->text[0]) - nnTextWidth(&state->text[state->cursorIndex]);

    // Adjust scroll offset for the cursor position
    if (cursorX < x + padding)
    {
        // Cursor is left of the visible area
        state->scrollOffsetX = nnTextWidth(&state->text[0]) - nnTextWidth(&state->text[state->cursorIndex]) - padding;
    }
    else if (cursorX > x + width - padding)
    {
        // Cursor is right of the visible area
        state->scrollOffsetX = nnTextWidth(&state->text[0]) - nnTextWidth(&state->text[state->cursorIndex]) + padding - width;
    }

    // Draw text input background and border
    nnColorf bgColor = _nnCurrentTheme.secondaryColor;
    nnColorf borderColor = state->hasFocus ? _nnCurrentTheme.primaryColor : _nnCurrentTheme.borderColor;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glColor4f(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw cursor
    if (state->hasFocus)
    {
        // Calculate the width of the text up to the cursor position
        float cursorDrawX = x + padding + nnTextWidth(&state->text[0]) - nnTextWidth(&state->text[state->cursorIndex]) - state->scrollOffsetX;

        // Clamp the cursor position to the visible area
        if (cursorDrawX < x + padding)
        {
            cursorDrawX = x + padding;
        }
        else if (cursorDrawX > x + width - padding)
        {
            cursorDrawX = x + width - padding;
        }

        // Draw the cursor line
        glBegin(GL_LINES);
        glVertex2f(cursorDrawX, y + padding);
        glVertex2f(cursorDrawX, y + height - padding);
        glEnd();
    }

    // Set scissor for clipping
    glEnable(GL_SCISSOR_TEST);
    glScissor(x + padding, nnScreenHeight() - (y + height), width - padding * 2, height);

    const char *placeholderText = state->text;
    const bool hasEnteredText = strlen(state->text) > 0;
    // Draw placeholder if textbox is empty and placeholder exists
    if (!hasEnteredText && placeholder)
    {
        nnColorf placeholderColor = _nnCurrentTheme.textSecondaryColor;
        glColor4f(placeholderColor.r, placeholderColor.g, placeholderColor.b, placeholderColor.a);
        placeholderText = placeholder;
        nnDrawText(placeholderText, x + 4, y + (height - nnTextHeight()) / 2);
    }
    else if (hasEnteredText) // Otherwise the entered text
    {
        nnColorf textColor = _nnCurrentTheme.textPrimaryColor;
        glColor4f(textColor.r, textColor.g, textColor.b, textColor.a);
        nnDrawText(state->text, x + padding - state->scrollOffsetX, y + (height - nnTextHeight()) / 2);
    }

    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_BLEND);

    // Sync internal state back to external buffer
    if (buffer)
    {
        strncpy(buffer, state->text, maxLength - 1);
        buffer[maxLength - 1] = '\0';
    }

    return state->cursorIndex;
}

bool nnButton(const char *format, int x, int y, int width, int height, ...)
{
    // Get mouse state
    nnPos mousePos = nnMousePosition();
    bool hovered = _nnstate.isAnyPopupOpen ? false : nnPosRecOverlaps(mousePos.x, mousePos.y, (nnRecf){x, y, width, height});
    bool pressed = hovered && nnMouseDown(0);
    bool released = hovered && nnMouseReleased(0);

    // Determine button color based on state
    nnColorf bgColor = pressed ? _nnCurrentTheme.secondaryColorAccent
                               : (hovered ? _nnCurrentTheme.primaryColorAccent
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
        char buffer[256] = {0};
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

    nnSetColor(nnGetColor());

    return released;
}

bool nnCheckbox(const char *format, bool isChecked, int x, int y, ...)
{
    // Unique ID based on position
    unsigned int id = _nnGenUID(x, y);

    // Find or initialize checkbox state
    _nnCheckboxState *state = NULL;
    for (int i = 0; i < _nnCheckboxStateCount; i++)
    {
        if (_nnCheckboxStates[i].id == id)
        {
            state = &_nnCheckboxStates[i];
            break;
        }
    }
    if (!state)
    {
        if (_nnCheckboxStateCount >= _NN_MAX_CHECKBOX_STATES)
        {
            printf("Error: Too many checkboxes! Increase _NN_MAX_CHECKBOX_STATES.\n");
            return -1;
        }
        state = &_nnCheckboxStates[_nnCheckboxStateCount++];
        state->id = id;
        state->isChecked = isChecked;
    }

    // Dimensions for the checkbox
    int checkboxSize = 20; // Square checkbox size
    int spacing = 8;       // Space between checkbox and label
    int margin = 1;        // Margin for the filled area

    char buffer[256] = {0};
    if (format)
    {
        va_list args;
        va_start(args, y);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
    }

    // Calculate text dimensions
    float textWidth = nnTextWidth(buffer);
    float textHeight = nnTextHeight();

    // Total clickable area
    int clickableWidth = checkboxSize + spacing + (int)textWidth;
    int clickableHeight = checkboxSize > textHeight ? checkboxSize : (int)textHeight;

    // Get mouse state
    nnPos mousePos = nnMousePosition();
    bool hovered = _nnstate.isAnyPopupOpen ? false : nnPosRecOverlaps(mousePos.x, mousePos.y, (nnRecf){x, y, clickableWidth, clickableHeight});
    bool released = hovered && nnMouseReleased(0);

    // Update checkbox state
    if (released)
    {
        state->isChecked = !state->isChecked;
    }

    // Draw the checkbox
    nnColorf boxColor = hovered ? _nnCurrentTheme.secondaryColorAccent
                                : _nnCurrentTheme.secondaryColor;
    nnColorf fillColor = hovered ? _nnCurrentTheme.primaryColorAccent
                                 : _nnCurrentTheme.primaryColor;
    nnColorf borderColor = _nnCurrentTheme.borderColor;
    nnColorf textColor = _nnCurrentTheme.textPrimaryColor;

    // Background of the checkbox (base color)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glColor4f(boxColor.r, boxColor.g, boxColor.b, boxColor.a);
    glVertex2f(x, y);
    glVertex2f(x + checkboxSize, y);
    glVertex2f(x + checkboxSize, y + checkboxSize);
    glVertex2f(x, y + checkboxSize);
    glEnd();

    // Border of the checkbox
    glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + checkboxSize, y);
    glVertex2f(x + checkboxSize, y + checkboxSize);
    glVertex2f(x, y + checkboxSize);
    glEnd();

    // Fill the checkbox with a margin if checked
    if (state->isChecked)
    {
        glBegin(GL_QUADS);
        glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
        glVertex2f(x + margin, y + margin);
        glVertex2f(x + checkboxSize - margin * 2, y + margin);
        glVertex2f(x + checkboxSize - margin * 2, y + checkboxSize - margin * 2);
        glVertex2f(x + margin, y + checkboxSize - margin * 2);
        glEnd();
    }

    // Draw the label text
    int textX = x + checkboxSize + spacing;
    int textY = y + (checkboxSize - textHeight) / 2; // Vertically center text with checkbox
    glColor4f(textColor.r, textColor.g, textColor.b, textColor.a);

    glDisable(GL_BLEND);

    nnSetColor(nnGetColor());
    nnDrawText(buffer, textX, textY);

    return state->isChecked;
}

float nnHSlider(float min, float max, float initial, float step, int x, int y, int width)
{
    if (min >= max)
    {
        printf("Error: min must be less than max.\n");
        return 0;
    }

    // Unique ID based on position
    unsigned int id = _nnGenUID(x, y);

    // Find or initialize slider state
    _nnSliderState *state = NULL;
    for (int i = 0; i < _nnSliderStateCount; i++)
    {
        if (_nnSliderStates[i].id == id)
        {
            state = &_nnSliderStates[i];
            break;
        }
    }
    if (!state)
    {
        if (_nnSliderStateCount >= _NN_MAX_SLIDER_STATES)
        {
            printf("Error: Too many sliders! Increase _NN_MAX_SLIDER_STATES.\n");
            return -1;
        }
        state = &_nnSliderStates[_nnSliderStateCount++];
        state->id = id;
        state->value = initial;
    }

    float height = 16;

    // Ensure the value is clamped between min and max
    state->value = fmaxf(min, fminf(max, state->value));

    // Get mouse state
    nnPos mousePos = nnMousePosition();
    bool hovered = _nnstate.isAnyPopupOpen ? false : nnPosRecOverlaps(mousePos.x, mousePos.y, (nnRecf){x, y, width, 16});
    bool dragging = hovered && nnMouseDown(0);

    // Handle dragging
    if (dragging)
    {
        float relativeX = mousePos.x - x;
        float proportion = fmaxf(0, fminf(1, relativeX / width));
        state->value = min + proportion * (max - min);
        state->value = roundf(state->value / step) * step; // Apply step resolution
    }

    // Calculate the knob position
    float proportion = (state->value - min) / (max - min);
    int knobX = x + (int)(proportion * width);

    // Draw the slider bar
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw the filled portion
    nnColorf fillColor = hovered ? _nnCurrentTheme.primaryColorAccent : _nnCurrentTheme.primaryColor;
    glBegin(GL_QUADS);
    glColor4f(fillColor.r, fillColor.g,
              fillColor.b, fillColor.a);
    glVertex2f(x, y);
    glVertex2f(knobX, y);
    glVertex2f(knobX, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw the unfilled portion
    nnColorf unfilledColor = hovered ? _nnCurrentTheme.secondaryColorAccent : _nnCurrentTheme.secondaryColor;
    glBegin(GL_QUADS);
    glColor4f(unfilledColor.r, unfilledColor.g,
              unfilledColor.b, unfilledColor.a);
    glVertex2f(knobX, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(knobX, y + height);
    glEnd();

    // Draw the border
    nnColorf borderColor = _nnCurrentTheme.borderColor;
    glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw the knob
    nnColorf knobColor = hovered ? _nnCurrentTheme.borderColorAccent : _nnCurrentTheme.borderColor;
    glBegin(GL_QUADS);
    glColor4f(knobColor.r, knobColor.g,
              knobColor.b, knobColor.a);
    glVertex2f(knobX - 4, y - 2);
    glVertex2f(knobX + 4, y - 2);
    glVertex2f(knobX + 4, y + height + 2);
    glVertex2f(knobX - 4, y + height + 2);
    glEnd();

    glDisable(GL_BLEND);

    nnSetColor(nnGetColor());

    return state->value;
}

float nnVSlider(float min, float max, float initial, float step, int x, int y, int height)
{
    if (min >= max)
    {
        printf("Error: min must be less than max.\n");
        return 0;
    }

    // Unique ID based on position
    unsigned int id = _nnGenUID(x, y);

    // Find or initialize slider state
    _nnSliderState *state = NULL;
    for (int i = 0; i < _nnSliderStateCount; i++)
    {
        if (_nnSliderStates[i].id == id)
        {
            state = &_nnSliderStates[i];
            break;
        }
    }
    if (!state)
    {
        if (_nnSliderStateCount >= _NN_MAX_SLIDER_STATES)
        {
            printf("Error: Too many sliders! Increase _NN_MAX_SLIDER_STATES.\n");
            return -1;
        }
        state = &_nnSliderStates[_nnSliderStateCount++];
        state->id = id;
        state->value = initial;
    }

    float width = 16;

    // Ensure the value is clamped between min and max
    state->value = fmaxf(min, fminf(max, state->value));

    // Get mouse state
    nnPos mousePos = nnMousePosition();
    bool hovered = _nnstate.isAnyPopupOpen ? false : nnPosRecOverlaps(mousePos.x, mousePos.y, (nnRecf){x, y, 16, height});
    bool dragging = hovered && nnMouseDown(0);

    // Handle dragging
    if (dragging)
    {
        float relativeY = mousePos.y - y;
        float proportion = fmaxf(0, fminf(1, 1.0f - (relativeY / height)));
        state->value = min + proportion * (max - min);
        state->value = roundf(state->value / step) * step; // Apply step resolution
    }

    // Calculate the knob position
    float proportion = (state->value - min) / (max - min);
    int knobY = y + height - (int)(proportion * height);

    // Draw the slider bar
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw the filled portion
    nnColorf fillColor = hovered ? _nnCurrentTheme.primaryColorAccent : _nnCurrentTheme.primaryColor;
    glBegin(GL_QUADS);
    glColor4f(fillColor.r, fillColor.g,
              fillColor.b, fillColor.a);
    glVertex2f(x, y + height);
    glVertex2f(x + width, y + height);
    glVertex2f(x + width, knobY);
    glVertex2f(x, knobY);
    glEnd();

    // Draw the unfilled portion
    nnColorf unfilledColor = hovered ? _nnCurrentTheme.secondaryColorAccent : _nnCurrentTheme.secondaryColor;
    glBegin(GL_QUADS);
    glColor4f(unfilledColor.r, unfilledColor.g,
              unfilledColor.b, unfilledColor.a);
    glVertex2f(x, knobY);
    glVertex2f(x + width, knobY);
    glVertex2f(x + width, y);
    glVertex2f(x, y);
    glEnd();

    // Draw the border
    nnColorf borderColor = _nnCurrentTheme.borderColor;
    glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw the knob
    nnColorf knobColor = hovered ? _nnCurrentTheme.borderColorAccent : _nnCurrentTheme.borderColor;
    glBegin(GL_QUADS);
    glColor4f(knobColor.r, knobColor.g,
              knobColor.b, knobColor.a);
    glVertex2f(x - 2, knobY - 4);
    glVertex2f(x + width + 2, knobY - 4);
    glVertex2f(x + width + 2, knobY + 4);
    glVertex2f(x - 2, knobY + 4);
    glEnd();

    glDisable(GL_BLEND);

    nnSetColor(nnGetColor());
    return state->value;
}

int nnHProgressbar(float min, float max, float deltaFillState, int x, int y, int width)
{
    if (min >= max)
    {
        printf("Error: min must be less than max.\n");
        return 0;
    }

    // Unique ID based on position
    unsigned int id = _nnGenUID(x, y);

    // Find or initialize progressbar state
    _nnProgressbarState *state = NULL;
    for (int i = 0; i < _nnProgressbarStateCount; i++)
    {
        if (_nnProgressbarStates[i].id == id)
        {
            state = &_nnProgressbarStates[i];
            break;
        }
    }
    if (!state)
    {
        if (_nnProgressbarStateCount >= _NN_MAX_PROGRESSBAR_STATES)
        {
            printf("Error: Too many progressbars! Increase _NN_MAX_PROGRESSBAR_STATES.\n");
            return -1;
        }
        state = &_nnProgressbarStates[_nnProgressbarStateCount++];
        state->id = id;
        state->fillState = 0.0;
    }

    // Adjust the fillState by the delta provided
    state->fillState += deltaFillState;

    // Clamp the internal fillState within the range [min, max]
    if (state->fillState < min)
        state->fillState = min;
    if (state->fillState > max)
        state->fillState = max;

    // Calculate the percentage fill state
    float range = max - min;
    float percentage = (state->fillState - min) / range;
    int filledWidth = (int)(percentage * width);

    // Draw the progress bar background (secondary color)
    nnColorf bgColor = _nnCurrentTheme.secondaryColor;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glColor4f(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + 20);
    glVertex2f(x, y + 20);
    glEnd();

    // Draw the filled portion (primary color)
    nnColorf fillColor = _nnCurrentTheme.primaryColor;
    glBegin(GL_QUADS);
    glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
    glVertex2f(x, y);
    glVertex2f(x + filledWidth, y);
    glVertex2f(x + filledWidth, y + 20);
    glVertex2f(x, y + 20);
    glEnd();

    // Draw the border
    nnColorf borderColor = _nnCurrentTheme.borderColor;
    glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + 20);
    glVertex2f(x, y + 20);
    glEnd();

    glDisable(GL_BLEND);

    nnSetColor(nnGetColor());

    return (int)(percentage * 100);
}

int nnVProgressbar(float min, float max, float deltaFillState, int x, int y, int height)
{
    if (min >= max)
    {
        printf("Error: min must be less than max.\n");
        return 0;
    }

    // Unique ID based on position
    unsigned int id = _nnGenUID(x, y);

    // Find or initialize progressbar state
    _nnProgressbarState *state = NULL;
    for (int i = 0; i < _nnProgressbarStateCount; i++)
    {
        if (_nnProgressbarStates[i].id == id)
        {
            state = &_nnProgressbarStates[i];
            break;
        }
    }
    if (!state)
    {
        if (_nnProgressbarStateCount >= _NN_MAX_PROGRESSBAR_STATES)
        {
            printf("Error: Too many progressbars! Increase _NN_MAX_PROGRESSBAR_STATES.\n");
            return -1;
        }
        state = &_nnProgressbarStates[_nnProgressbarStateCount++];
        state->id = id;
        state->fillState = 0.0;
    }

    // Adjust the fillState by the delta provided
    state->fillState += deltaFillState;

    // Clamp the internal fillState within the range [min, max]
    if (state->fillState < min)
        state->fillState = min;
    if (state->fillState > max)
        state->fillState = max;

    // Calculate the percentage fill state
    float range = max - min;
    float percentage = (state->fillState - min) / range;
    int filledHeight = (int)(percentage * height);

    // Draw the progress bar background (secondary color)
    nnColorf bgColor = _nnCurrentTheme.secondaryColor;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin(GL_QUADS);
    glColor4f(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glVertex2f(x, y);
    glVertex2f(x + 20, y);
    glVertex2f(x + 20, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw the filled portion (primary color)
    nnColorf fillColor = _nnCurrentTheme.primaryColor;
    glBegin(GL_QUADS);
    glColor4f(fillColor.r, fillColor.g, fillColor.b, fillColor.a);
    glVertex2f(x, y + height - filledHeight);
    glVertex2f(x + 20, y + height - filledHeight);
    glVertex2f(x + 20, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw the border
    nnColorf borderColor = _nnCurrentTheme.borderColor;
    glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + 20, y);
    glVertex2f(x + 20, y + height);
    glVertex2f(x, y + height);
    glEnd();

    glDisable(GL_BLEND);

    nnSetColor(nnGetColor());

    return (int)(percentage * 100);
}

int nnDropdown(const char *buttonText, const char **options, int numOptions, int x, int y, int width, int height)
{
    // Unique ID based on position
    unsigned int id = _nnGenUID(x, y);

    // Find or initialize dropdown state
    _nnDropdownState *state = NULL;
    for (int i = 0; i < _nnDropdownStateCount; i++)
    {
        if (_nnDropdownStates[i].id == id)
        {
            state = &_nnDropdownStates[i];
            break;
        }
    }
    if (!state)
    {
        if (_nnDropdownStateCount >= _NN_MAX_DROPDOWN_STATES)
        {
            printf("Error: Too many dropdowns! Increase _NN_MAX_DROPDOWN_STATES.\n");
            return -1;
        }
        state = &_nnDropdownStates[_nnDropdownStateCount++];
        state->id = id;
        state->isOpen = false;
        state->selectedIndex = -1;
        state->scrollOffset = 0;
        state->initialized = false;
    }

    const int maxVisibleOptions = 10;
    // const int optionHeight = nnTextHeight() + 12;

    // Initialize the button text with the first option if not initialized
    if (!state->initialized)
    {
        if (buttonText)
        {
            strncpy(state->selectedText, buttonText, sizeof(state->selectedText) - 1);
        }
        else if (numOptions > 0 && options[0])
        {
            strncpy(state->selectedText, options[0], sizeof(state->selectedText) - 1);
        }
        state->initialized = true;
    }

    // Get mouse position
    nnPos mousePos = nnMousePosition();

    // Determine if the mouse is hovering over the dropdown button
    bool hoveringButton = nnPosRecOverlaps(mousePos.x, mousePos.y, (nnRecf){x, y, width, height});

    // Calculate the actual height of the visible list
    int actualVisibleOptions = (numOptions < maxVisibleOptions) ? numOptions : maxVisibleOptions;
    int visibleListHeight = actualVisibleOptions * height;

    // Check if the list fits below or above the button
    int screenHeight = nnScreenHeight();
    bool drawAbove = (y - height / 2) > screenHeight / 2;

    // Adjust Y position for the list if drawing above
    int listY = drawAbove ? (y - visibleListHeight) : (y + height);

    // Toggle dropdown open/close on button click
    if (hoveringButton && nnMouseReleased(0))
    {
        state->isOpen = !state->isOpen;
        // Other elements can use this to determine of they can be clicked.
        // If a popup is open, other elements behind should not be clickable.
        if (state->isOpen)
        {
            _nnstate.isAnyPopupOpen = true;
        }
        else
        {
            _nnstate.isAnyPopupOpen = false;
        }
    }

    // Close dropdown if mouse is not hovering over button or list
    bool hoveringList = false;
    if (state->isOpen)
    {
        int visibleListHeight = maxVisibleOptions * height;
        hoveringList = nnPosRecOverlaps(mousePos.x, mousePos.y, (nnRecf){x, drawAbove ? listY : y, width, drawAbove ? visibleListHeight : visibleListHeight + height});
    }
    if (!hoveringButton && !hoveringList && nnMouseReleased(0))
    {
        state->isOpen = false;
    }

    // Handle scrolling using nnMouseWheelDelta
    if (state->isOpen && hoveringList)
    {
        int wheelDelta = nnMouseWheelDelta();
        if (drawAbove)
        {
            // Reverse the scroll direction for above layout
            if (wheelDelta > 0)
            {
                state->scrollOffset = (state->scrollOffset < numOptions - maxVisibleOptions) ? state->scrollOffset + 1 : state->scrollOffset;
            }
            else if (wheelDelta < 0)
            {
                state->scrollOffset = (state->scrollOffset > 0) ? state->scrollOffset - 1 : 0;
            }
        }
        else
        {
            // Default scroll logic for below layout
            if (wheelDelta > 0)
            {
                state->scrollOffset = (state->scrollOffset > 0) ? state->scrollOffset - 1 : 0;
            }
            else if (wheelDelta < 0)
            {
                state->scrollOffset = (state->scrollOffset < numOptions - maxVisibleOptions) ? state->scrollOffset + 1 : state->scrollOffset;
            }
        }
    }

    // Draw dropdown button
    nnColorf bgColor = state->isOpen ? _nnCurrentTheme.primaryColorAccent
                                     : (hoveringButton ? _nnCurrentTheme.primaryColorAccent : _nnCurrentTheme.primaryColor);
    nnColorf textColor = _nnCurrentTheme.textPrimaryColor;
    nnColorf borderColor = _nnCurrentTheme.borderColor;

    // Draw button background
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

    // Draw triangle indicator (direction depends on list position: above or below)
    int triangleSize = height / 4;
    int triangleX = x + width - triangleSize * 2; // Position triangle on the right side
    int triangleY = drawAbove ? (y + height / 2) + triangleSize / 2 : (y + height / 2) - triangleSize / 2;

    glBegin(GL_TRIANGLES);
    glColor4f(textColor.r, textColor.g, textColor.b, textColor.a);
    if (drawAbove)
    {
        glVertex2f(triangleX, triangleY);
        glVertex2f(triangleX + triangleSize, triangleY);
        glVertex2f(triangleX + triangleSize / 2, triangleY - triangleSize);
    }
    else
    {
        glVertex2f(triangleX, triangleY);
        glVertex2f(triangleX + triangleSize, triangleY);
        glVertex2f(triangleX + triangleSize / 2, triangleY + triangleSize);
    }
    glEnd();

    // Draw button text (truncate if necessary)
    char truncatedText[256];
    strncpy(truncatedText, state->selectedText, sizeof(truncatedText) - 1);
    truncatedText[sizeof(truncatedText) - 1] = '\0';

    float textWidth = nnTextWidth(truncatedText);
    if (textWidth > width - triangleSize * 3)
    {
        for (int i = strlen(truncatedText) - 1; i > 0; i--)
        {
            truncatedText[i] = '\0';
            textWidth = nnTextWidth(truncatedText);
            if (textWidth <= width - triangleSize * 3 - nnTextWidth("..."))
            {
                strcat(truncatedText, "...");
                break;
            }
        }
    }

    int textX = x + 8; // Left-align with a margin of 8 pixels
    int textY = y + (height - nnTextHeight()) / 2;
    glColor4f(textColor.r, textColor.g, textColor.b, textColor.a);
    nnDrawText(truncatedText, textX, textY);

    // Draw dropdown list if open
    if (state->isOpen)
    {
        // Draw the dropdown list options
        for (int i = 0; i < numOptions; i++)
        {
            // Calculate the visible range of options considering the scroll offset
            int visibleStart = state->scrollOffset;
            int visibleEnd = state->scrollOffset + actualVisibleOptions;
            if (i < visibleStart || i >= visibleEnd)
                continue;

            // Calculate option position
            int visibleIndex = i - state->scrollOffset;
            int optionY = drawAbove
                              ? (listY + (actualVisibleOptions - 1 - visibleIndex) * height) // Adjust for drawing above
                              : (listY + visibleIndex * height);                             // Default when drawing below

            // Determine if mouse is hovering over the option
            bool hoveringOption = nnPosRecOverlaps(mousePos.x, mousePos.y, (nnRecf){x, optionY, width, height});

            // Background color for the option
            nnColorf optionBgColor = hoveringOption ? _nnCurrentTheme.secondaryColorAccent : _nnCurrentTheme.secondaryColor;

            // Draw option popup background
            glBegin(GL_QUADS);
            glColor4f(optionBgColor.r, optionBgColor.g, optionBgColor.b, optionBgColor.a);
            glVertex3f(x, optionY, _NN_Z_INDEX_POPUP);
            glVertex3f(x + width, optionY, _NN_Z_INDEX_POPUP);
            glVertex3f(x + width, optionY + height, _NN_Z_INDEX_POPUP);
            glVertex3f(x, optionY + height, _NN_Z_INDEX_POPUP);
            glEnd();

            // Draw option popup border
            glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
            glBegin(GL_LINE_LOOP);
            glVertex3f(x, optionY, _NN_Z_INDEX_POPUP);
            glVertex3f(x + width, optionY, _NN_Z_INDEX_POPUP);
            glVertex3f(x + width, optionY + height, _NN_Z_INDEX_POPUP);
            glVertex3f(x, optionY + height, _NN_Z_INDEX_POPUP);
            glEnd();

            // Draw option text (truncate if necessary)
            char optionText[256];
            strncpy(optionText, options[i], sizeof(optionText) - 1);
            optionText[sizeof(optionText) - 1] = '\0';

            float optionTextWidth = nnTextWidth(optionText);
            float rightMargin = 20;
            if (optionTextWidth > width - rightMargin)
            {
                for (int j = strlen(optionText) - 1; j > 0; j--)
                {
                    optionText[j] = '\0';
                    optionTextWidth = nnTextWidth(optionText);
                    if (optionTextWidth <= width - rightMargin - nnTextWidth("..."))
                    {
                        strcat(optionText, "...");
                        break;
                    }
                }
            }

            int optionTextX = x + 10; // Left-aligned with a margin of 10 pixels
            int optionTextY = optionY + (height - nnTextHeight()) / 2;
            glColor4f(textColor.r, textColor.g, textColor.b, textColor.a);
            nnDrawTextZ(optionText, optionTextX, optionTextY, _NN_Z_INDEX_POPUP_TEXT);

            // Handle option click
            if (hoveringOption && nnMouseReleased(0))
            {
                state->selectedIndex = i;
                strncpy(state->selectedText, options[i], sizeof(state->selectedText) - 1);
                state->isOpen = false;
                nnFlushMouse();
            }
        }

        // Draw scrollbar if the list is scrollable
        if (numOptions > maxVisibleOptions)
        {
            float scrollbarHeight = (float)actualVisibleOptions / numOptions * visibleListHeight;
            float scrollbarPosition = (float)state->scrollOffset / numOptions * visibleListHeight;
            int scrollbarX = x + width - 2; // 4px wide scrollbar at the right edge
            int scrollbarY = drawAbove
                                 ? (listY + visibleListHeight - scrollbarHeight - scrollbarPosition)
                                 : (listY + scrollbarPosition);

            // Draw scrollbar background
            // nnColorf scrollbarBgColor = _nnCurrentTheme.secondaryColor; // Background color
            // glBegin(GL_QUADS);
            // glColor4f(scrollbarBgColor.r, scrollbarBgColor.g, scrollbarBgColor.b, scrollbarBgColor.a);
            // glVertex2f(scrollbarX, listY);
            // glVertex2f(scrollbarX + 2, listY);
            // glVertex2f(scrollbarX + 2, listY + visibleListHeight);
            // glVertex2f(scrollbarX, listY + visibleListHeight);
            // glEnd();

            // Draw scrollbar foreground
            nnColorf scrollbarFgColor = _nnCurrentTheme.primaryColor; // Scrollbar color
            glBegin(GL_QUADS);
            glColor4f(scrollbarFgColor.r, scrollbarFgColor.g, scrollbarFgColor.b, scrollbarFgColor.a);
            glVertex3f(scrollbarX, scrollbarY, _NN_Z_INDEX_POPUP);
            glVertex3f(scrollbarX + 2, scrollbarY, _NN_Z_INDEX_POPUP);
            glVertex3f(scrollbarX + 2, scrollbarY + scrollbarHeight, _NN_Z_INDEX_POPUP);
            glVertex3f(scrollbarX, scrollbarY + scrollbarHeight, _NN_Z_INDEX_POPUP);
            glEnd();
        }
    }

    glDisable(GL_BLEND);

    nnSetColor(nnGetColor());

    return state->selectedIndex;
}

int nnScrollableList(const char **items, int numItems, int x, int y, int width, int height)
{
    // Unique ID based on position
    unsigned int id = _nnGenUID(x, y);

    // Find or initialize dropdown state
    _nnScrollableListState *state = NULL;
    for (int i = 0; i < _nnScrollableListStateCount; i++)
    {
        if (_nnScrollableListStates[i].id == id)
        {
            state = &_nnScrollableListStates[i];
            break;
        }
    }
    if (!state)
    {
        if (_nnScrollableListStateCount >= _NN_MAX_SCROLLABLELIST_STATES)
        {
            printf("Error: Too many scrollable lists! Increase _NN_MAX_SCROLLABLELIST_STATES.\n");
            return -1;
        }
        state = &_nnScrollableListStates[_nnScrollableListStateCount++];
        state->id = id;
        state->selectedIndex = -1;
        state->scrollOffset = 0;
    }

    const int itemHeight = nnTextHeight() + 14; // Fixed height for each item
    int maxVisibleItems = height / itemHeight;  // Calculate how many items can be displayed

    // Ensure the scroll offset and maxVisibleItems are valid
    if (maxVisibleItems > numItems)
        maxVisibleItems = numItems; // No need to scroll if all items fit
    if (state->scrollOffset > numItems - maxVisibleItems)
        state->scrollOffset = numItems - maxVisibleItems;

    // Get mouse position
    nnPos mousePos = nnMousePosition();

    // Draw the background of the scrollable list
    nnColorf bgColor = _nnCurrentTheme.secondaryColor;
    nnColorf borderColor = _nnCurrentTheme.borderColor;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw container background
    glBegin(GL_QUADS);
    glColor4f(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw container border
    glColor4f(borderColor.r, borderColor.g, borderColor.b, borderColor.a);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Draw visible items
    for (int i = 0; i < maxVisibleItems && (i + state->scrollOffset) < numItems; i++)
    {
        int itemIndex = i + state->scrollOffset;
        int itemY = y + i * itemHeight;

        // Check if the mouse is hovering over the item
        bool hovering = nnPosRecOverlaps(mousePos.x, mousePos.y, (nnRecf){x, itemY, width, itemHeight});

        // Determine item background color
        nnColorf itemBgColor;
        if (itemIndex == state->selectedIndex)
        {
            itemBgColor = _nnCurrentTheme.primaryColor; // Selected item color
        }
        else
        {
            itemBgColor = hovering ? _nnCurrentTheme.secondaryColorAccent : _nnCurrentTheme.secondaryColor;
        }

        // Draw item background
        glBegin(GL_QUADS);
        glColor4f(itemBgColor.r, itemBgColor.g, itemBgColor.b, itemBgColor.a);
        glVertex2f(x, itemY);
        glVertex2f(x + width - 1, itemY);
        glVertex2f(x + width - 1, itemY + itemHeight);
        glVertex2f(x, itemY + itemHeight);
        glEnd();

        // Draw item text (truncate if necessary)
        char truncatedText[256];
        strncpy(truncatedText, items[itemIndex], sizeof(truncatedText) - 1);
        truncatedText[sizeof(truncatedText) - 1] = '\0';

        float textWidth = nnTextWidth(truncatedText);
        if (textWidth > width - 16) // Leave some padding
        {
            for (int j = strlen(truncatedText) - 1; j > 0; j--)
            {
                truncatedText[j] = '\0';
                textWidth = nnTextWidth(truncatedText);
                if (textWidth <= width - 16 - nnTextWidth("..."))
                {
                    strcat(truncatedText, "...");
                    break;
                }
            }
        }

        int textX = x + 8; // Left-align with a margin of 8 pixels
        int textY = itemY + (itemHeight - nnTextHeight()) / 2;
        nnColorf textColor = _nnCurrentTheme.textPrimaryColor;
        glColor4f(textColor.r, textColor.g, textColor.b, textColor.a);
        nnDrawText(truncatedText, textX, textY);

        // Handle item click
        if (hovering && nnMouseReleased(0))
        {
            state->selectedIndex = itemIndex;
        }
    }

    // Handle scrolling with mouse wheel
    bool hovering = nnPosRecOverlaps(mousePos.x, mousePos.y, (nnRecf){x, y, width, height});
    int wheelDelta = nnMouseWheelDelta();
    if (hovering && numItems > maxVisibleItems)
    {
        if (wheelDelta > 0)
        {
            state->scrollOffset = (state->scrollOffset > 0) ? state->scrollOffset - 1 : 0;
        }
        else if (wheelDelta < 0)
        {
            state->scrollOffset = (state->scrollOffset < numItems - maxVisibleItems) ? state->scrollOffset + 1 : state->scrollOffset;
        }
    }

    // Draw scrollbar if necessary
    if (numItems > maxVisibleItems)
    {
        float scrollbarHeight = (float)maxVisibleItems / numItems * height;
        float scrollbarPosition = (float)state->scrollOffset / numItems * height;
        int scrollbarX = x + width - 3; // 4px wide scrollbar at the right edge
        int scrollbarY = y + scrollbarPosition;

        // Draw scrollbar
        nnColorf scrollbarColor = _nnCurrentTheme.primaryColor;
        glBegin(GL_QUADS);
        glColor4f(scrollbarColor.r, scrollbarColor.g, scrollbarColor.b, scrollbarColor.a);
        glVertex2f(scrollbarX, scrollbarY);
        glVertex2f(scrollbarX + 3, scrollbarY);
        glVertex2f(scrollbarX + 3, scrollbarY + scrollbarHeight);
        glVertex2f(scrollbarX, scrollbarY + scrollbarHeight);
        glEnd();
    }

    glDisable(GL_BLEND);

    nnSetColor(nnGetColor());

    return state->selectedIndex;
}

#endif // NONOGL_IMPLEMENTATION