#include "mg_internal.h"
#include <stdlib.h>
#include <string.h>


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy Start  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


// Expands 0-255 into 0-256
#define EXPAND(X) ((X) + ((X) > 0))

#define CLIP0(CX, X, X2, W) \
    if (X < CX) {           \
        int D = CX - X;     \
        W -= D;             \
        X2 += D;            \
        X += D;             \
    }
#define CLIP1(X, DW, W) \
    if (X + W > DW)     \
        W = DW - X;
#define CLIP()                  \
    CLIP0(dst->cx, dx, sx, w);  \
    CLIP0(dst->cy, dy, sy, h);  \
    CLIP0(0, sx, dx, w);        \
    CLIP0(0, sy, dy, h);        \
    CLIP1(dx, dst->cx + cw, w); \
    CLIP1(dy, dst->cy + ch, h); \
    CLIP1(sx, src->w, w);       \
    CLIP1(sy, src->h, h);       \
    if (w <= 0 || h <= 0)       \
    return

MgSurface* mgBitmap2(int w, int h, int extra) {
    MgSurface* surface = (MgSurface*)calloc(1, sizeof(MgSurface) + extra);
    surface->w = w;
    surface->h = h;
    surface->cw = -1;
    surface->ch = -1;
    surface->pixels = (MgPixel*)calloc(w * h, sizeof(MgPixel));
    surface->blitMode = MG_BLEND_ALPHA;
    return surface;
}

MgSurface* mgBitmap(int w, int h) {
    return mgBitmap2(w, h, 0);
}


void mgResize(MgSurface* surface, int w, int h) {
    if (surface->w == w && surface->h == h) {
        return;
    }

    int y, cw, ch;
    MgPixel* newpix = (MgPixel*)calloc(w * h, sizeof(MgPixel));
    cw = (w < surface->w) ? w : surface->w;
    ch = (h < surface->h) ? h : surface->h;

    // Copy any old data across.
    for (y = 0; y < ch; y++)
        memcpy(newpix + y * w, surface->pixels + y * surface->w, cw * sizeof(MgPixel));

    free(surface->pixels);
    surface->pixels = newpix;
    surface->w = w;
    surface->h = h;
}

int mgCalcScale(int bmpW, int bmpH, int areaW, int areaH) {
    // We want it as big as possible in the window, but still
    // maintaining the correct aspect ratio, and always
    // having an integer pixel size.
    int scale = 0;
    for (;;) {
        scale++;
        if (bmpW * scale > areaW || bmpH * scale > areaH) {
            scale--;
            break;
        }
    }
    return (scale > 1) ? scale : 1;
}

int mgEnforceScale(int scale, int flags) {
    if ((flags & MG_4X) && scale < 4)
        scale = 4;
    if ((flags & MG_3X) && scale < 3)
        scale = 3;
    if ((flags & MG_2X) && scale < 2)
        scale = 2;
    return scale;
}

void mgPosition(MgSurface* surface, int scale, int windowW, int windowH, int out[4]) {
    // Center the image on screen at this scale.
    out[0] = (windowW - surface->w * scale) / 2;
    out[1] = (windowH - surface->h * scale) / 2;
    out[2] = out[0] + surface->w * scale;
    out[3] = out[1] + surface->h * scale;
}

void mgClear(MgSurface* surface, MgPixel color) {
    int count = surface->w * surface->h;
    int n;
    for (n = 0; n < count; n++)
        surface->pixels[n] = color;
}

void mgFill(MgSurface* surface, int x, int y, int w, int h, MgPixel color) {
    MgPixel* td;
    int dt, i;

    if (x < 0) {
        w += x;
        x = 0;
    }
    if (y < 0) {
        h += y;
        y = 0;
    }
    if (x + w > surface->w) {
        w = surface->w - x;
    }
    if (y + h > surface->h) {
        h = surface->h - y;
    }
    if (w <= 0 || h <= 0)
        return;

    td = &surface->pixels[y * surface->w + x];
    dt = surface->w;
    do {
        for (i = 0; i < w; i++)
            td[i] = color;
        td += dt;
    } while (--h);
}

void mgLine(MgSurface* surface, int x0, int y0, int x1, int y1, MgPixel color) {
    int sx, sy, dx, dy, err, e2;
    dx = abs(x1 - x0);
    dy = abs(y1 - y0);
    if (x0 < x1)
        sx = 1;
    else
        sx = -1;
    if (y0 < y1)
        sy = 1;
    else
        sy = -1;
    err = dx - dy;

    do {
        mgPlot(surface, x0, y0, color);
        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    } while (x0 != x1 || y0 != y1);
}

void mgFillRect(MgSurface* surface, int x, int y, int w, int h, MgPixel color) {
    x += 1;
    y += 1;
    w -= 2;
    h -= 2;

    int cx = surface->cx;
    int cy = surface->cy;
    int cw = surface->cw >= 0 ? surface->cw : surface->w;
    int ch = surface->ch >= 0 ? surface->ch : surface->h;

    if (x < cx) {
        w += (x - cx);
        x = cx;
    }
    if (y < cy) {
        h += (y - cy);
        y = cy;
    }
    if (x + w > cx + cw) {
        w -= (x + w) - (cx + cw);
    }
    if (y + h > cy + ch) {
        h -= (y + h) - (cy + ch);
    }
    if (w <= 0 || h <= 0)
        return;

    MgPixel* td = &surface->pixels[y * surface->w + x];
    int dt = surface->w;
    int xa = EXPAND(color.a);
    int a = xa * xa;

    do {
        for (int i = 0; i < w; i++) {
            td[i].r += (unsigned char)((color.r - td[i].r) * a >> 16);
            td[i].g += (unsigned char)((color.g - td[i].g) * a >> 16);
            td[i].b += (unsigned char)((color.b - td[i].b) * a >> 16);
            td[i].a += (surface->blitMode) * (unsigned char)((color.a - td[i].a) * a >> 16);
        }
        td += dt;
    } while (--h);
}

void mgRect(MgSurface* surface, int x, int y, int w, int h, MgPixel color) {
    int x1, y1;
    if (w <= 0 || h <= 0) {
        return;
    }

    if (w == 1) {
        mgLine(surface, x, y, x, y + h, color);
    } else if (h == 1) {
        mgLine(surface, x, y, x + w, y, color);
    } else {
        x1 = x + w - 1;
        y1 = y + h - 1;
        mgLine(surface, x, y, x1, y, color);
        mgLine(surface, x1, y, x1, y1, color);
        mgLine(surface, x1, y1, x, y1, color);
        mgLine(surface, x, y1, x, y, color);
    }
}

void mgFillCircle(MgSurface* surface, int x0, int y0, int r, MgPixel color) {
    if (r <= 0) {
        return;
    }

    int E = 1 - r;
    int dx = 0;
    int dy = -2 * r;
    int x = 0;
    int y = r;

    mgLine(surface, x0 - r + 1, y0, x0 + r, y0, color);

    while (x < y - 1) {
        x++;

        if (E >= 0) {
            y--;
            dy += 2;
            E += dy;
            mgLine(surface, x0 - x + 1, y0 + y, x0 + x, y0 + y, color);
            mgLine(surface, x0 - x + 1, y0 - y, x0 + x, y0 - y, color);
        }

        dx += 2;
        E += dx + 1;

        if (x != y) {
            mgLine(surface, x0 - y + 1, y0 + x, x0 + y, y0 + x, color);
            mgLine(surface, x0 - y + 1, y0 - x, x0 + y, y0 - x, color);
        }
    }
}

void mgCircle(MgSurface* surface, int x0, int y0, int r, MgPixel color) {
    int E = 1 - r;
    int dx = 0;
    int dy = -2 * r;
    int x = 0;
    int y = r;

    mgPlot(surface, x0, y0 + r, color);
    mgPlot(surface, x0, y0 - r, color);
    mgPlot(surface, x0 + r, y0, color);
    mgPlot(surface, x0 - r, y0, color);

    while (x < y - 1) {
        x++;

        if (E >= 0) {
            y--;
            dy += 2;
            E += dy;
        }

        dx += 2;
        E += dx + 1;

        mgPlot(surface, x0 + x, y0 + y, color);
        mgPlot(surface, x0 - x, y0 + y, color);
        mgPlot(surface, x0 + x, y0 - y, color);
        mgPlot(surface, x0 - x, y0 - y, color);

        if (x != y) {
            mgPlot(surface, x0 + y, y0 + x, color);
            mgPlot(surface, x0 - y, y0 + x, color);
            mgPlot(surface, x0 + y, y0 - x, color);
            mgPlot(surface, x0 - y, y0 - x, color);
        }
    }
}

MgPixel mgGet(MgSurface* surface, int x, int y) {
    MgPixel empty = { 0, 0, 0, 0 };
    if (x >= 0 && y >= 0 && x < surface->w && y < surface->h)
        return surface->pixels[y * surface->w + x];
    return empty;
}

void mgPlot(MgSurface* surface, int x, int y, MgPixel pixel) {
    int xa, i, a;

    int cx = surface->cx;
    int cy = surface->cy;
    int cw = surface->cw >= 0 ? surface->cw : surface->w;
    int ch = surface->ch >= 0 ? surface->ch : surface->h;

    if (x >= cx && y >= cy && x < cx + cw && y < cy + ch) {
        xa = EXPAND(pixel.a);
        a = xa * xa;
        i = y * surface->w + x;

        surface->pixels[i].r += (unsigned char)((pixel.r - surface->pixels[i].r) * a >> 16);
        surface->pixels[i].g += (unsigned char)((pixel.g - surface->pixels[i].g) * a >> 16);
        surface->pixels[i].b += (unsigned char)((pixel.b - surface->pixels[i].b) * a >> 16);
        surface->pixels[i].a += (surface->blitMode) * (unsigned char)((pixel.a - surface->pixels[i].a) * a >> 16);
    }
}

void mgClip(MgSurface* surface, int cx, int cy, int cw, int ch) {
    surface->cx = cx;
    surface->cy = cy;
    surface->cw = cw;
    surface->ch = ch;
}

void mgBlit(MgSurface* dst, MgSurface* src, int dx, int dy, int sx, int sy, int w, int h) {
    int cw = dst->cw >= 0 ? dst->cw : dst->w;
    int ch = dst->ch >= 0 ? dst->ch : dst->h;

    CLIP();

    MgPixel* ts = &src->pixels[sy * src->w + sx];
    MgPixel* td = &dst->pixels[dy * dst->w + dx];
    int st = src->w;
    int dt = dst->w;
    do {
        memcpy(td, ts, w * sizeof(MgPixel));
        ts += st;
        td += dt;
    } while (--h);
}

void mgBlitTint(MgSurface* dst, MgSurface* src, int dx, int dy, int sx, int sy, int w, int h, MgPixel tint) {
    int cw = dst->cw >= 0 ? dst->cw : dst->w;
    int ch = dst->ch >= 0 ? dst->ch : dst->h;

    CLIP();

    int xr = EXPAND(tint.r);
    int xg = EXPAND(tint.g);
    int xb = EXPAND(tint.b);
    int xa = EXPAND(tint.a);

    MgPixel* ts = &src->pixels[sy * src->w + sx];
    MgPixel* td = &dst->pixels[dy * dst->w + dx];
    int st = src->w;
    int dt = dst->w;
    do {
        for (int x = 0; x < w; x++) {
            unsigned r = (xr * ts[x].r) >> 8;
            unsigned g = (xg * ts[x].g) >> 8;
            unsigned b = (xb * ts[x].b) >> 8;
            unsigned a = xa * EXPAND(ts[x].a);
            td[x].r += (unsigned char)((r - td[x].r) * a >> 16);
            td[x].g += (unsigned char)((g - td[x].g) * a >> 16);
            td[x].b += (unsigned char)((b - td[x].b) * a >> 16);
            td[x].a += (dst->blitMode) * (unsigned char)((ts[x].a - td[x].a) * a >> 16);
        }
        ts += st;
        td += dt;
    } while (--h);
}

void mgBlitAlpha(MgSurface* dst, MgSurface* src, int dx, int dy, int sx, int sy, int w, int h, float alpha) {
    alpha = (alpha < 0) ? 0 : (alpha > 1 ? 1 : alpha);
    mgBlitTint(dst, src, dx, dy, sx, sy, w, h, mgRGBA(0xff, 0xff, 0xff, (unsigned char)(alpha * 255)));
}

void mgBlitMode(MgSurface* dst, int mode) {
    dst->blitMode = mode;
}

#undef CLIP0
#undef CLIP1
#undef CLIP


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy End  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^