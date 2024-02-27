#include "mg_internal.h"


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy Start  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


#if __linux__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xlocale.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>
#include <GL/glx.h>

static Display* dpy;
static Window root;
static XVisualInfo* vi;
static Atom wmDeleteMessage;
static XIM inputMethod;
static GLXFBConfig fbConfig;

static PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = 0;

static void initX11Stuff() {
    static int done = 0;
    if (!done) {
        dpy = XOpenDisplay(NULL);
        if (dpy == NULL) {
            mgError(0, "Cannot connect to X server");
        }

        root = DefaultRootWindow(dpy);

        static int attribList[] = { GLX_RENDER_TYPE,
                                    GLX_RGBA_BIT,
                                    GLX_DRAWABLE_TYPE,
                                    GLX_WINDOW_BIT,
                                    GLX_DOUBLEBUFFER,
                                    1,
                                    GLX_RED_SIZE,
                                    1,
                                    GLX_GREEN_SIZE,
                                    1,
                                    GLX_BLUE_SIZE,
                                    1,
                                    None };

        int fbcCount = 0;
        GLXFBConfig* fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), attribList, &fbcCount);
        if (!fbc) {
            mgError(0, "Failed to choose FB config");
        }
        fbConfig = fbc[0];

        vi = glXGetVisualFromFBConfig(dpy, fbConfig);
        if (vi == NULL) {
            mgError(0, "No appropriate visual found");
        }

        GLXContext tmpCtx = glXCreateContext(dpy, vi, 0, GL_TRUE);
        glXCreateContextAttribsARB =
            (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");
        glXDestroyContext(dpy, tmpCtx);
        if (!glXCreateContextAttribsARB) {
            mgError(0, "Failed to get glXCreateContextAttribsARB");
        }

        inputMethod = XOpenIM(dpy, NULL, NULL, NULL);
        if (inputMethod == NULL) {
            mgError(0, "Failed to create input method");
        }

        wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);

        done = 1;
    }
}

static int hasGLXExtension(Display* display, const char* wanted) {
    const char* extensions = glXQueryExtensionsString(display, DefaultScreen(display));
    char* dup = strdup(extensions);
    char* found = 0;

    for (char* start = dup;; start = 0) {
        found = strtok(start, " ");
        if (found == 0 || strcmp(found, wanted) == 0) {
            break;
        }
    }

    free(dup);
    return found != 0;
}

static void setupVSync(Display* display, Window win) {
    if (hasGLXExtension(display, "GLX_EXT_swap_control")) {
        PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT =
            (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddressARB((const GLubyte*)"glXSwapIntervalEXT");
        if (glXSwapIntervalEXT) {
            glXSwapIntervalEXT(display, win, 1);
        }
    } else if (hasGLXExtension(display, "GLX_MESA_swap_control")) {
        PFNGLXSWAPINTERVALMESAPROC glXSwapIntervalMESA =
            (PFNGLXSWAPINTERVALMESAPROC)glXGetProcAddressARB((const GLubyte*)"glXSwapIntervalMESA");
        if (glXSwapIntervalMESA) {
            glXSwapIntervalMESA(1);
        }
    } else if (hasGLXExtension(display, "GLX_SGI_swap_control")) {
        PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI =
            (PFNGLXSWAPINTERVALSGIPROC)glXGetProcAddressARB((const GLubyte*)"glXSwapIntervalSGI");
        if (glXSwapIntervalSGI) {
            glXSwapIntervalSGI(1);
        }
    }
}

static void mgHideCursor(MgInternal* win) {
    Cursor invisibleCursor;
    Pixmap bitmapNoData;
    XColor black;
    static char noData[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    black.red = black.green = black.blue = 0;

    bitmapNoData = XCreateBitmapFromData(win->dpy, win->win, noData, 8, 8);
    invisibleCursor = XCreatePixmapCursor(win->dpy, bitmapNoData, bitmapNoData, &black, &black, 0, 0);
    XDefineCursor(win->dpy, win->win, invisibleCursor);
    XFreeCursor(win->dpy, invisibleCursor);
    XFreePixmap(win->dpy, bitmapNoData);
}

typedef struct {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long inputMode;
    unsigned long status;
} WindowHints;

MgSurface* mgWindow(int w, int h, const char* title, int flags) {
    MgSurface* surface = 0;
    Colormap cmap;
    XSetWindowAttributes swa;
    Window xwin;
    GLXContext glc;
    XIC ic;
    int scale;

    initX11Stuff();

    if (flags & MG_AUTO) {
        // Always use a 1:1 pixel size, unless downscaled by mgEnforceScale below.
        scale = 1;
    } else {
        // See how big we can make it and still fit on-screen.
        Screen* screen = DefaultScreenOfDisplay(dpy);
        int maxW = WidthOfScreen(screen);
        int maxH = HeightOfScreen(screen);
        scale = mgCalcScale(w, h, maxW, maxH);
    }

    scale = mgEnforceScale(scale, flags);

    cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = StructureNotifyMask;

    // Create window of wanted size
    xwin = XCreateWindow(dpy, root, 0, 0, w * scale, h * scale, 0, vi->depth, InputOutput, vi->visual,
                         CWColormap | CWEventMask, &swa);
    XMapWindow(dpy, xwin);

    if (flags & MG_FULLSCREEN) {
        // https://superuser.com/questions/1680077/does-x11-actually-have-a-native-fullscreen-mode
        Atom wm_state   = XInternAtom (dpy, "_NET_WM_STATE", true );
        Atom wm_fullscreen = XInternAtom (dpy, "_NET_WM_STATE_FULLSCREEN", true );
        XChangeProperty(dpy, xwin, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&wm_fullscreen, 1);
    } else {
        // Wait for window to get mapped
        for (;;) {
            XEvent e;
            XNextEvent(dpy, &e);
            if (e.type == MapNotify) {
                break;
            }
        }

        // Reset size if we did not get the window size we wanted above.
        XWindowAttributes wa;
        XGetWindowAttributes(dpy, xwin, &wa);
        scale = mgCalcScale(w, h, wa.width, wa.height);
        scale = mgEnforceScale(scale, flags);
        XResizeWindow(dpy, xwin, w * scale, h * scale);
    }

    XTextProperty prop;
    int result = Xutf8TextListToTextProperty(dpy, (char**)&title, 1, XUTF8StringStyle, &prop);
    if (result == Success) {
        Atom wmName = XInternAtom(dpy, "_NET_WM_NAME", 0);
        XSetTextProperty(dpy, xwin, &prop, wmName);
        XFree(prop.value);
    }

    ic = XCreateIC(inputMethod, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, XNClientWindow, xwin, NULL);
    if (ic == NULL) {
        printf("Failed to create input context\n");
        exit(0);
    }
    XSetICFocus(ic);

    XSetWMProtocols(dpy, xwin, &wmDeleteMessage, 1);

    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    int contextAttributes[] = { GLX_CONTEXT_MAJOR_VERSION_ARB, 3, GLX_CONTEXT_MINOR_VERSION_ARB, 3, None };
    glc = glXCreateContextAttribsARB(dpy, fbConfig, NULL, GL_TRUE, contextAttributes);
    glXMakeCurrent(dpy, xwin, glc);

    setupVSync(dpy, xwin);

    surface = mgBitmap2(w, h, sizeof(MgInternal));
    surface->handle = (void*)xwin;

    MgInternal* win = mgInternal(surface);
    win->win = xwin;
    win->dpy = dpy;
    win->glc = glc;
    win->ic = ic;

    win->shown = 0;
    win->closed = 0;
    win->scale = scale;

    win->lastChar = 0;
    win->flags = flags;
    win->p1 = win->p2 = win->p3 = 0;
    win->p4 = 1;
    win->widgetsWanted = 0;
    win->widgetAlpha = 0;
    win->widgetsScale = 0;
    win->widgets = 0;
    win->gl.gl_legacy = 0;

    memset(win->keys, 0, 256);
    memset(win->prev, 0, 256);

    if (flags & MG_NOCURSOR) {
        mgHideCursor(win);
    }

    mgPosition(surface, win->scale, surface->w, surface->h, win->pos);
    mgGAPICreate(surface);
    mgGAPIBegin(surface);

    return surface;
}

int mgClosed(MgSurface* surface) {
    MgInternal* win = mgInternal(surface);
    return win->win == 0;
}

int mgGAPIBegin(MgSurface* surface) {
    MgSurfaceInternal* win = mgInternal(surface);
    return glXMakeCurrent(win->dpy, win->win, win->glc) ? 0 : -1;
}

int mgGAPIEnd(MgSurface* surface) {
    (void)surface;
    return glXMakeCurrent(NULL, 0, 0) ? 0 : -1;
}

int mgKeyDown(MgSurface* surface, int key) {
    MgSurfaceInternal* win;
    assert(key < 256);
    win = mgInternal(surface);
    return win->keys[key] && !win->prev[key];
}

int mgKeyHeld(MgSurface* surface, int key) {
    MgSurfaceInternal* win;
    assert(key < 256);
    win = mgInternal(surface);
    return win->keys[key];
}

int mgReadChar(MgSurface* surface) {
    MgSurfaceInternal* win = mgInternal(surface);
    int c = win->lastChar;
    win->lastChar = 0;
    return c;
}

uint8_t mgKeyFromX11(KeySym sym) {
    if (sym >= 'a' && sym <= 'z') {
        return (uint8_t)sym - ('a' - 'A');
    }

    if (sym >= '0' && sym <= '9') {
        return (uint8_t)sym;
    }

    switch (sym) {
        case XK_KP_0:
            return MK_PAD0;
        case XK_KP_1:
            return MK_PAD1;
        case XK_KP_2:
            return MK_PAD2;
        case XK_KP_3:
            return MK_PAD3;
        case XK_KP_4:
            return MK_PAD4;
        case XK_KP_5:
            return MK_PAD5;
        case XK_KP_6:
            return MK_PAD6;
        case XK_KP_7:
            return MK_PAD7;
        case XK_KP_8:
            return MK_PAD8;
        case XK_KP_9:
            return MK_PAD9;

        case XK_KP_Multiply:
            return MK_PADMUL;
        case XK_KP_Divide:
            return MK_PADDIV;
        case XK_KP_Add:
            return MK_PADADD;
        case XK_KP_Subtract:
            return MK_PADSUB;
        case XK_KP_Decimal:
            return MK_PADDOT;
        case XK_KP_Enter:
            return MK_PADENTER;

        case XK_F1:
            return MK_F1;
        case XK_F2:
            return MK_F2;
        case XK_F3:
            return MK_F3;
        case XK_F4:
            return MK_F4;
        case XK_F5:
            return MK_F5;
        case XK_F6:
            return MK_F6;
        case XK_F7:
            return MK_F7;
        case XK_F8:
            return MK_F8;
        case XK_F9:
            return MK_F9;
        case XK_F10:
            return MK_F10;
        case XK_F11:
            return MK_F11;
        case XK_F12:
            return MK_F12;

        case XK_BackSpace:
            return MK_BACKSPACE;
        case XK_Tab:
            return MK_TAB;
        case XK_Return:
            return MK_RETURN;
        case XK_Pause:
            return MK_PAUSE;
        case XK_Caps_Lock:
            return MK_CAPSLOCK;
        case XK_Escape:
            return MK_ESCAPE;
        case XK_space:
            return MK_SPACE;

        case XK_Page_Up:
            return MK_PAGEUP;
        case XK_Page_Down:
            return MK_PAGEDN;
        case XK_End:
            return MK_END;
        case XK_Home:
            return MK_HOME;
        case XK_Left:
            return MK_LEFT;
        case XK_Up:
            return MK_UP;
        case XK_Right:
            return MK_RIGHT;
        case XK_Down:
            return MK_DOWN;
        case XK_Insert:
            return MK_INSERT;
        case XK_Delete:
            return MK_DELETE;

        case XK_Meta_L:
            return MK_LWIN;
        case XK_Meta_R:
            return MK_RWIN;
        case XK_Num_Lock:
            return MK_NUMLOCK;
        case XK_Scroll_Lock:
            return MK_SCROLL;
        case XK_Shift_L:
            return MK_LSHIFT;
        case XK_Shift_R:
            return MK_RSHIFT;
        case XK_Control_L:
            return MK_LCONTROL;
        case XK_Control_R:
            return MK_RCONTROL;
        case XK_Alt_L:
            return MK_LALT;
        case XK_Alt_R:
            return MK_RALT;

        case XK_semicolon:
            return MK_SEMICOLON;
        case XK_equal:
            return MK_EQUALS;
        case XK_comma:
            return MK_COMMA;
        case XK_minus:
            return MK_MINUS;
        case XK_period:
            return MK_DOT;
        case XK_slash:
            return MK_SLASH;
        case XK_grave:
            return MK_BACKTICK;
        case XK_bracketleft:
            return MK_LSQUARE;
        case XK_backslash:
            return MK_BACKSLASH;
        case XK_bracketright:
            return MK_RSQUARE;
        case XK_apostrophe:
            return MK_TICK;
    }
    return 0;
}

static void mgUpdateModifiers(MgSurfaceInternal* win) {
    win->keys[MK_SHIFT] = win->keys[MK_LSHIFT] || win->keys[MK_RSHIFT];
    win->keys[MK_CONTROL] = win->keys[MK_LCONTROL] || win->keys[MK_RCONTROL];
    win->keys[MK_ALT] = win->keys[MK_LALT] || win->keys[MK_RALT];
}

static void mgInterpretChar(MgSurfaceInternal* win, Window root, unsigned int keycode, unsigned int mask) {
    XKeyEvent event;
    memset(&event, 0, sizeof(event));
    event.type = KeyPress;
    event.display = win->dpy;
    event.root = root;
    event.window = win->win;
    event.state = mask;
    event.keycode = keycode;
    char inputTextUTF8[10];
    Status status = 0;
    Xutf8LookupString(win->ic, &event, inputTextUTF8, sizeof(inputTextUTF8), NULL, &status);

    if (status == XLookupChars) {
        mgDecodeUTF8(inputTextUTF8, &win->lastChar);
    }
}

static void mgProcessInput(MgSurfaceInternal* win, int winWidth, int winHeight) {
    {
        Window focused;
        int revertTo;
        XGetInputFocus(win->dpy, &focused, &revertTo);

        if (win->win != focused) {
            return;
        }
    }

    Window root;
    Window child;
    int rootX;
    int rootY;
    int winX;
    int winY;
    unsigned int mask;

    if (XQueryPointer(win->dpy, win->win, &root, &child, &rootX, &rootY, &winX, &winY, &mask)) {
        static unsigned int prevButtons;
        unsigned int buttons = mask & (Button1Mask | Button2Mask | Button3Mask);

        win->mouseX = (winX - win->pos[0]) / win->scale;
        win->mouseY = (winY - win->pos[1]) / win->scale;

        if (buttons != prevButtons && (winX > 0 && winX < winWidth) && (winY > 0 && winY < winHeight)) {
            win->mouseButtons = (buttons & Button1Mask)       ? 1
                                : 0 | (buttons & Button3Mask) ? 2
                                : 0 | (buttons & Button2Mask) ? 4
                                                              : 0;
        }
        prevButtons = buttons;
    }

    static char prevKeys[32];
    char keys[32];
    XQueryKeymap(win->dpy, keys);
    for (int i = 0; i < 32; i++) {
        char thisBlock = keys[i];
        char prevBlock = prevKeys[i];
        if (thisBlock != prevBlock) {
            for (int j = 0; j < 8; j++) {
                int thisBit = thisBlock & 1;
                int prevBit = prevBlock & 1;
                thisBlock >>= 1;
                prevBlock >>= 1;
                if (thisBit != prevBit) {
                    int keyCode = 8 * i + j;
                    KeySym keySym = XkbKeycodeToKeysym(win->dpy, keyCode, 0, 0);
                    if (keySym != NoSymbol) {
                        int key = mgKeyFromX11(keySym);
                        win->keys[key] = thisBit;
                        mgUpdateModifiers(win);

                        if (thisBit) {
                            mgInterpretChar(win, root, keyCode, mask);
                        }
                    }
                }
            }
        }
    }
    memcpy(prevKeys, keys, 32);

    XEvent event;
    while (XCheckTypedWindowEvent(win->dpy, win->win, ClientMessage, &event)) {
        if (event.xclient.data.l[0] == wmDeleteMessage) {
            glXMakeCurrent(win->dpy, None, NULL);
            glXDestroyContext(win->dpy, win->glc);
            XDestroyWindow(win->dpy, win->win);
            win->win = 0;
        }
    }
    XFlush(win->dpy);
}

void mgUpdate(MgSurface* surface) {
    XWindowAttributes gwa;

    MgSurfaceInternal* win = mgInternal(surface);

    memcpy(win->prev, win->keys, 256);

    XGetWindowAttributes(win->dpy, win->win, &gwa);

    if (win->flags & MG_AUTO)
        mgResize(surface, gwa.width / win->scale, gwa.height / win->scale);
    else
        win->scale = mgEnforceScale(mgCalcScale(surface->w, surface->h, gwa.width, gwa.height), win->flags);

    mgPosition(surface, win->scale, gwa.width, gwa.height, win->pos);
    glXMakeCurrent(win->dpy, win->win, win->glc);
    mgGAPIPresent(surface, gwa.width, gwa.height);
    glXSwapBuffers(win->dpy, win->win);

    mgProcessInput(win, gwa.width, gwa.height);
}

void mgFree(MgSurface* surface) {
    if (surface->handle) {
        MgSurfaceInternal* win = mgInternal(surface);
        if (win->win) {
            glXMakeCurrent(win->dpy, None, NULL);
            glXDestroyContext(win->dpy, win->glc);
            XDestroyWindow(win->dpy, win->win);
            win->win = 0;
        }
    }
    free(surface->pix);
    free(surface);
}

void mgError(MgSurface* surface, const char* message, ...) {
    char tmp[1024];

    va_list args;
    va_start(args, message);
    vsnprintf(tmp, sizeof(tmp), message, args);
    tmp[sizeof(tmp) - 1] = 0;
    va_end(args);

    printf("minigfx fatal error: %s\n", tmp);

    exit(1);
}

float mgTime() {
    static double lastTime = 0;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    double now = (double)tv.tv_sec + (tv.tv_usec / 1000000.0);
    double elapsed = lastTime == 0 ? 0 : now - lastTime;
    lastTime = now;

    return (float)elapsed;
}

void mgMouse(MgSurface* surface, int* x, int* y, int* buttons) {
    MgSurfaceInternal* win = mgInternal(surface);
    if (x) {
        *x = win->mouseX;
    }
    if (y) {
        *y = win->mouseY;
    }
    if (buttons) {
        *buttons = win->mouseButtons;
    }
}

int mgTouch(MgSurface* surface, MgSurfaceTouchPoint* points, int maxPoints) {
    int buttons = 0;
    if (maxPoints > 0) {
        mgMouse(surface, &points->x, &points->y, &buttons);
    }
    return buttons ? 1 : 0;
}

#endif  // __linux__


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy End  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^