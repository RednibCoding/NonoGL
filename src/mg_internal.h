// can't use pragma once here because this file probably will endup in .c
#ifndef __MINIGFX_INTERNAL_H__
#define __MINIGFX_INTERNAL_H__

#include "mg_header.h"



// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy Start  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



#define _CRT_SECURE_NO_WARNINGS NOPE

// Creates a new bitmap, with extra payload bytes.
MgSurface* mgBitmap2(int w, int h, int extra);

// Resizes an existing bitmap.
void mgResize(MgSurface* surface, int w, int h);

// Calculates the biggest scale that a bitmap can fit into an area at.
int mgCalcScale(int bmpW, int bmpH, int areaW, int areaH);

// Calculates a new scale, taking minimum-scale flags into account.
int mgEnforceScale(int scale, int flags);

// Calculates the correct position for a bitmap to fit into a window.
void mgPosition(MgSurface* surface, int scale, int windowW, int windowH, int out[4]);

// ----------------------------------------------------------
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#endif

#if __linux__
#include <X11/X.h>
#include <X11/Xlib.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glx.h>
#endif

typedef struct {

#ifdef _WIN32
    HGLRC hglrc;
    HDC dc;
#endif
    GLuint tex[2];
    GLuint vao;
    GLuint program;
    GLuint uniform_projection;
    GLuint uniform_model;
    GLuint uniform_parameters;
    int gl_legacy;
    int gl_user_opengl_rendering;
} GLStuff;


typedef struct {
    int shown, closed;
    GLStuff gl;

#ifdef _WIN32
    wchar_t* wtitle;
    DWORD dwStyle;
    RECT oldPos;
#endif // _WIN32

#ifdef __linux__
    Display* dpy;
    Window win;
    GLXContext glc;
    XIC ic;
#endif  // __linux__

    MgSurface* widgets;
    int widgetsWanted;
    unsigned char widgetAlpha;
    float widgetsScale;

    float p1, p2, p3, p4;

    int flags;
    int scale;
    int pos[4];
    int lastChar;
    char keys[256], prev[256];


#if defined(__linux__) 
    int mouseButtons;
    int mouseX;
    int mouseY;
#endif  // __linux__


} MgInternal;

// ----------------------------------------------------------

MgInternal* mgInternal(MgSurface* surface);

void mgGAPICreate(MgSurface* surface);
void mgGAPIDestroy(MgSurface* surface);
int mgGAPIBegin(MgSurface* surface);
int mgGAPIEnd(MgSurface* surface);
void mgGAPIPresent(MgSurface* surface, int w, int h);



// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy End  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^



#endif // __MINIGFX_INTERNAL_H__
