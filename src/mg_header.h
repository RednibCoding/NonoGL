#pragma once

#ifdef __cplusplus
extern "C" {
#endif^

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy Start  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

// Comment the following line if the window position should be saved in the registry
#define MG_DO_NOT_PRESERVE_WINDOW_POSITION

// Comment the following if you want the window to maximize to fullscreen when user maximizes the window
#define MG_DO_NOT_MAXIMIZE_TO_FULLSCREEN

// Compiler configuration ----------------------------------------------------------------
    #ifdef _MSC_VER
    #define MG_INLINE static __forceinline
    #else
    #define MG_INLINE static inline
    #endif

// Bitmaps ----------------------------------------------------------------

    // This struct contains one pixel.
    typedef struct {
        unsigned char r, g, b, a;
    } MgPixel;

    // Window flags.
    #define MG_FIXED      0   // window's bitmap is a fixed size (default)
    #define MG_AUTO       1   // window's bitmap is scaled with the window
    #define MG_2X         2   // always enforce (at least) 2X pixel scale
    #define MG_3X         4   // always enforce (at least) 3X pixel scale
    #define MG_4X         8   // always enforce (at least) 4X pixel scale
    #define MG_RETINA     16  // enable retina support on OS X
    #define MG_NOCURSOR   32  // hide cursor
    #define MG_FULLSCREEN 64  // start in full-screen mode

    // A Mg bitmap.
    typedef struct MgSurface {
        int w, h;           // width/height (unscaled)
        int cx, cy, cw, ch; // clip rect
        MgPixel *pixels;        // pixel data
        void *handle;       // OS window handle, NULL for off-screen bitmaps.
        int blitMode;       // Target bitmap blit mode
    } MgSurface;

    // Creates a new empty window with a given bitmap size.
    //
    // Title is UTF-8.
    //
    // In MG_FIXED mode, the window is made as large as possible to contain an integer-scaled
    // version of the bitmap while still fitting on the screen. Resizing the window will adapt
    // the scale in integer steps to fit the bitmap.
    //
    // In MG_AUTO mode, the initial window size is set to the bitmap size times the pixel
    // scale. Resizing the window will resize the bitmap using the specified scale.
    // For example, in forced 2X mode, the window will be twice as wide (and high) as the bitmap.
    //
    // Turning on MG_RETINA mode will request full backing resolution on OSX, meaning that
    // the effective window size might be integer scaled to a larger size. In MG_AUTO mode,
    // this means that the MgSurface bitmap will change size if the window is moved between
    // retina and non-retina screens.
    //
    MgSurface *mgWindow(int w, int h, const char *title, int flags);

    // Creates an empty off-screen bitmap.
    MgSurface *mgBitmap(int w, int h);

    // Deletes a window/bitmap.
    void mgFree(MgSurface *surface);

    // Returns non-zero if the user requested to close a window.
    int mgClosed(MgSurface *surface);

    // Displays a window's contents on-screen and updates input.
    void mgUpdate(MgSurface *surface);

    // Called before doing direct OpenGL calls and before mgUpdate.
    // Returns non-zero if OpenGL is available.
    int mgBeginOpenGL(MgSurface *surface);

    // Sets post shader for a window.
    // This replaces the built-in post-FX shader.
    void mgSetPostShader(MgSurface *surface, const char* code, int size);

    // Sets post-FX properties for a window.
    //
    // The built-in post-FX shader uses the following parameters:
    // p1: hblur - use bilinear filtering along the x-axis (pixels)
    // p2: vblur - use bilinear filtering along the y-axis (pixels)
    // p3: scanlines - CRT scanlines effect (0-1)
    // p4: contrast - contrast boost (1 = no change, 2 = 2X contrast, etc)
    void mgSetPostFX(MgSurface *surface, float p1, float p2, float p3, float p4);


// Drawing ----------------------------------------------------------------

    // Helper for reading pixels.
    // For high performance, just access surface->pixels directly.
    MgPixel mgGet(MgSurface *surface, int x, int y);

    // Plots a pixel.
    // Clips and blends.
    // For high performance, just access surface->pixels directly.
    void mgPlot(MgSurface *surface, int x, int y, MgPixel pix);

    // Clears a bitmap to a color.
    // No blending, no clipping.
    void mgClear(MgSurface *surface, MgPixel color);

    // Fills a rectangular area.
    // No blending, no clipping.
    void mgFill(MgSurface *surface, int x, int y, int w, int h, MgPixel color);

    // Draws a line.
    // Start pixel is drawn, end pixel is not.
    // Clips and blends.
    void mgLine(MgSurface *surface, int x0, int y0, int x1, int y1, MgPixel color);

    // Draws an empty rectangle.
    // Drawing a 1x1 rectangle yields the same result as calling mgPlot.
    // Clips and blends.
    void mgRect(MgSurface *surface, int x, int y, int w, int h, MgPixel color);

    // Fills a rectangle.
    // Fills the inside of the specified rectangular area.
    // Calling mgRect followed by mgFillRect using the same arguments
    // causes no overdrawing.
    // Clips and blends.
    void mgFillRect(MgSurface *surface, int x, int y, int w, int h, MgPixel color);

    // Draws a circle.
    // Drawing a zero radius circle yields the same result as calling mgPlot.
    // Drawing a circle with radius one draws a circle three pixels wide.
    // Clips and blends.
    void mgCircle(MgSurface *surface, int x, int y, int r, MgPixel color);

    // Fills a circle.
    // Fills the inside of the specified circle.
    // Calling mgCircle followed by mgFillCircle using the same arguments
    // causes no overdrawing.
    // Filling a circle with zero radius has no effect.
    // Clips and blends.
    void mgFillCircle(MgSurface *surface, int x, int y, int r, MgPixel color);

    // Sets clip rect.
    // Set to (0, 0, -1, -1) to reset clipping to full bitmap.
    void mgClip(MgSurface *surface, int cx, int cy, int cw, int ch);

    // Copies bitmap data.
    // dx/dy = dest co-ordinates
    // sx/sy = source co-ordinates
    // w/h   = width/height
    //
    // RGBAdest = RGBAsrc
    // Clips, does not blend.
    void mgBlit(MgSurface *dest, MgSurface *src, int dx, int dy, int sx, int sy, int w, int h);

    // Same as mgBlit, but alpha blends the source bitmap with the
    // target using per pixel alpha and the specified global alpha.
    //
    // Ablend = Asrc * alpha
    // RGBdest = RGBsrc * Ablend + RGBdest * (1 - Ablend)
    //
    // Blit mode == MG_KEEP_ALPHA:
    // Adest = Adest
    //
    // Blit mode == MG_BLEND_ALPHA:
    // Adest = Asrc * Ablend + Adest * (1 - Ablend)
    // Clips and blends.
    void mgBlitAlpha(MgSurface *dest, MgSurface *src, int dx, int dy, int sx, int sy, int w, int h, float alpha);

    // Same as mgBlit, but tints the source bitmap with a color
    // and alpha blends the resulting source with the destination.
    //
    // Rblend = Rsrc * Rtint
    // Gblend = Gsrc * Gtint
    // Bblend = Bsrc * Btint
    // Ablend = Asrc * Atint
    //
    // RGBdest = RGBblend * Ablend + RGBdest * (1 - Ablend)
    //
    // Blit mode == MG_KEEP_ALPHA:
    // Adest = Adest
    //
    // Blit mode == MG_BLEND_ALPHA:
    // Adest = Ablend * Ablend + Adest * (1 - Ablend)
    // Clips and blends.
    void mgBlitTint(MgSurface *dest, MgSurface *src, int dx, int dy, int sx, int sy, int w, int h, MgPixel tint);

    enum MGBlitMode {
        MG_KEEP_ALPHA = 0,    // Keep destination alpha value
        MG_BLEND_ALPHA = 1,   // Blend destination alpha (default)
    };

    // Set destination bitmap blend mode for blit operations.
    void mgBlitMode(MgSurface *dest, int mode);

    // Helper for making colors.
    MG_INLINE MgPixel mgRGB(unsigned char r, unsigned char g, unsigned char b)
    {
        MgPixel p; p.r = r; p.g = g; p.b = b; p.a = 0xff; return p;
    }

    // Helper for making colors.
    MG_INLINE MgPixel mgRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        MgPixel p; p.r = r; p.g = g; p.b = b; p.a = a; return p;
    }


// Font printing ----------------------------------------------------------

    typedef struct MgGlyph {
        int code, x, y, w, h;
    } MgGlyph;

    typedef struct MgFont {
        MgSurface *surface;
        int numGlyphs;
        MgGlyph *glyphs;
    } MgFont;

    typedef enum TCodepage {
        TCP_ASCII = 0,
        TCP_1252 = 1252,
        TCP_UTF32 = 12001
    } TCodepage;

    // Loads a font.
    //
    // Codepages:
    //
    //  TCP_ASCII   - Regular 7-bit ASCII
    //  TCP_1252    - Windows 1252
    //  TCP_UTF32   - Unicode subset
    //
    // For ASCII and 1252, the font bitmap should contain all characters
    // for the given codepage, excluding the first 32 control codes.
    //
    // For UTF32 - the font bitmap contains a subset of Unicode characters
    // and must be in the format generated by mgFont for UTF32.
    //
    MgFont *mgLoadFont(MgSurface *bitmap, int codepage);

    // Frees a font.
    void mgFreeFont(MgFont *font);

    // Prints UTF-8 text onto a bitmap.
    // NOTE:
    //  This uses the target bitmap blit mode.
    //  See mgBlitTint for details.
    void mgPrint(MgSurface *dest, MgFont *font, int x, int y, MgPixel color, const char *text, ...);

    // Returns the width/height of a string.
    int mgTextWidth(MgFont *font, const char *text);
    int mgTextHeight(MgFont *font, const char *text);

    // The built-in font.
    extern MgFont *mgfont;


// User Input -------------------------------------------------------------

    // Key scancodes. For letters/numbers, use ASCII ('A'-'Z' and '0'-'9').
    typedef enum {
        MK_PAD0=128,MK_PAD1,MK_PAD2,MK_PAD3,MK_PAD4,MK_PAD5,MK_PAD6,MK_PAD7,MK_PAD8,MK_PAD9,
        MK_PADMUL,MK_PADADD,MK_PADENTER,MK_PADSUB,MK_PADDOT,MK_PADDIV,
        MK_F1,MK_F2,MK_F3,MK_F4,MK_F5,MK_F6,MK_F7,MK_F8,MK_F9,MK_F10,MK_F11,MK_F12,
        MK_BACKSPACE,MK_TAB,MK_RETURN,MK_SHIFT,MK_CONTROL,MK_ALT,MK_PAUSE,MK_CAPSLOCK,
        MK_ESCAPE,MK_SPACE,MK_PAGEUP,MK_PAGEDN,MK_END,MK_HOME,MK_LEFT,MK_UP,MK_RIGHT,MK_DOWN,
        MK_INSERT,MK_DELETE,MK_LWIN,MK_RWIN,MK_NUMLOCK,MK_SCROLL,MK_LSHIFT,MK_RSHIFT,
        MK_LCONTROL,MK_RCONTROL,MK_LALT,MK_RALT,MK_SEMICOLON,MK_EQUALS,MK_COMMA,MK_MINUS,
        MK_DOT,MK_SLASH,MK_BACKTICK,MK_LSQUARE,MK_BACKSLASH,MK_RSQUARE,MK_TICK
    } MgKey;

    // Returns mouse input for a window.
    void mgMouse(MgSurface *surface, int *x, int *y, int *buttons);

    typedef struct {
        int x;
        int y;
    } MgTouchPoint;

    // Reads touch input for a window.
    // Returns number of touch points read.
    int mgTouch(MgSurface *surface, MgTouchPoint* points, int maxPoints);

    // Reads the keyboard for a window.
    // Returns non-zero if a key is pressed/held.
    // mgKeyDown tests for the initial press, mgKeyHeld repeats each frame.
    int mgKeyDown(MgSurface *surface, int key);
    int mgKeyHeld(MgSurface *surface, int key);

    // Reads character input for a window.
    // Returns the Unicode value of the last key pressed, or 0 if none.
    int mgReadChar(MgSurface *surface);

    // Show / hide virtual keyboard.
    // (Only available on iOS / Android)
    void mgShowKeyboard(int show);


// Bitmap I/O -------------------------------------------------------------

    // Loads a PNG, from either a file or memory. (fileName is UTF-8)
    // On error, returns NULL and sets errno.
    MgSurface *mgLoadImage(const char *fileName);
    MgSurface *mgLoadImageMem(const void *data, int length);

    // Saves a PNG to a file. (fileName is UTF-8)
    // On error, returns zero and sets errno.
    int mgSaveImage(const char *fileName, MgSurface *surface);


    // Helpers ----------------------------------------------------------------

    // Returns the amount of time elapsed since mgTime was last called,
    // or zero on the first call.
    float mgTime(void);

    // Displays an error message and quits. (UTF-8)
    // 'bmp' can be NULL.
    void mgError(MgSurface *surface, const char *message, ...);

    // Reads an entire file into memory. (fileName is UTF-8)
    // Free it yourself after with 'free'.
    // On error, returns NULL and sets errno.
    // MG will automatically append a NUL terminator byte
    // to the end (not included in the length)
    void *mgReadFile(const char *fileName, int *length);

    // Decompresses DEFLATEd zip/zlib data into a buffer.
    // Returns non-zero on success.
    int mgInflate(void *out, unsigned outlen, const void *in, unsigned inlen);

    // Decodes a single UTF8 codepoint and returns the next pointer.
    const char *mgDecodeUTF8(const char *text, int *cp);

    // Encodes a single UTF8 codepoint and returns the next pointer.
    char *mgEncodeUTF8(char *text, int cp);


// Extensions ----------------------------------------------------------------

    // Returns the frames per second based on the given delta time.
    // Delta time can be received by calling 'mgTime()' once in the main loop.
    int mgFps(float dt);


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy End  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#ifdef __cplusplus
}
#endif