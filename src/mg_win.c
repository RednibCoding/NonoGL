#include "mg_internal.h"


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy Start  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


#include <assert.h>


// not really windows stuff
MgInternal* mgInternal(MgSurface* surface) {
    assert(surface->handle);
    return (MgInternal*)(surface + 1);
}

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#pragma comment(lib, "opengl32")  // glViewport
#pragma comment(lib, "shell32")   // CommandLineToArgvW
#pragma comment(lib, "user32")    // SetWindowLong
#pragma comment(lib, "gdi32")     // ChoosePixelFormat
#pragma comment(lib, "advapi32")  // RegSetValueEx

#define WIDGET_SCALE 3
#define WIDGET_FADE 16

int main(int argc, char* argv[]);

#ifndef MG_DO_NOT_PRESERVE_WINDOW_POSITION
HKEY mgRegKey;
#endif

#ifdef __TINYC__
#define CP_UTF8 65001
int WINAPI MultiByteToWideChar();
int WINAPI WideCharToMultiByte();
#endif

static wchar_t* unicode(const char* str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
    wchar_t* dest = (wchar_t*)malloc(sizeof(wchar_t) * len);
    MultiByteToWideChar(CP_UTF8, 0, str, -1, dest, len);
    return dest;
}

void mgError(MgSurface* surface, const char* message, ...) {
    char tmp[1024];

    va_list args;
    va_start(args, message);
    _vsnprintf(tmp, sizeof(tmp), message, args);
    tmp[sizeof(tmp) - 1] = 0;
    va_end(args);

    MessageBoxW(surface ? (HWND)surface->handle : NULL, unicode(tmp), surface ? mgInternal(surface)->wtitle : L"Error",
                MB_OK | MB_ICONERROR);
    exit(1);
}

void mgEnterBorderlessWindowed(MgSurface* surface) {
    // Enter borderless windowed mode.
    MONITORINFO mi = { sizeof(mi) };
    MgInternal* win = mgInternal(surface);

    GetWindowRect((HWND)surface->handle, &win->oldPos);

    GetMonitorInfo(MonitorFromWindow((HWND)surface->handle, MONITOR_DEFAULTTONEAREST), &mi);
    win->dwStyle = WS_VISIBLE | WS_POPUP;
    SetWindowLong((HWND)surface->handle, GWL_STYLE, win->dwStyle);
    SetWindowPos((HWND)surface->handle, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                 mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, 0);
}

void mgLeaveBorderlessWindowed(MgSurface* surface) {
    MgInternal* win = mgInternal(surface);

    win->dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
    SetWindowLong((HWND)surface->handle, GWL_STYLE, win->dwStyle);

    SetWindowPos((HWND)surface->handle, NULL, win->oldPos.left, win->oldPos.top, win->oldPos.right - win->oldPos.left,
                 win->oldPos.bottom - win->oldPos.top, 0);
}

void mgWinUpdateWidgets(MgSurface* surface, int dw, int dh) {
    POINT pt;
    int i, x, clicked = 0;
    char str[8];
    MgPixel col;
    MgPixel off = mgRGB(255, 255, 255);
    MgPixel on = mgRGB(0, 200, 255);
    MgInternal* win = mgInternal(surface);
    (void)dh;

    mgClear(win->widgets, mgRGBA(0, 0, 0, 0));

    if (!(win->dwStyle & WS_POPUP)) {
        win->widgetsWanted = 0;
        win->widgetAlpha = 0;
        return;
    }

    // See if we want to be showing widgets or not.
    GetCursorPos(&pt);
    ScreenToClient((HWND)surface->handle, &pt);
    if (pt.y == 0)
        win->widgetsWanted = 1;
    if (pt.y > win->widgets->h * WIDGET_SCALE)
        win->widgetsWanted = 0;

    // Track the alpha.
    if (win->widgetsWanted)
        win->widgetAlpha = (win->widgetAlpha <= 255 - WIDGET_FADE) ? win->widgetAlpha + WIDGET_FADE : 255;
    else
        win->widgetAlpha = (win->widgetAlpha >= WIDGET_FADE) ? win->widgetAlpha - WIDGET_FADE : 0;

    // Get relative coords.
    pt.x -= (dw - win->widgets->w * WIDGET_SCALE);
    pt.x /= WIDGET_SCALE;
    pt.y /= WIDGET_SCALE;

    mgClear(win->widgets, mgRGBA(0, 0, 0, win->widgetAlpha));

    // Render it.
    for (i = 0; i < 3; i++) {
        switch (i) {
            case 0:
                str[0] = '_';
                str[1] = 0;
                break;  // "_" (minimize)
            case 1:
                str[0] = 0xEF;
                str[1] = 0xBF;
                str[2] = 0xBD;
                str[3] = 0;
                break;  // "[]" (maximize)
            case 2:
                str[0] = 0xC3;
                str[1] = 0x97;
                str[2] = 0;
                break;  // "x" (close)
        }
        x = win->widgets->w + (i - 3) * 12;
        if (i == 2)
            off = mgRGB(255, 0, 0);
        if (pt.x >= x && pt.x < x + 10 && pt.y < win->widgets->h) {
            col = on;
            if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
                clicked |= 1 << i;
        } else {
            col = off;
        }
        col.a = win->widgetAlpha;
        mgPrint(win->widgets, mgfont, x, 2, col, str);
    }

    if (clicked & 1)
        ShowWindow((HWND)surface->handle, SW_MINIMIZE);
    if (clicked & 2)
        mgLeaveBorderlessWindowed(surface);
    if (clicked & 4)
        SendMessage((HWND)surface->handle, WM_CLOSE, 0, 0);
}

void mgUpdate(MgSurface* surface) {
    MSG msg;
    RECT rc;
    int dw, dh;
    MgInternal* win = mgInternal(surface);

    if (!win->shown) {
        win->shown = 1;
        UpdateWindow((HWND)surface->handle);
        ShowWindow((HWND)surface->handle, SW_SHOW);
    }

    // Get the window size.
    GetClientRect((HWND)surface->handle, &rc);
    dw = rc.right - rc.left;
    dh = rc.bottom - rc.top;

    // Update the widget overlay.
    mgWinUpdateWidgets(surface, dw, dh);

    if (!mgGAPIBegin(surface)) {
        mgGAPIPresent(surface, dw, dh);
        SwapBuffers(win->gl.dc);
        mgGAPIEnd(surface);
    }

    memcpy(win->prev, win->keys, 256);

    // Run the message pump.
    while (PeekMessage(&msg, (HWND)surface->handle, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT)
            break;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

typedef BOOL(APIENTRY* PFNWGLSWAPINTERVALFARPROC_)(int);
static PFNWGLSWAPINTERVALFARPROC_ wglSwapIntervalEXT_ = 0;

int mgGAPIBegin(MgSurface* surface) {
    MgInternal* win = mgInternal(surface);

    return wglMakeCurrent(win->gl.dc, win->gl.hglrc) ? 0 : -1;
}

int mgGAPIEnd(MgSurface* surface) {
    (void)surface;
    return wglMakeCurrent(NULL, NULL) ? 0 : -1;
}

static BOOL UnadjustWindowRectEx(LPRECT prc, DWORD dwStyle, BOOL fMenu, DWORD dwExStyle) {
    BOOL fRc;
    RECT rc;
    SetRectEmpty(&rc);
    fRc = AdjustWindowRectEx(&rc, dwStyle, fMenu, dwExStyle);
    if (fRc) {
        prc->left -= rc.left;
        prc->top -= rc.top;
        prc->right -= rc.right;
        prc->bottom -= rc.bottom;
    }
    return fRc;
}

LRESULT CALLBACK mgWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    MgSurface* surface;
    MgInternal* win = NULL;
    RECT rc;
    int dw, dh;

    GetClientRect(hWnd, &rc);
    dw = rc.right - rc.left;
    dh = rc.bottom - rc.top;

    surface = (MgSurface*)GetPropW(hWnd, L"minigfx");
    if (surface)
        win = mgInternal(surface);

    switch (message) {
        case WM_PAINT:
            if (!mgGAPIBegin(surface)) {
                mgGAPIPresent(surface, dw, dh);
                SwapBuffers(win->gl.dc);
                mgGAPIEnd(surface);
            }
            ValidateRect(hWnd, NULL);
            break;
        case WM_CLOSE:
            if (win)
                win->closed = 1;
            break;
        case WM_GETMINMAXINFO:
            if (surface) {
                MINMAXINFO* info = (MINMAXINFO*)lParam;
                RECT rc;
                rc.left = 0;
                rc.top = 0;
                if (win->flags & MG_AUTO) {
                    rc.right = 32;
                    rc.bottom = 32;
                } else {
                    int minscale = mgEnforceScale(1, win->flags);
                    rc.right = surface->w * minscale;
                    rc.bottom = surface->h * minscale;
                }
                AdjustWindowRectEx(&rc, win->dwStyle, FALSE, 0);
                info->ptMinTrackSize.x = rc.right - rc.left;
                info->ptMinTrackSize.y = rc.bottom - rc.top;
            }
            return 0;
        case WM_SIZING:
            if (win) {
                // Calculate scale-constrained sizes.
                RECT* rc = (RECT*)lParam;
                int dx, dy;
                UnadjustWindowRectEx(rc, win->dwStyle, FALSE, 0);
                dx = (rc->right - rc->left) % win->scale;
                dy = (rc->bottom - rc->top) % win->scale;
                switch (wParam) {
                    case WMSZ_LEFT:
                        rc->left += dx;
                        break;
                    case WMSZ_RIGHT:
                        rc->right -= dx;
                        break;
                    case WMSZ_TOP:
                        rc->top += dy;
                        break;
                    case WMSZ_TOPLEFT:
                        rc->left += dx;
                        rc->top += dy;
                        break;
                    case WMSZ_TOPRIGHT:
                        rc->right -= dx;
                        rc->top += dy;
                        break;
                    case WMSZ_BOTTOM:
                        rc->bottom -= dy;
                        break;
                    case WMSZ_BOTTOMLEFT:
                        rc->left += dx;
                        rc->bottom -= dy;
                        break;
                    case WMSZ_BOTTOMRIGHT:
                        rc->right -= dx;
                        rc->bottom -= dy;
                        break;
                }
                AdjustWindowRectEx(rc, win->dwStyle, FALSE, 0);
            }
            return TRUE;
        case WM_SIZE:
            if (win) {
                if (wParam != SIZE_MINIMIZED) {
                    // Detect window size changes and update our bitmap accordingly.
                    dw = LOWORD(lParam);
                    dh = HIWORD(lParam);
                    if (win->flags & MG_AUTO) {
                        mgResize(surface, dw / win->scale, dh / win->scale);
                    } else {
                        win->scale = mgEnforceScale(mgCalcScale(surface->w, surface->h, dw, dh), win->flags);
                    }
                    mgPosition(surface, win->scale, dw, dh, win->pos);
                }

            #ifndef MG_DO_NOT_MAXIMIZE_TO_FULLSCREEN
                // If someone tried to maximize us (e.g. via shortcut launch options),
                // prefer instead to be borderless.
                if (wParam == SIZE_MAXIMIZED) {
                    ShowWindow((HWND)surface->handle, SW_NORMAL);
                    mgEnterBorderlessWindowed(surface);
                }
            #endif
            }
            return 0;
#ifndef MG_DO_NOT_PRESERVE_WINDOW_POSITION
        case WM_WINDOWPOSCHANGED: {
            // Save our position.
            WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
            GetWindowPlacement(hWnd, &wp);
            if (win->dwStyle & WS_POPUP)
                wp.showCmd = SW_MAXIMIZE;
            RegSetValueExW(mgRegKey, win->wtitle, 0, REG_BINARY, (BYTE*)&wp, sizeof(wp));
            return DefWindowProcW(hWnd, message, wParam, lParam);
        }
#endif
        case WM_ACTIVATE:
            if (win) {
                memset(win->keys, 0, 256);
                memset(win->prev, 0, 256);
                win->lastChar = 0;
            }
            return 0;
        case WM_CHAR:
            if (win) {
                if (wParam == '\r') {
                    wParam = '\n';
                }
                int repeating = (HIWORD(lParam) & KF_REPEAT) == KF_REPEAT;
                if (!repeating) {
                    win->lastChar = wParam;
                }
            }
            return DefWindowProcW(hWnd, message, wParam, lParam);
        case WM_MENUCHAR:
            // Disable beep on Alt+Enter
            if (LOWORD(wParam) == VK_RETURN)
                return MNC_CLOSE << 16;
            return DefWindowProcW(hWnd, message, wParam, lParam);
        case WM_SYSKEYDOWN:
            if (win) {
                if (wParam == VK_RETURN) {
                    // Alt+Enter
                    if (win->dwStyle & WS_POPUP)
                        mgLeaveBorderlessWindowed(surface);
                    else
                        mgEnterBorderlessWindowed(surface);
                    return 0;
                }
            }
            // fall-thru
        case WM_KEYDOWN:
            if (win)
                win->keys[wParam] = 1;
            return DefWindowProcW(hWnd, message, wParam, lParam);
        case WM_SYSKEYUP:
            // fall-thru
        case WM_KEYUP:
            if (win)
                win->keys[wParam] = 0;
            return DefWindowProcW(hWnd, message, wParam, lParam);
        default:
            return DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

MgSurface* mgWindow(int w, int h, const char* title, int flags) {
    WNDCLASSEXW wcex = { 0 };
    int maxW, maxH, scale;
    HWND hWnd;
    DWORD dwStyle;
    RECT rc;
    DWORD err;
    MgSurface* surface;
    MgInternal* win;
#ifndef MG_DO_NOT_PRESERVE_WINDOW_POSITION
    WINDOWPLACEMENT wp;
    DWORD wpsize = sizeof(wp);
#endif

    wchar_t* wtitle = unicode(title);

// Find our registry key.
#ifndef MG_DO_NOT_PRESERVE_WINDOW_POSITION
    RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\minigfx", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
                    &mgRegKey, NULL);
#endif

    // Register a window class.
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = mgWndProc;
    wcex.hInstance = GetModuleHandle(NULL);
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName = L"minigfx";
    RegisterClassExW(&wcex);

    if (flags & MG_AUTO) {
        // Always use a 1:1 pixel size.
        scale = 1;
    } else {
        // See how big we can make it and still fit on-screen.
        maxW = GetSystemMetrics(SM_CXSCREEN) * 3 / 4;
        maxH = GetSystemMetrics(SM_CYSCREEN) * 3 / 4;
        scale = mgCalcScale(w, h, maxW, maxH);
    }

    scale = mgEnforceScale(scale, flags);

    // Get the final window size.
    dwStyle = WS_OVERLAPPEDWINDOW;
    rc.left = 0;
    rc.top = 0;
    rc.right = w * scale;
    rc.bottom = h * scale;
    AdjustWindowRect(&rc, dwStyle, FALSE);

    // Make a window.
    hWnd = CreateWindowW(L"minigfx", wtitle, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
                         NULL, NULL, wcex.hInstance, NULL);
    err = GetLastError();
    if (!hWnd)
        ExitProcess(1);

    if (flags & MG_NOCURSOR) {
        ShowCursor(FALSE);
    }

    // Wrap a bitmap around it.
    surface = mgBitmap2(w, h, sizeof(MgInternal));
    surface->handle = hWnd;

    // Set up the Windows parts.
    win = mgInternal(surface);
    win->dwStyle = dwStyle;
    win->wtitle = wtitle;
    win->shown = 0;
    win->closed = 0;
    win->scale = scale;
    win->lastChar = 0;
    win->flags = flags;

    win->p1 = win->p2 = win->p3 = 0;
    win->p4 = 1;

    win->widgetsWanted = 0;
    win->widgetAlpha = 0;
    win->widgetsScale = WIDGET_SCALE;
    win->widgets = mgBitmap(40, 14);

    SetPropW(hWnd, L"minigfx", surface);

    mgGAPICreate(surface);

    if (flags & MG_FULLSCREEN) {
        mgEnterBorderlessWindowed(surface);
    } else {
// Try and restore our window position.
#ifndef MG_DO_NOT_PRESERVE_WINDOW_POSITION
        if (RegQueryValueExW(mgRegKey, wtitle, NULL, NULL, (BYTE*)&wp, &wpsize) == ERROR_SUCCESS) {
            if (wp.showCmd == SW_MAXIMIZE)
                mgEnterBorderlessWindowed(surface);
            else
                SetWindowPlacement(hWnd, &wp);
        }
#endif
    }

    wglSwapIntervalEXT_ = (PFNWGLSWAPINTERVALFARPROC_)wglGetProcAddress("wglSwapIntervalEXT");
    if (wglSwapIntervalEXT_)
        wglSwapIntervalEXT_(1);

    return surface;
}

void mgFree(MgSurface* surface) {
    if (surface->handle) {
        MgInternal* win = mgInternal(surface);
        mgGAPIDestroy(surface);

        if (win->gl.hglrc && !wglDeleteContext(win->gl.hglrc)) {
            mgError(surface, "Cannot delete OpenGL context.\n");
        }
        win->gl.hglrc = NULL;

        if (win->gl.dc && !ReleaseDC((HWND)surface->handle, win->gl.dc)) {
            mgError(surface, "Cannot release OpenGL device context.\n");
        }
        win->gl.dc = NULL;

        DestroyWindow((HWND)surface->handle);
        free(win->wtitle);
        mgFree(win->widgets);
    }
    free(surface->pixels);
    free(surface);
}

int mgClosed(MgSurface* surface) {
    MgInternal* win = mgInternal(surface);
    int val = win->closed;
    win->closed = 0;
    return val;
}

float mgTime() {
    static int first = 1;
    static LARGE_INTEGER prev;

    LARGE_INTEGER cnt, freq;
    ULONGLONG diff;
    QueryPerformanceCounter(&cnt);
    QueryPerformanceFrequency(&freq);

    if (first) {
        first = 0;
        prev = cnt;
    }

    diff = cnt.QuadPart - prev.QuadPart;
    prev = cnt;
    return (float)(diff / (double)freq.QuadPart);
}

void mgMouse(MgSurface* surface, int* x, int* y, int* buttons) {
    POINT pt;
    MgInternal* win;

    win = mgInternal(surface);
    GetCursorPos(&pt);
    ScreenToClient((HWND)surface->handle, &pt);
    *x = (pt.x - win->pos[0]) / win->scale;
    *y = (pt.y - win->pos[1]) / win->scale;
    *buttons = 0;
    if (GetFocus() != surface->handle)
        return;
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
        *buttons |= 1;
    if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
        *buttons |= 2;
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
        *buttons |= 4;
}

int mgTouch(MgSurface* surface, MgTouchPoint* points, int maxPoints) {
    int buttons = 0;
    if (maxPoints > 0) {
        mgMouse(surface, &points[0].x, &points[1].y, &buttons);
    }
    return buttons ? 1 : 0;
}

static int mgWinVK(int key) {
    if (key >= 'A' && key <= 'Z')
        return key;
    if (key >= '0' && key <= '9')
        return key;
    switch (key) {
        case MK_BACKSPACE:
            return VK_BACK;
        case MK_TAB:
            return VK_TAB;
        case MK_RETURN:
            return VK_RETURN;
        case MK_SHIFT:
            return VK_SHIFT;
        case MK_CONTROL:
            return VK_CONTROL;
        case MK_ALT:
            return VK_MENU;
        case MK_PAUSE:
            return VK_PAUSE;
        case MK_CAPSLOCK:
            return VK_CAPITAL;
        case MK_ESCAPE:
            return VK_ESCAPE;
        case MK_SPACE:
            return VK_SPACE;
        case MK_PAGEUP:
            return VK_PRIOR;
        case MK_PAGEDN:
            return VK_NEXT;
        case MK_END:
            return VK_END;
        case MK_HOME:
            return VK_HOME;
        case MK_LEFT:
            return VK_LEFT;
        case MK_UP:
            return VK_UP;
        case MK_RIGHT:
            return VK_RIGHT;
        case MK_DOWN:
            return VK_DOWN;
        case MK_INSERT:
            return VK_INSERT;
        case MK_DELETE:
            return VK_DELETE;
        case MK_LWIN:
            return VK_LWIN;
        case MK_RWIN:
            return VK_RWIN;
        // case MK_APPS: return VK_APPS; // this key doesn't exist on OS X
        case MK_PAD0:
            return VK_NUMPAD0;
        case MK_PAD1:
            return VK_NUMPAD1;
        case MK_PAD2:
            return VK_NUMPAD2;
        case MK_PAD3:
            return VK_NUMPAD3;
        case MK_PAD4:
            return VK_NUMPAD4;
        case MK_PAD5:
            return VK_NUMPAD5;
        case MK_PAD6:
            return VK_NUMPAD6;
        case MK_PAD7:
            return VK_NUMPAD7;
        case MK_PAD8:
            return VK_NUMPAD8;
        case MK_PAD9:
            return VK_NUMPAD9;
        case MK_PADMUL:
            return VK_MULTIPLY;
        case MK_PADADD:
            return VK_ADD;
        case MK_PADENTER:
            return VK_SEPARATOR;
        case MK_PADSUB:
            return VK_SUBTRACT;
        case MK_PADDOT:
            return VK_DECIMAL;
        case MK_PADDIV:
            return VK_DIVIDE;
        case MK_F1:
            return VK_F1;
        case MK_F2:
            return VK_F2;
        case MK_F3:
            return VK_F3;
        case MK_F4:
            return VK_F4;
        case MK_F5:
            return VK_F5;
        case MK_F6:
            return VK_F6;
        case MK_F7:
            return VK_F7;
        case MK_F8:
            return VK_F8;
        case MK_F9:
            return VK_F9;
        case MK_F10:
            return VK_F10;
        case MK_F11:
            return VK_F11;
        case MK_F12:
            return VK_F12;
        case MK_NUMLOCK:
            return VK_NUMLOCK;
        case MK_SCROLL:
            return VK_SCROLL;
        case MK_LSHIFT:
            return VK_LSHIFT;
        case MK_RSHIFT:
            return VK_RSHIFT;
        case MK_LCONTROL:
            return VK_LCONTROL;
        case MK_RCONTROL:
            return VK_RCONTROL;
        case MK_LALT:
            return VK_LMENU;
        case MK_RALT:
            return VK_RMENU;
        case MK_SEMICOLON:
            return VK_OEM_1;
        case MK_EQUALS:
            return VK_OEM_PLUS;
        case MK_COMMA:
            return VK_OEM_COMMA;
        case MK_MINUS:
            return VK_OEM_MINUS;
        case MK_DOT:
            return VK_OEM_PERIOD;
        case MK_SLASH:
            return VK_OEM_2;
        case MK_BACKTICK:
            return VK_OEM_3;
        case MK_LSQUARE:
            return VK_OEM_4;
        case MK_BACKSLASH:
            return VK_OEM_5;
        case MK_RSQUARE:
            return VK_OEM_6;
        case MK_TICK:
            return VK_OEM_7;
    }
    return 0;
}

int mgKeyDown(MgSurface* surface, int key) {
    MgInternal* win;
    int k = mgWinVK(key);
    if (GetFocus() != surface->handle)
        return 0;
    win = mgInternal(surface);
    return win->keys[k] && !win->prev[k];
}

int mgKeyHeld(MgSurface* surface, int key) {
    MgInternal* win;
    int k = mgWinVK(key);
    if (GetFocus() != surface->handle)
        return 0;
    win = mgInternal(surface);
    return win->keys[k];
}

int mgReadChar(MgSurface* surface) {
    MgInternal* win = mgInternal(surface);
    int c = win->lastChar;
    win->lastChar = 0;
    return c;
}

// We supply our own WinMain and just chain through to the user's
// real entry point.
#ifdef UNICODE
int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
#else
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
    int n, argc;
    LPWSTR* wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
    char** argv = (char**)calloc(argc + 1, sizeof(int));

    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    for (n = 0; n < argc; n++) {
        int len = WideCharToMultiByte(CP_UTF8, 0, wargv[n], -1, 0, 0, NULL, NULL);
        argv[n] = (char*)malloc(len);
        WideCharToMultiByte(CP_UTF8, 0, wargv[n], -1, argv[n], len, NULL, NULL);
    }
    return main(argc, argv);
}
#endif // _WIN32


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy End  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^