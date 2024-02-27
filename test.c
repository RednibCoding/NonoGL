#define MINIGFX_IMPLEMENTATION
#include "minigfx.h"

int main(int argc, char *argv[])
{
    MgSurface *screen = mgWindow(320, 240, "Hello", MG_AUTO | MG_2X);
    while (!mgClosed(screen))
    {
        mgClear(screen, mgRGB(0x80, 0x90, 0xa0));
        mgPrint(screen, mgfont, 120, 110, mgRGB(0xff, 0xff, 0xff), "Hello, world.");
        mgUpdate(screen);
    }
    mgFree(screen);
    return 0;
}