#define FREEGLUT_STATIC

#ifndef MINI_G_H
#define MINI_G_H

#include <stdbool.h>
#include "internal/include/GL/freeglut.h"
#include "internal/include/stb_image.h"

/******************************************************************************************************************************/

/*  Public API */

/******************************************************************************************************************************/

typedef struct
{
    float x;
    float y;
} mgPointf;

typedef struct
{
    float x;
    float y;
    float width;
    float height;
} mgRecf;

typedef struct
{
    float r;
    float g;
    float b;
    float a;
} mgColorf;

typedef struct
{
    unsigned int id;
    int width;
    int height;
} mgImage;

typedef struct
{
    unsigned int textureID; // OpenGL texture ID
    int width;              // Width of the buffer
    int height;             // Height of the buffer
    mgColorf *pixels;       // CPU-side pixel buffer
} mgPixelBuffer;

int mgFPS;
float mgDT;

// Create a window
bool mgCreateWindow(char *title, int width, int height, bool scalable, bool filtered);

// Release resources and free memory
void mgDestroyWindow();

// Set the window title
void mgSetWindowTitle(const char *format, ...);

// Set the clear color
void mgSetClearColor(mgColorf color);

// Set the draw color
void mgSetColor(mgColorf color);

// Clear the screen
void mgCls();

// End rendering and swap buffers
void mgFlip();

// Load a file as bytes into a buffer and return the pointer to that buffer
unsigned char *mgLoadFileBytes(const char *filepath, int *size);

// Free the buffer allocated by mgLoadFileBytes
void mgFreeFileBytes(unsigned char *buffer);

// Load an image from file
mgImage mgLoadImage(const char *filepath);

// Load an image from memory
mgImage mgLoadImageMem(const unsigned char *data, int size);

// Draw an image on the screen
void mgDrawImage(mgImage image, mgPointf pos);

// Draw a portion of an image
void mgDrawImagePortion(mgImage image, mgPointf pos, mgRecf srcRec);

// Create a pixel buffer with the given width and height
mgPixelBuffer *mgCreatePixelBuffer(int width, int height);

// Write a pixel at x, y location with the given color to the given pixel buffer
void mgPutPixel(mgPixelBuffer *buffer, int x, int y, mgColorf color);

// Update pixels that have changed in the pixel buffer
void mgUpdatePixelBuffer(mgPixelBuffer *buffer);

// Draw the pixel buffer to the screen
void mgDrawPixelBuffer(mgPixelBuffer *buffer);

// Free the given pixel buffer
void mgFreePixelBuffer(mgPixelBuffer *buffer);

// Draw an individual pixel to the screen (use a pixel buffer for large chunks of pixels instead)
void drawPixel(float x, float y, mgColorf color);

// Draw text on the screen
void mgDrawText(const char *format, mgPointf pos, ...);

// Check if a point overlaps with a rectangle
bool mgPointRecOverlaps(mgPointf point, mgRecf rect);

// Check if two rectangles overlap
bool mgRecsOverlap(mgRecf rec1, mgRecf rec2);

// Check if a point overlaps with a circle
bool mgPointCircleOverlaps(mgPointf point, mgPointf circlecenter, float circleradius);

// Check if a rectangle overlaps with a circle
bool mgRecCircleOverlaps(mgRecf rec, mgPointf circlecenter, float circleradius);

// Check if two cirles overlap
bool mgCirclesOverlaps(mgPointf circle1center, float circle1radius, mgPointf circle2center, float circle2radius);

// Returns `true` if the specified key was pressed since the last frame.
bool mgKeyHit(int key);

// Returns `true` if the specified key is currently being held down.
bool mgKeyDown(int key);

// Returns `true` if the specified key was released since the last frame.
bool mgKeyReleased(int key);

// Returns `true` if the specified mouse button was clicked since the last frame.
bool mgMouseHit(int button);

// Returns `true` if the specified mouse button is currently being held down.
bool mgMouseDown(int button);

// Returns `true` if the specified mouse button was released since the last frame.
bool mgMouseReleased(int button);

// Returns the direction of the mouse wheel movement: `1` for up, `-1` for down, and `0` if no movement occurred.
int mgMouseWheelDelta();

// Returns the current position of the mouse cursor as an `mgPointf` struct.
mgPointf mgGetMousePosition();

// Returns the mouse motion delta (change in position) since the last frame.
mgPointf mgMouseMotionDelta();

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

    // Keyboard state
    bool keys[_MG_MAX_KEYS];
    bool keysPressed[_MG_MAX_KEYS];
    bool keysReleased[_MG_MAX_KEYS];

    // Mouse state
    bool mouseButtons[_MG_MAX_MOUSE_BUTTONS];
    bool mouseButtonsPressed[_MG_MAX_MOUSE_BUTTONS];
    bool mouseButtonsReleased[_MG_MAX_MOUSE_BUTTONS];
    int mouseWheelDelta;
    mgPointf mousePosition;
    mgPointf mouseMotionDelta;

} _mgState;
#endif // MINI_G_H

/******************************************************************************************************************************/

/*  IMPLEMENTATION */

/******************************************************************************************************************************/
#ifdef MG_IMPL

#define STB_IMAGE_IMPLEMENTATION
#include "internal/include/stb_image.h"

_mgState _mgstate;

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
        float scaleX = (float)width / _mgstate.initialWidth;
        float scaleY = (float)height / _mgstate.initialHeight;
        _mgSetScale(scaleX, scaleY);
    }
}

void _mgFramebufferSizeCallback(int width, int height)
{
    glViewport(0, 0, width, height);
    _mgSetupOrthoProjection(width, height);
    _mgUpdateScale(width, height, _mgstate.scalable);
}

void _mgTimerCallback(int value)
{
    // Calculate deltaTime and currentFPS
    double currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0; // Get time in seconds
    _mgstate.deltaTime = currentTime - _mgstate.lastTime;
    _mgstate.currentFPS = (_mgstate.deltaTime > 0) ? (1.0 / _mgstate.deltaTime) : 0;
    _mgstate.lastTime = currentTime;

    mgFPS = _mgstate.currentFPS;
    mgDT = _mgstate.deltaTime;

    // Trigger display refresh
    glutPostRedisplay();

    // Re-register the timer callback for 60 FPS
    glutTimerFunc(1000 / 60, _mgTimerCallback, 0);
}

void _mgKeyDownCallback(unsigned char key, int x, int y)
{
    if (!_mgstate.keys[key])
    {
        _mgstate.keysPressed[key] = true;
    }
    _mgstate.keys[key] = true;
}

void _mgKeyUpCallback(unsigned char key, int x, int y)
{
    _mgstate.keys[key] = false;
    _mgstate.keysReleased[key] = true;
}

void _mgMouseButtonCallback(int button, int state, int x, int y)
{
    if (button < _MG_MAX_MOUSE_BUTTONS)
    {
        if (state == GLUT_DOWN)
        {
            if (!_mgstate.mouseButtons[button])
            {
                _mgstate.mouseButtonsPressed[button] = true;
            }
            _mgstate.mouseButtons[button] = true;
        }
        else if (state == GLUT_UP)
        {
            _mgstate.mouseButtons[button] = false;
            _mgstate.mouseButtonsReleased[button] = true;
        }
    }
}

void _mgMouseWheelFunc(int wheel, int direction, int x, int y)
{
    _mgstate.mouseWheelDelta = direction; // +1 for up, -1 for down
}

void _mgMouseMotionFunc(int x, int y)
{
    // Motion delta
    mgPointf newMousePosition = {(float)x, (float)y};
    _mgstate.mouseMotionDelta.x = newMousePosition.x - _mgstate.mousePosition.x;
    _mgstate.mouseMotionDelta.y = newMousePosition.y - _mgstate.mousePosition.y;
    _mgstate.mousePosition = newMousePosition;
}

/*******************************************************************************************************/
// Public api function implementations
/*******************************************************************************************************/

bool mgCreateWindow(char *title, int width, int height, bool scalable, bool filtered)
{
    _mgstate.scalable = scalable;
    _mgstate.filtered = filtered;

    _mgstate.title = title;
    _mgstate.initialWidth = width;
    _mgstate.initialHeight = height;
    _mgstate.width = width;
    _mgstate.height = height;

    _mgstate.deltaTime = 0.0f;
    _mgstate.lastTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    _mgstate.currentFPS = 0;

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    int argc = 1;
    char *argv[1] = {(char *)"Something"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    _mgstate.window = glutCreateWindow(title);

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
    _mgSetupOrthoProjection(_mgstate.width, _mgstate.height);
    _mgUpdateScale(_mgstate.width, _mgstate.height, _mgstate.scalable);

    /* Set initial scale */
    _mgSetScale(1.0f, 1.0f);

    /* Start the timer for limiting FPS */
    glutTimerFunc(1000 / 60, _mgTimerCallback, 0);

    return true;
}

void mgDestroyWindow()
{
    glutDestroyWindow(_mgstate.window);
}

void mgSetClearColor(mgColorf color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void mgSetColor(mgColorf color)
{
    glColor4f(color.r, color.g, color.b, color.a);
}

void mgSetDisplayLoop(void (*callback)(void))
{
    gDisplayCallback = callback;
    glutDisplayFunc(_mgDisplayCallbackWrapper);
}

void mgRun()
{
    if (gDisplayCallback == NULL)
    {
        printf("DisplayCallback not set, shutting down...");
        exit(-1);
    }
    glutMainLoop();
}

void mgCls()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void mgSetWindowTitle(const char *format, ...)
{
    char buffer[256];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    glutSetWindowTitle(buffer);
}

void mgFlip()
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

void mgFreeFileBytes(unsigned char *buffer)
{
    if (buffer)
    {
        free(buffer);
    }
}

mgImage mgLoadImage(const char *filepath)
{
    mgImage image;
    // stbi_set_flip_vertically_on_load(1);
    unsigned char *imageData = stbi_load(filepath, &image.width, &image.height, 0, 4);
    if (!imageData)
    {
        printf("Failed to load image:\n%s\n", filepath);
        image.id = 0;
        return image;
    }

    glGenTextures(1, &image.id);
    glBindTexture(GL_TEXTURE_2D, image.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    if (!_mgstate.filtered)
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

mgImage mgLoadImageMem(const unsigned char *data, int size)
{
    mgImage image;
    unsigned char *imageData = stbi_load_from_memory(data, size, &image.width, &image.height, 0, 4);
    if (!imageData)
    {
        printf("Failed to load image from memory\n");
        image.id = 0;
        return image;
    }

    glGenTextures(1, &image.id);
    glBindTexture(GL_TEXTURE_2D, image.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    if (_mgstate.filtered)
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

void mgDrawImage(mgImage image, mgPointf pos)
{
    glBindTexture(GL_TEXTURE_2D, image.id);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(pos.x, pos.y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(pos.x + image.width, pos.y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(pos.x + image.width, pos.y + image.height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(pos.x, pos.y + image.height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void mgDrawImagePortion(mgImage image, mgPointf pos, mgRecf srcRec)
{
    glBindTexture(GL_TEXTURE_2D, image.id);
    glEnable(GL_TEXTURE_2D);

    float texLeft = srcRec.x / image.width;
    float texRight = (srcRec.x + srcRec.width) / image.width;
    float texTop = srcRec.y / image.height;
    float texBottom = (srcRec.y + srcRec.height) / image.height;

    glBegin(GL_QUADS);
    glTexCoord2f(texLeft, texTop);
    glVertex2f(pos.x, pos.y);
    glTexCoord2f(texRight, texTop);
    glVertex2f(pos.x + srcRec.width, pos.y);
    glTexCoord2f(texRight, texBottom);
    glVertex2f(pos.x + srcRec.width, pos.y + srcRec.height);
    glTexCoord2f(texLeft, texBottom);
    glVertex2f(pos.x, pos.y + srcRec.height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

mgPixelBuffer *mgCreatePixelBuffer(int width, int height)
{
    mgPixelBuffer *buffer = malloc(sizeof(mgPixelBuffer));
    if (!buffer)
        return NULL;

    buffer->width = width;
    buffer->height = height;

    // Allocate pixel memory
    buffer->pixels = calloc(width * height, sizeof(mgColorf));
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

void mgPutPixel(mgPixelBuffer *buffer, int x, int y, mgColorf color)
{
    if (!buffer || x < 0 || y < 0 || x >= buffer->width || y >= buffer->height)
        return;

    buffer->pixels[y * buffer->width + x] = color;
}

void mgUpdatePixelBuffer(mgPixelBuffer *buffer)
{
    if (!buffer || !buffer->pixels)
        return;

    glBindTexture(GL_TEXTURE_2D, buffer->textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, buffer->width, buffer->height, GL_RGBA, GL_FLOAT, buffer->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void mgDrawPixelBuffer(mgPixelBuffer *buffer)
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

void mgFreePixelBuffer(mgPixelBuffer *buffer)
{
    if (!buffer)
        return;

    if (buffer->pixels)
        free(buffer->pixels);
    glDeleteTextures(1, &buffer->textureID);
    free(buffer);
}

void drawPixel(float x, float y, mgColorf color)
{
    glColor4f(color.r, color.g, color.b, color.a);

    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void mgDrawText(const char *format, mgPointf pos, ...)
{
    char buffer[1024];

    va_list args;
    va_start(args, pos);

    vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    glRasterPos2f(pos.x, pos.y);

    for (char *c = buffer; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

bool mgPointRecOverlaps(mgPointf point, mgRecf rec)
{
    return (point.x >= rec.x && point.x <= rec.x + rec.width && point.y >= rec.y && point.y <= rec.y + rec.height) ? true : false;
}

bool mgRecsOverlap(mgRecf rec1, mgRecf rec2)
{
    return (rec1.x < rec2.x + rec2.width && rec1.x + rec1.width > rec2.x && rec1.y < rec2.y + rec2.height && rec1.y + rec1.height > rec2.y) ? true : false;
}

bool mgPointCircleOverlaps(mgPointf point, mgPointf circleCenter, float circleRadius)
{
    float dx = point.x - circleCenter.x;
    float dy = point.y - circleCenter.y;
    return (dx * dx + dy * dy <= circleRadius * circleRadius) ? true : false;
}

bool mgRecCircleOverlaps(mgRecf rec, mgPointf circleCenter, float circleRadius)
{
    // Find the closest point on the rectangle to the circle center
    float closestX = (circleCenter.x < rec.x) ? rec.x : (circleCenter.x > rec.x + rec.width) ? rec.x + rec.width
                                                                                             : circleCenter.x;
    float closestY = (circleCenter.y < rec.y) ? rec.y : (circleCenter.y > rec.y + rec.height) ? rec.y + rec.height
                                                                                              : circleCenter.y;

    // Calculate the distance between the circle center and this closest point
    float dx = circleCenter.x - closestX;
    float dy = circleCenter.y - closestY;

    return (dx * dx + dy * dy <= circleRadius * circleRadius) ? true : false;
}

bool mgCirclesOverlaps(mgPointf circle1Center, float circle1Radius, mgPointf circle2Center, float circle2Radius)
{
    float dx = circle1Center.x - circle2Center.x;
    float dy = circle1Center.y - circle2Center.y;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = circle1Radius + circle2Radius;
    return (distanceSquared <= radiusSum * radiusSum) ? true : false;
}

bool mgKeyHit(int key)
{
    bool wasPressed = _mgstate.keysPressed[key];
    _mgstate.keysPressed[key] = false; // Reset after checking
    return wasPressed;
}

bool mgKeyDown(int key)
{
    return _mgstate.keys[key];
}

bool mgKeyReleased(int key)
{
    bool wasReleased = _mgstate.keysReleased[key];
    _mgstate.keysReleased[key] = false; // Reset after checking
    return wasReleased;
}

bool mgMouseHit(int button)
{
    bool wasPressed = _mgstate.mouseButtonsPressed[button];
    _mgstate.mouseButtonsPressed[button] = false; // Reset after checking
    return wasPressed;
}

bool mgMouseDown(int button)
{
    return _mgstate.mouseButtons[button];
}

bool mgMouseReleased(int button)
{
    bool wasReleased = _mgstate.mouseButtonsReleased[button];
    _mgstate.mouseButtonsReleased[button] = false; // Reset after checking
    return wasReleased;
}

int mgMouseWheelDelta()
{
    int delta = _mgstate.mouseWheelDelta;
    _mgstate.mouseWheelDelta = 0; // Reset after checking
    return delta;
}

mgPointf mgGetMousePosition()
{
    return _mgstate.mousePosition;
}

mgPointf mgMouseMotionDelta()
{
    mgPointf delta = _mgstate.mouseMotionDelta;
    _mgstate.mouseMotionDelta.x = 0.0f; // Reset after checking
    _mgstate.mouseMotionDelta.y = 0.0f;
    return delta;
}

#endif // MG_IMPL