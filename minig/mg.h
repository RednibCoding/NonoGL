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

#endif // MINI_G_H

/******************************************************************************************************************************/

/******************************************************************************************************************************/

/*  IMPLEMENTATION */

/******************************************************************************************************************************/
#ifdef MG_IMPL

#define STB_IMAGE_IMPLEMENTATION
#include "internal/include/stb_image.h"

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
} _mgState;

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

void _mgKeyCallback(unsigned char key, int x, int y)
{
    if (key == 27) // ESC key
        printf("Hello key");
    glutLeaveMainLoop();
}

void _mgSpecialKeyCallback(int key, int x, int y)
{
    // Handle special keys if needed
}

void _mgMouseButtonCallback(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        printf("Left mouse button pressed\n");
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        printf("Right mouse button pressed\n");
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
        printf("Middle mouse button pressed\n");
}

void _mgMouseScrollCallback(int button, int dir, int x, int y)
{
    // This is how FreeGLUT handles mouse wheel
    if (dir > 0)
        printf("Mouse wheel up\n");
    else
        printf("Mouse wheel down\n");
}

void _mgCursorPositionCallback(int x, int y)
{
    printf("Cursor position: (%d, %d)\n", x, y);
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
    glutKeyboardFunc(_mgKeyCallback);
    glutSpecialFunc(_mgSpecialKeyCallback);
    glutMouseFunc(_mgMouseButtonCallback);
    glutMotionFunc(_mgCursorPositionCallback);
    glutPassiveMotionFunc(_mgCursorPositionCallback);
    glutMouseWheelFunc(_mgMouseScrollCallback);
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

// Function to draw text with formatting support
void mgDrawText(const char *format, mgPointf pos, ...)
{
    char buffer[1024]; // Buffer to hold the formatted string

    // Initialize the variable argument list
    va_list args;
    va_start(args, pos);

    // Format the string
    vsnprintf(buffer, sizeof(buffer), format, args);

    // End the variable argument list
    va_end(args);

    // Set the position for the text
    glRasterPos2f(pos.x, pos.y);

    // Render each character of the formatted string
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

#endif // MG_IMPL