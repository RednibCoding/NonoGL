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

typedef struct
{
    int x;
    int y;
} nnVec2;

typedef struct
{
    float x;
    float y;
    float width;
    float height;
} nnRecf;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} nnColorf;

typedef struct
{
    unsigned int textureID;
    int width;
    int height;
} nnImage;

typedef struct
{
    unsigned int textureID; // OpenGL texture ID
    int width;              // Width of the buffer
    int height;             // Height of the buffer
    nnColorf *pixels;       // CPU-side pixel buffer
} nnPixmap;

// Represents a font loaded with stb_truetype
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

int nnFPS;
float nnDT;

// Create a window
bool nnCreateWindow(char *title, int width, int height, bool scalable, bool filtered);

// Set the display/render callback function
void nnSetDisplayFunc(void (*callback)(void));

// Release resources and free memory
void nnDestroyWindow();

// Set the window title
void nnSetWindowTitle(const char *format, ...);

// Set the clear color
void nnSetClearColor(nnColorf color);

// Set the draw color
void nnSetColor(nnColorf color);

// Resets the color to default (1.0, 1.0, 1.0)
void nnResetColor();

// Clear the screen
void nnCls();

// End rendering and swap buffers
void nnFlip();

// Load a file as bytes into a buffer and return the pointer to that buffer
unsigned char *nnLoadFileBytes(const char *filepath, int *size);

// Free the buffer allocated by nnLoadFileBytes
void nnFreeFileBytes(unsigned char *buffer);

// Load an image from file
nnImage nnLoadImage(const char *filepath);

// Load an image from memory
nnImage nnLoadImageMem(const unsigned char *data, int size);

// Draw an image on the screen
void nnDrawImage(nnImage image, int x, int y);

// Draw a portion of an image
void nnDrawImagePortion(nnImage image, int x, int y, nnRecf srcRec);

// Frees the given image
void nnFreeImage(nnImage image);

// Create a pixel buffer with the given width and height
nnPixmap *nnCreatePixmap(int width, int height);

// Creates a Pixmap from an image
nnPixmap *nnCreatePixmapFromImage(nnImage image);

// Write a pixel at x, y location with the given color to the given pixel buffer
void nnPutPixel(nnPixmap *buffer, int x, int y, nnColorf color);

// Read a pixel from the given pixmap.
nnColorf nnReadPixel(nnPixmap *pixmap, int x, int y);

// Update pixels that have changed in the Pixmap
void nnUpdatePixmap(nnPixmap *buffer);

// Draw the Pixmap to the screen
void nnDrawPixmap(nnPixmap *pixmap, int x, int y);

// Free the given pixel buffer
void nnFreePixmap(nnPixmap *buffer);

// Draw an individual pixel to the screen (use a pixel buffer for large chunks of pixels instead)
void drawPixel(float x, float y);

// Load a font from a file
nnFont *nnLoadFont(const char *filepath, float fontSize);

// Set font for text rendering, if not set, a default non-scalable font will be used
void nnSetFont(nnFont *font);

// Render text using the font
void nnDrawText(const char *format, int x, int y, ...);

// Free the font
void nnFreeFont(nnFont *font);

// Check if a point overlaps with a rectangle
bool nnVec2RecOverlaps(int x, int y, nnRecf rect);

// Check if two rectangles overlap
bool nnRecsOverlap(nnRecf rec1, nnRecf rec2);

// Check if a point overlaps with a circle
bool nnVec2CircleOverlaps(int x, int y, int cx, int cy, float circleradius);

// Check if a rectangle overlaps with a circle
bool nnRecCircleOverlaps(nnRecf rec, int cx, int cy, float circleradius);

// Check if two cirles overlap
bool nnCirclesOverlaps(int cx1, int cy1, float circle1radius, int cx2, int cy2, float circle2radius);

// Returns `true` if the specified key was pressed since the last frame.
bool nnKeyHit(int key);

// Returns `true` if the specified key is currently being held down.
bool nnKeyDown(int key);

// Returns `true` if the specified key was released since the last frame.
bool nnKeyReleased(int key);

// Returns `true` if the specified mouse button was clicked since the last frame.
bool nnMouseHit(int button);

// Returns `true` if the specified mouse button is currently being held down.
bool nnMouseDown(int button);

// Returns `true` if the specified mouse button was released since the last frame.
bool nnMouseReleased(int button);

// Returns the direction of the mouse wheel movement: `1` for up, `-1` for down, and `0` if no movement occurred.
int nnMouseWheelDelta();

// Returns the current position of the mouse cursor as an `nnVec2` struct.
nnVec2 nnGetMousePosition();

// Returns the mouse motion delta (change in position) since the last frame.
nnVec2 nnMouseMotionDelta();

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
    int window;
    nnFont *font;

    // Keyboard state
    bool keys[_NN_MAX_KEYS];
    bool keysPressed[_NN_MAX_KEYS];
    bool keysReleased[_NN_MAX_KEYS];

    // Mouse state
    bool mouseButtons[_NN_MAX_MOUSE_BUTTONS];
    bool mouseButtonsPressed[_NN_MAX_MOUSE_BUTTONS];
    bool mouseButtonsReleased[_NN_MAX_MOUSE_BUTTONS];
    int mouseWheelDelta;
    nnVec2 mousePosition;
    nnVec2 mouseMotionDelta;

    // Function pointer for the display callback
    void (*displayCallback)(void);

} _nnState;

_nnState _nnstate;

// Define custom exit handlers
void _customExitFunction(int status)
{
    exit(status);
}

// Wrapper display function that calls the function pointer
void _nnDisplayCallbackWrapper()
{
    if (_nnstate.displayCallback != NULL)
    {
        _nnstate.displayCallback();
    }
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
    // Calculate deltaTime and currentFPS
    double currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0; // Get time in seconds
    _nnstate.deltaTime = currentTime - _nnstate.lastTime;
    _nnstate.currentFPS = (_nnstate.deltaTime > 0) ? (1.0 / _nnstate.deltaTime) : 0;
    _nnstate.lastTime = currentTime;

    nnFPS = _nnstate.currentFPS;
    nnDT = _nnstate.deltaTime;

    // Trigger display refresh
    glutPostRedisplay();

    // Re-register the timer callback for 60 FPS
    glutTimerFunc(1000 / 60, _nnTimerCallback, 0);
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
    nnVec2 newMousePosition = {(float)x, (float)y};
    _nnstate.mouseMotionDelta.x = newMousePosition.x - _nnstate.mousePosition.x;
    _nnstate.mouseMotionDelta.y = newMousePosition.y - _nnstate.mousePosition.y;
    _nnstate.mousePosition = newMousePosition;
}

void _nnDrawTextDefaultFont(const char *format, int x, int y, ...)
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

/*******************************************************************************************************/
// Public api function implementations
/*******************************************************************************************************/

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

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    int argc = 1;
    char *argv[1] = {(char *)"Something"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    _nnstate.window = glutCreateWindow(title);

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
    glutTimerFunc(1000 / 60, _nnTimerCallback, 0);

    return true;
}

void nnDestroyWindow()
{
    glutDestroyWindow(_nnstate.window);
}

void nnSetClearColor(nnColorf color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void nnSetColor(nnColorf color)
{
    glColor4f(color.r, color.g, color.b, color.a);
}

void nnResetColor()
{
    glColor4f(1.0, 1.0, 1.0, 1.0);
}

void nnSetDisplayFunc(void (*callback)(void))
{
    _nnstate.displayCallback = callback;
    glutDisplayFunc(_nnDisplayCallbackWrapper);
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

void nnCls()
{
    glClear(GL_COLOR_BUFFER_BIT);
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

void nnFlip()
{
    glutSwapBuffers();
    glutMainLoopEvent();
}

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

nnImage nnLoadImage(const char *filepath)
{
    nnImage image;
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
    if (!image.textureID == 0)
        return;

    glBindTexture(GL_TEXTURE_2D, image.textureID);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + image.width, y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + image.width, y + image.height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + image.height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void nnDrawImagePortion(nnImage image, int x, int y, nnRecf srcRec)
{
    glBindTexture(GL_TEXTURE_2D, image.textureID);
    glEnable(GL_TEXTURE_2D);

    float texLeft = srcRec.x / image.width;
    float texRight = (srcRec.x + srcRec.width) / image.width;
    float texTop = srcRec.y / image.height;
    float texBottom = (srcRec.y + srcRec.height) / image.height;

    glBegin(GL_QUADS);
    glTexCoord2f(texLeft, texTop);
    glVertex2f(x, y);
    glTexCoord2f(texRight, texTop);
    glVertex2f(x + srcRec.width, y);
    glTexCoord2f(texRight, texBottom);
    glVertex2f(x + srcRec.width, y + srcRec.height);
    glTexCoord2f(texLeft, texBottom);
    glVertex2f(x, y + srcRec.height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void nnFreeImage(nnImage image)
{
    if (!image.textureID == 0)
        return;

    glDeleteTextures(1, &image.textureID);
}

nnPixmap *nnCreatePixmap(int width, int height)
{
    nnPixmap *buffer = malloc(sizeof(nnPixmap));
    if (!buffer)
        return NULL;

    buffer->width = width;
    buffer->height = height;

    // Allocate pixel memory
    buffer->pixels = calloc(width * height, sizeof(nnColorf));
    if (!buffer->pixels)
    {
        free(buffer);
        return NULL;
    }

    // Create OpenGL texture
    glGenTextures(1, &buffer->textureID);
    glBindTexture(GL_TEXTURE_2D, buffer->textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, buffer->pixels);

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

    return buffer;
}

nnPixmap *nnCreatePixmapFromImage(nnImage image)
{
    if (image.textureID == 0 || image.width <= 0 || image.height <= 0)
    {
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
            nnPutPixel(pixmap, x, y, color);
        }
    }

    // Upload the pixel data to the pixmap's GPU texture
    nnUpdatePixmap(pixmap);

    // Free the temporary pixel data buffer
    free(pixelData);

    return pixmap;
}

void nnPutPixel(nnPixmap *buffer, int x, int y, nnColorf color)
{
    if (!buffer || x < 0 || y < 0 || x >= buffer->width || y >= buffer->height)
        return;

    buffer->pixels[y * buffer->width + x] = color;
}

nnColorf nnReadPixel(nnPixmap *pixmap, int x, int y)
{
    if (!pixmap || x < 0 || y < 0 || x >= pixmap->width || y >= pixmap->height)
    {
        return (nnColorf){0, 0, 0, 0}; // Return transparent color for out-of-bounds or invalid pixmap
    }

    return pixmap->pixels[y * pixmap->width + x];
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
    if (pixmap->textureID == 0)
    {
        printf("Failed to draw pixmap\n");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, pixmap->textureID);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + pixmap->width, y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + pixmap->width, y + pixmap->height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + pixmap->height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
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

void drawPixel(float x, float y)
{
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

nnFont *nnLoadFont(const char *filepath, float fontSize)
{
    nnFont *font = malloc(sizeof(nnFont));
    if (!font)
        return NULL;

    // Load the font file into memory
    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        printf("Failed to open font file: %s\n", filepath);
        free(font);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    font->fontBuffer = malloc(size);
    if (!font->fontBuffer)
    {
        printf("Failed to allocate memory for font buffer.\n");
        fclose(file);
        free(font);
        return NULL;
    }
    fread(font->fontBuffer, 1, size, file);
    fclose(file);

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

void nnSetFont(nnFont *font)
{
    if (!font)
        return;

    _nnstate.font = font;
}

void nnDrawText(const char *format, int x, int y, ...)
{
    if (!format)
        return;

    if (!_nnstate.font)
    {
        _nnDrawTextDefaultFont(format, x, y);
        return;
    }

    char buffer[1024];
    va_list args;
    va_start(args, y);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _nnstate.font->textureID);

    glBegin(GL_QUADS);

    float tx = (float)x;
    float ty = (float)y;

    for (char *c = buffer; *c != '\0'; c++)
    {
        if (*c < 32 || *c >= 128)
            continue;

        stbtt_aligned_quad quad;
        stbtt_GetBakedQuad(_nnstate.font->charData, _nnstate.font->atlasWidth, _nnstate.font->atlasHeight, *c - 32, &tx, &ty, &quad, 1);

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

bool nnVec2RecOverlaps(int x, int y, nnRecf rec)
{
    return (x >= rec.x && x <= rec.x + rec.width && y >= rec.y && y <= rec.y + rec.height) ? true : false;
}

bool nnRecsOverlap(nnRecf rec1, nnRecf rec2)
{
    return (rec1.x < rec2.x + rec2.width && rec1.x + rec1.width > rec2.x && rec1.y < rec2.y + rec2.height && rec1.y + rec1.height > rec2.y) ? true : false;
}

bool nnVec2CircleOverlaps(int x, int y, int cx, int cy, float circleRadius)
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

nnVec2 nnGetMousePosition()
{
    return _nnstate.mousePosition;
}

nnVec2 nnMouseMotionDelta()
{
    nnVec2 delta = _nnstate.mouseMotionDelta;
    _nnstate.mouseMotionDelta.x = 0; // Reset after checking
    _nnstate.mouseMotionDelta.y = 0;
    return delta;
}

#endif // NONOGL_IMPLEMENTATION