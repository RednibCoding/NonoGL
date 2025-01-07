#define FREEGLUT_STATIC

#ifndef TINY_GL_H
#define TINY_GL_H

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
} tgVec2;

typedef struct
{
    float x;
    float y;
    float width;
    float height;
} tgRecf;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} tgColorf;

typedef struct
{
    unsigned int textureID;
    int width;
    int height;
} tgImage;

typedef struct
{
    unsigned int textureID; // OpenGL texture ID
    int width;              // Width of the buffer
    int height;             // Height of the buffer
    tgColorf *pixels;       // CPU-side pixel buffer
} tgPixmap;

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
} tgFont;

int tgFPS;
float tgDT;

// Create a window
bool tgCreateWindow(char *title, int width, int height, bool scalable, bool filtered);

// Release resources and free memory
void tgDestroyWindow();

// Set the window title
void tgSetWindowTitle(const char *format, ...);

// Set the clear color
void tgSetClearColor(tgColorf color);

// Set the draw color
void tgSetColor(tgColorf color);

// Resets the color to default (1.0, 1.0, 1.0)
void tgResetColor();

// Clear the screen
void tgCls();

// End rendering and swap buffers
void tgFlip();

// Load a file as bytes into a buffer and return the pointer to that buffer
unsigned char *mgLoadFileBytes(const char *filepath, int *size);

// Free the buffer allocated by tgLoadFileBytes
void tgFreeFileBytes(unsigned char *buffer);

// Load an image from file
tgImage tgLoadImage(const char *filepath);

// Load an image from memory
tgImage tgLoadImageMem(const unsigned char *data, int size);

// Draw an image on the screen
void tgDrawImage(tgImage image, int x, int y);

// Draw a portion of an image
void tgDrawImagePortion(tgImage image, int x, int y, tgRecf srcRec);

// Frees the given image
void tgFreeImage(tgImage image);

// Create a pixel buffer with the given width and height
tgPixmap *mgCreatePixmap(int width, int height);

// Write a pixel at x, y location with the given color to the given pixel buffer
void tgPutPixel(tgPixmap *buffer, int x, int y, tgColorf color);

// Update pixels that have changed in the pixel buffer
void tgUpdatePixmap(tgPixmap *buffer);

// Draw the pixel buffer to the screen
void tgDrawPixmap(tgPixmap *buffer);

// Free the given pixel buffer
void tgFreePixmap(tgPixmap *buffer);

// Draw an individual pixel to the screen (use a pixel buffer for large chunks of pixels instead)
void drawPixel(float x, float y);

// Load a font from a file
tgFont *tgLoadFont(const char *filepath, float fontSize);

// Set font for text rendering, if not set, a default non-scalable font will be used
void tgSetFont(tgFont *font);

// Render text using the font
void tgDrawText(const char *format, int x, int y, ...);

// Free the font
void tgFreeFont(tgFont *font);

// Check if a point overlaps with a rectangle
bool tgVec2RecOverlaps(int x, int y, tgRecf rect);

// Check if two rectangles overlap
bool tgRecsOverlap(tgRecf rec1, tgRecf rec2);

// Check if a point overlaps with a circle
bool tgVec2CircleOverlaps(int x, int y, int cx, int cy, float circleradius);

// Check if a rectangle overlaps with a circle
bool tgRecCircleOverlaps(tgRecf rec, int cx, int cy, float circleradius);

// Check if two cirles overlap
bool tgCirclesOverlaps(int cx1, int cy1, float circle1radius, int cx2, int cy2, float circle2radius);

// Returns `true` if the specified key was pressed since the last frame.
bool tgKeyHit(int key);

// Returns `true` if the specified key is currently being held down.
bool tgKeyDown(int key);

// Returns `true` if the specified key was released since the last frame.
bool tgKeyReleased(int key);

// Returns `true` if the specified mouse button was clicked since the last frame.
bool tgMouseHit(int button);

// Returns `true` if the specified mouse button is currently being held down.
bool tgMouseDown(int button);

// Returns `true` if the specified mouse button was released since the last frame.
bool tgMouseReleased(int button);

// Returns the direction of the mouse wheel movement: `1` for up, `-1` for down, and `0` if no movement occurred.
int tgMouseWheelDelta();

// Returns the current position of the mouse cursor as an `mgVec2` struct.
tgVec2 tgGetMousePosition();

// Returns the mouse motion delta (change in position) since the last frame.
tgVec2 tgMouseMotionDelta();

/******************************************************************************************************************************/
/*  End of Public API */
/******************************************************************************************************************************/
#define _MG_MAX_KEYS 256
#define _MG_MAX_MOUSE_BUTTONS 3

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
    tgFont *font;

    // Keyboard state
    bool keys[_MG_MAX_KEYS];
    bool keysPressed[_MG_MAX_KEYS];
    bool keysReleased[_MG_MAX_KEYS];

    // Mouse state
    bool mouseButtons[_MG_MAX_MOUSE_BUTTONS];
    bool mouseButtonsPressed[_MG_MAX_MOUSE_BUTTONS];
    bool mouseButtonsReleased[_MG_MAX_MOUSE_BUTTONS];
    int mouseWheelDelta;
    tgVec2 mousePosition;
    tgVec2 mouseMotionDelta;

} _tgState;
#endif // TINY_GL_H

/******************************************************************************************************************************/

/*  IMPLEMENTATION */

/******************************************************************************************************************************/
#ifdef TINYGL_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#include "internal/include/stb_image.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "internal/include/stb_truetype.h"

_tgState _tgstate;

// Global function pointer for the display callback
void (*gDisplayCallback)(void) = NULL;

// Define custom exit handlers
void _customExitFunction(int status)
{
    exit(status);
}

// Wrapper display function that calls the function pointer
void _mgDisplayCallbackWrapper()
{
    if (gDisplayCallback != NULL)
    {
        gDisplayCallback();
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

void _mgSpecialKeyCallback(int key, int x, int y)
{
    // Handle special keys here
}

void _mgSetupOrthoProjection(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void _mgSetScale(float scaleX, float scaleY)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(scaleX, scaleY, 1.0f);
}

void _mgUpdateScale(int width, int height, int shouldUpdate)
{
    if (shouldUpdate != 0)
    {
        float scaleX = (float)width / _tgstate.initialWidth;
        float scaleY = (float)height / _tgstate.initialHeight;
        _mgSetScale(scaleX, scaleY);
    }
}

void _mgFramebufferSizeCallback(int width, int height)
{
    glViewport(0, 0, width, height);
    _mgSetupOrthoProjection(width, height);
    _mgUpdateScale(width, height, _tgstate.scalable);
}

void _mgTimerCallback(int value)
{
    // Calculate deltaTime and currentFPS
    double currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0; // Get time in seconds
    _tgstate.deltaTime = currentTime - _tgstate.lastTime;
    _tgstate.currentFPS = (_tgstate.deltaTime > 0) ? (1.0 / _tgstate.deltaTime) : 0;
    _tgstate.lastTime = currentTime;

    tgFPS = _tgstate.currentFPS;
    tgDT = _tgstate.deltaTime;

    // Trigger display refresh
    glutPostRedisplay();

    // Re-register the timer callback for 60 FPS
    glutTimerFunc(1000 / 60, _mgTimerCallback, 0);
}

void _mgKeyDownCallback(unsigned char key, int x, int y)
{
    if (!_tgstate.keys[key])
    {
        _tgstate.keysPressed[key] = true;
    }
    _tgstate.keys[key] = true;
}

void _mgKeyUpCallback(unsigned char key, int x, int y)
{
    _tgstate.keys[key] = false;
    _tgstate.keysReleased[key] = true;
}

void _mgMouseButtonCallback(int button, int state, int x, int y)
{
    if (button < _MG_MAX_MOUSE_BUTTONS)
    {
        if (state == GLUT_DOWN)
        {
            if (!_tgstate.mouseButtons[button])
            {
                _tgstate.mouseButtonsPressed[button] = true;
            }
            _tgstate.mouseButtons[button] = true;
        }
        else if (state == GLUT_UP)
        {
            _tgstate.mouseButtons[button] = false;
            _tgstate.mouseButtonsReleased[button] = true;
        }
    }
}

void _mgMouseWheelFunc(int wheel, int direction, int x, int y)
{
    _tgstate.mouseWheelDelta = direction; // +1 for up, -1 for down
}

void _mgMouseMotionFunc(int x, int y)
{
    // Motion delta
    tgVec2 newMousePosition = {(float)x, (float)y};
    _tgstate.mouseMotionDelta.x = newMousePosition.x - _tgstate.mousePosition.x;
    _tgstate.mouseMotionDelta.y = newMousePosition.y - _tgstate.mousePosition.y;
    _tgstate.mousePosition = newMousePosition;
}

void _mgDrawTextDefaultFont(const char *format, int x, int y, ...)
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

bool tgCreateWindow(char *title, int width, int height, bool scalable, bool filtered)
{
    _tgstate.scalable = scalable;
    _tgstate.filtered = filtered;

    _tgstate.title = title;
    _tgstate.initialWidth = width;
    _tgstate.initialHeight = height;
    _tgstate.width = width;
    _tgstate.height = height;

    _tgstate.deltaTime = 0.0f;
    _tgstate.lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    _tgstate.currentFPS = 0;

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    int argc = 1;
    char *argv[1] = {(char *)"Something"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    _tgstate.window = glutCreateWindow(title);

    /* Set the callbacks */
    glutKeyboardFunc(_mgKeyDownCallback);
    glutKeyboardUpFunc(_mgKeyUpCallback);
    glutMouseFunc(_mgMouseButtonCallback);
    glutMouseWheelFunc(_mgMouseWheelFunc);
    glutMotionFunc(_mgMouseMotionFunc);
    glutPassiveMotionFunc(_mgMouseMotionFunc);
    glutSpecialFunc(_mgSpecialKeyCallback);
    glutReshapeFunc(_mgFramebufferSizeCallback);

    /* Set up the orthographic projection */
    _mgSetupOrthoProjection(_tgstate.width, _tgstate.height);
    _mgUpdateScale(_tgstate.width, _tgstate.height, _tgstate.scalable);

    /* Set initial scale */
    _mgSetScale(1.0f, 1.0f);

    /* Start the timer for limiting FPS */
    glutTimerFunc(1000 / 60, _mgTimerCallback, 0);

    return true;
}

void tgDestroyWindow()
{
    glutDestroyWindow(_tgstate.window);
}

void tgSetClearColor(tgColorf color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void tgSetColor(tgColorf color)
{
    glColor4f(color.r, color.g, color.b, color.a);
}

void tgResetColor()
{
    glColor4f(1.0, 1.0, 1.0, 1.0);
}

void tgSetDisplayLoop(void (*callback)(void))
{
    gDisplayCallback = callback;
    glutDisplayFunc(_mgDisplayCallbackWrapper);
}

void tgRun()
{
    if (gDisplayCallback == NULL)
    {
        printf("DisplayCallback not set, shutting down...");
        exit(-1);
    }
    glutMainLoop();
}

void tgCls()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void tgSetWindowTitle(const char *format, ...)
{
    char buffer[256];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    glutSetWindowTitle(buffer);
}

void tgFlip()
{
    glutSwapBuffers();
    glutMainLoopEvent();
}

unsigned char *mgLoadFileBytes(const char *filepath, int *size)
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

void tgFreeFileBytes(unsigned char *buffer)
{
    if (buffer)
    {
        free(buffer);
    }
}

tgImage tgLoadImage(const char *filepath)
{
    tgImage image;
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

    if (!_tgstate.filtered)
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

tgImage tgLoadImageMem(const unsigned char *data, int size)
{
    tgImage image;
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

    if (_tgstate.filtered)
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

void tgDrawImage(tgImage image, int x, int y)
{
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

void tgDrawImagePortion(tgImage image, int x, int y, tgRecf srcRec)
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

void tgFreeImage(tgImage image)
{
    if (!image.textureID == 0)
        return;

    glDeleteTextures(1, &image.textureID);
}

tgPixmap *mgCreatePixmap(int width, int height)
{
    tgPixmap *buffer = malloc(sizeof(tgPixmap));
    if (!buffer)
        return NULL;

    buffer->width = width;
    buffer->height = height;

    // Allocate pixel memory
    buffer->pixels = calloc(width * height, sizeof(tgColorf));
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    return buffer;
}

void tgPutPixel(tgPixmap *buffer, int x, int y, tgColorf color)
{
    if (!buffer || x < 0 || y < 0 || x >= buffer->width || y >= buffer->height)
        return;

    buffer->pixels[y * buffer->width + x] = color;
}

void tgUpdatePixmap(tgPixmap *buffer)
{
    if (!buffer || !buffer->pixels)
        return;

    glBindTexture(GL_TEXTURE_2D, buffer->textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, buffer->width, buffer->height, GL_RGBA, GL_FLOAT, buffer->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void tgDrawPixmap(tgPixmap *buffer)
{
    if (!buffer)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, buffer->textureID);

    // Draw a full-screen quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0.0f, 0.0f); // Bottom-left
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(buffer->width, 0.0f); // Bottom-right
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(buffer->width, buffer->height); // Top-right
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, buffer->height); // Top-left
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void tgFreePixmap(tgPixmap *buffer)
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

tgFont *tgLoadFont(const char *filepath, float fontSize)
{
    tgFont *font = malloc(sizeof(tgFont));
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    free(bitmap);

    return font;
}

void tgSetFont(tgFont *font)
{
    if (!font)
        return;

    _tgstate.font = font;
}

void tgDrawText(const char *format, int x, int y, ...)
{
    if (!format)
        return;

    if (!_tgstate.font)
    {
        _mgDrawTextDefaultFont(format, x, y);
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
    glBindTexture(GL_TEXTURE_2D, _tgstate.font->textureID);

    glBegin(GL_QUADS);

    float tx = (float)x;
    float ty = (float)y;

    for (char *c = buffer; *c != '\0'; c++)
    {
        if (*c < 32 || *c >= 128)
            continue;

        stbtt_aligned_quad quad;
        stbtt_GetBakedQuad(_tgstate.font->charData, _tgstate.font->atlasWidth, _tgstate.font->atlasHeight, *c - 32, &tx, &ty, &quad, 1);

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

void tgFreeFont(tgFont *font)
{
    if (!font)
        return;

    if (font->fontBuffer)
        free(font->fontBuffer);

    glDeleteTextures(1, &font->textureID);
    free(font);
}

bool tgVec2RecOverlaps(int x, int y, tgRecf rec)
{
    return (x >= rec.x && x <= rec.x + rec.width && y >= rec.y && y <= rec.y + rec.height) ? true : false;
}

bool tgRecsOverlap(tgRecf rec1, tgRecf rec2)
{
    return (rec1.x < rec2.x + rec2.width && rec1.x + rec1.width > rec2.x && rec1.y < rec2.y + rec2.height && rec1.y + rec1.height > rec2.y) ? true : false;
}

bool tgVec2CircleOverlaps(int x, int y, int cx, int cy, float circleRadius)
{
    float dx = x - cx;
    float dy = y - cy;
    return (dx * dx + dy * dy <= circleRadius * circleRadius) ? true : false;
}

bool tgRecCircleOverlaps(tgRecf rec, int cx, int cy, float circleRadius)
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

bool tgCirclesOverlaps(int cx1, int cy1, float circle1Radius, int cx2, int cy2, float circle2Radius)
{
    float dx = cx1 - cx2;
    float dy = cy1 - cy2;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = circle1Radius + circle2Radius;
    return (distanceSquared <= radiusSum * radiusSum) ? true : false;
}

bool tgKeyHit(int key)
{
    bool wasPressed = _tgstate.keysPressed[key];
    _tgstate.keysPressed[key] = false; // Reset after checking
    return wasPressed;
}

bool tgKeyDown(int key)
{
    return _tgstate.keys[key];
}

bool tgKeyReleased(int key)
{
    bool wasReleased = _tgstate.keysReleased[key];
    _tgstate.keysReleased[key] = false; // Reset after checking
    return wasReleased;
}

bool tgMouseHit(int button)
{
    bool wasPressed = _tgstate.mouseButtonsPressed[button];
    _tgstate.mouseButtonsPressed[button] = false; // Reset after checking
    return wasPressed;
}

bool tgMouseDown(int button)
{
    return _tgstate.mouseButtons[button];
}

bool tgMouseReleased(int button)
{
    bool wasReleased = _tgstate.mouseButtonsReleased[button];
    _tgstate.mouseButtonsReleased[button] = false; // Reset after checking
    return wasReleased;
}

int tgMouseWheelDelta()
{
    int delta = _tgstate.mouseWheelDelta;
    _tgstate.mouseWheelDelta = 0; // Reset after checking
    return delta;
}

tgVec2 tgGetMousePosition()
{
    return _tgstate.mousePosition;
}

tgVec2 tgMouseMotionDelta()
{
    tgVec2 delta = _tgstate.mouseMotionDelta;
    _tgstate.mouseMotionDelta.x = 0; // Reset after checking
    _tgstate.mouseMotionDelta.y = 0;
    return delta;
}

#endif // TINYGL_IMPLEMENTATION