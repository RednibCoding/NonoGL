/*
    Dont forget to link against ./minig/lib/<arch>/libglfw3.a
    e.g.: -L./minig/lib/win64 -lglfw3
*/

#ifndef MINI_G_H
#define MINI_G_H

#include <stdbool.h>
#include "internal/glfw3.h"
#include "internal/stb_image.h"
#include "internal/stb_truetype.h"

typedef struct {
    GLFWwindow *window;
    char *title;
    int initialWidth;
    int initialHeight;
    int width;
    int height;
    bool filtered;
    bool scaleable;
    float scale;
    float deltaTime;
    float lastTime;
    int currentFPS;
} _mgState;

typedef struct {
    unsigned int id;
    int width;
    int height;
} mgImage;

typedef struct {
    float x;
    float y;
} mgPointf;

typedef struct {
    float x;
    float y;
    float width;
    float height;
} mgRecf;

typedef struct {
    float r;
    float g;
    float b;
    float a;
} mgColorf;

// Create a window
bool mgCreateWindow(char *title, int width, int height, bool scaleable, bool filtered);

// Release resources and free memory
void mgDestroyWindow();

// Returns true when window close event has been triggered, other false
bool mgWindowShouldClose();

// Set the window title
void mgSetWindowTitle(char *title);

// Set the clear color
void mgSetClearColor(mgColorf color);

// Clear the screen
void mgClear();

// End rendering
void mgSwap();

// Get the delta time
float mgDeltaTime();

// Get the current fps
int mgCurrentFps();

// Load a file as bytes into a buffer and return the pointer to that buffer
unsigned char* mgLoadFileBytes(const char* filepath, int* size);

// Free the buffer allocated by mgLoadFileBytes
void mgFreeFileBytes(unsigned char* buffer);

// Load a png image from file
mgImage mgLoadImage(const char* filepath);

// Load a png image from memory
mgImage mgLoadImageMem(const unsigned char *data, int size);

// Draw an image on the screen
void mgDrawImage(mgImage image, mgPointf pos);

// Draw a portion of an image
void mgDrawImagePortion(mgImage image, mgPointf pos, mgRecf srcRec);

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

/*  IMPLEMENTATION */

/******************************************************************************************************************************/
#ifdef MG_IMPL

#define STB_IMAGE_IMPLEMENTATION
#include "internal/stb_image.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "internal/stb_truetype.h"

_mgState _mgstate;

void _mgKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void _mgMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        printf("Left mouse button pressed\n");
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        printf("Right mouse button pressed\n");
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
        printf("Middle mouse button pressed\n");
}

void _mgMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    printf("Scroll offset: (%.2f, %.2f)\n", xoffset, yoffset);
}

void _mgCursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    printf("Cursor position: (%.2f, %.2f)\n", xpos, ypos);
}

void _mgSetupOrthoProjection(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void _mgSetScale(float scaleX, float scaleY) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(scaleX, scaleY, 1.0f);
}

void _mgUpdateScale(int width, int height, int shouldUpdate) {
    if (shouldUpdate != 0) {
        float scaleX = (float)width / _mgstate.initialWidth;
        float scaleY = (float)height / _mgstate.initialHeight;
        _mgSetScale(scaleX, scaleY);
    }
}

void _mgFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    _mgSetupOrthoProjection(width, height);
    _mgUpdateScale(width, height, _mgstate.scaleable);
}

/*******************************************************************************************************/
// Publing api functions
/*******************************************************************************************************/

bool mgCreateWindow(char *title, int width, int height, bool scaleable, bool filtered) {
    _mgstate.scaleable = scaleable;
    _mgstate.filtered = filtered;

    _mgstate.title = title;
    _mgstate.initialWidth = width;
    _mgstate.initialHeight = height;
    _mgstate.width = width;
    _mgstate.height = height;

    _mgstate.deltaTime = 0.0f;
    _mgstate.lastTime = glfwGetTime();
    _mgstate.currentFPS = 0;

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    if (!glfwInit())
        return false;

    /* Create a windowed mode window and its OpenGL context */
    _mgstate.window = glfwCreateWindow(_mgstate.initialWidth, _mgstate.initialHeight, _mgstate.title, NULL, NULL);
    if (!_mgstate.window)
    {
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(_mgstate.window);

    /* Set the key callback */
    glfwSetKeyCallback(_mgstate.window, _mgKeyCallback);

    /* Set the mouse button and cursor position callbacks */
    glfwSetMouseButtonCallback(_mgstate.window, _mgMouseButtonCallback);
    glfwSetCursorPosCallback(_mgstate.window, _mgCursorPositionCallback);

    /* Set the scroll callback */
    glfwSetScrollCallback(_mgstate.window, _mgMouseScrollCallback);

    /* Set the framebuffer size callback to handle window resizing */
    glfwSetFramebufferSizeCallback(_mgstate.window, _mgFramebufferSizeCallback);

    /* Set up the orthographic projection */
    glfwGetFramebufferSize(_mgstate.window, &_mgstate.width, &_mgstate.height);
    _mgSetupOrthoProjection(_mgstate.width, _mgstate.height);
    _mgUpdateScale(_mgstate.width, _mgstate.height, _mgstate.scaleable);

    /* Set initial scale */
    _mgSetScale(1.0f, 1.0f);

    glfwSwapInterval(1); // Default to 60 FPS

    return true;
}

void mgDestroyWindow() {

    if (_mgstate.window) {
        glfwDestroyWindow(_mgstate.window);
    }    
    glfwTerminate();
}

bool mgWindowShouldClose() {
    return glfwWindowShouldClose(_mgstate.window);
}

void mgSetWindowTitle(char *title) {
    glfwSetWindowTitle(_mgstate.window, title);
}

// Set the clear color
void mgSetClearColor(mgColorf color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

// Clear the screen
void mgClear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void mgSwap() {
    /* Swap front and back buffers */
    glfwSwapBuffers(_mgstate.window);

    /* Poll for and process events */
    glfwPollEvents();

    // Calculate deltaTime and currentFPS
    double currentTime = glfwGetTime();
    _mgstate.deltaTime = currentTime - _mgstate.lastTime;
    _mgstate.currentFPS = 1.0 / _mgstate.deltaTime;
    _mgstate.lastTime = currentTime;
}

float mgDeltaTime() {
    return _mgstate.deltaTime;
}

int mgCurrentFps() {
    return _mgstate.currentFPS;
}

unsigned char* mgLoadFileBytes(const char* filepath, int* size) {
    FILE* file = fopen(filepath, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* buffer = (unsigned char*)malloc(*size);
    if (!buffer) {
        printf("Failed to allocate memory for file: %s\n", filepath);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, *size, file);
    fclose(file);

    return buffer;
}

void mgFreeFileBytes(unsigned char* buffer) {
    if (buffer) {
        free(buffer);
    }
}

mgImage mgLoadImage(const char* filepath) {
    mgImage image;
    // stbi_set_flip_vertically_on_load(1);
    unsigned char* imageData = stbi_load(filepath, &image.width, &image.height, 0, 4);
    if (!imageData) {
        printf("Failed to load image\n");
        image.id = 0;
        return image;
    }

    glGenTextures(1, &image.id);
    glBindTexture(GL_TEXTURE_2D, image.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    if (!_mgstate.filtered) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(imageData);

    return image;
}

mgImage mgLoadImageMem(const unsigned char *data, int size) {
    mgImage image;
    unsigned char* imageData = stbi_load_from_memory(data, size, &image.width, &image.height, 0, 4);
    if (!imageData) {
        printf("Failed to load image from memory\n");
        image.id = 0;
        return image;
    }

    glGenTextures(1, &image.id);
    glBindTexture(GL_TEXTURE_2D, image.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

    if (!_mgstate.filtered) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(imageData);

    return image;
}

void mgDrawImage(mgImage image, mgPointf pos) {
    glBindTexture(GL_TEXTURE_2D, image.id);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(pos.x, pos.y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(pos.x + image.width, pos.y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(pos.x + image.width, pos.y + image.height);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(pos.x, pos.y + image.height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void mgDrawImagePortion(mgImage image, mgPointf pos, mgRecf srcRec) {
    glBindTexture(GL_TEXTURE_2D, image.id);
    glEnable(GL_TEXTURE_2D);

    float texLeft = srcRec.x / image.width;
    float texRight = (srcRec.x + srcRec.width) / image.width;
    float texTop = srcRec.y / image.height;
    float texBottom = (srcRec.y + srcRec.height) / image.height;

    glBegin(GL_QUADS);
        glTexCoord2f(texLeft, texTop); glVertex2f(pos.x, pos.y);
        glTexCoord2f(texRight, texTop); glVertex2f(pos.x + srcRec.width, pos.y);
        glTexCoord2f(texRight, texBottom); glVertex2f(pos.x + srcRec.width, pos.y + srcRec.height);
        glTexCoord2f(texLeft, texBottom); glVertex2f(pos.x, pos.y + srcRec.height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

bool mgPointRecOverlaps(mgPointf point, mgRecf rec) {
    return (point.x >= rec.x && point.x <= rec.x + rec.width && point.y >= rec.y && point.y <= rec.y + rec.height) ? true : false;
}

bool mgRecsOverlap(mgRecf rec1, mgRecf rec2) {
    return (rec1.x < rec2.x + rec2.width && rec1.x + rec1.width > rec2.x && rec1.y < rec2.y + rec2.height && rec1.y + rec1.height > rec2.y) ? true : false;
}

bool mgPointCircleOverlaps(mgPointf point, mgPointf circleCenter, float circleRadius) {
    float dx = point.x - circleCenter.x;
    float dy = point.y - circleCenter.y;
    return (dx * dx + dy * dy <= circleRadius * circleRadius) ? true : false;
}

bool mgRecCircleOverlaps(mgRecf rec, mgPointf circleCenter, float circleRadius) {
    // Find the closest point on the rectangle to the circle center
    float closestX = (circleCenter.x < rec.x) ? rec.x : (circleCenter.x > rec.x + rec.width) ? rec.x + rec.width : circleCenter.x;
    float closestY = (circleCenter.y < rec.y) ? rec.y : (circleCenter.y > rec.y + rec.height) ? rec.y + rec.height : circleCenter.y;
    
    // Calculate the distance between the circle center and this closest point
    float dx = circleCenter.x - closestX;
    float dy = circleCenter.y - closestY;
    
    return (dx * dx + dy * dy <= circleRadius * circleRadius) ? true : false;
}

bool mgCirclesOverlaps(mgPointf circle1Center, float circle1Radius, mgPointf circle2Center, float circle2Radius) {
    float dx = circle1Center.x - circle2Center.x;
    float dy = circle1Center.y - circle2Center.y;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = circle1Radius + circle2Radius;
    return (distanceSquared <= radiusSum * radiusSum) ? true : false;
}

#endif // MG_IMPL