/*
Like most of stb libraries, this include needs to be included in one
compilation unit with MINIGFX_IMPLEMENTATION defined.
*/

#include "mg_header.h"

#ifdef MINIGFX_IMPLEMENTATION
#ifdef __cplusplus
extern "C" {
#endif

#include "mg_amalgamated.c"

#ifdef __cplusplus
}
#endif
#endif