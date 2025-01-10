#ifndef NN_KEY_DEFINES_H
#define NN_KEY_DEFINES_H

#include "GL/freeglut.h"

// Special Keys (we have to offset them since glut special key ranges overlap normal key ranges e.g. key arrow-left and key 'd' are both decimal 100)
#define nnVK_F1 (GLUT_KEY_F1 + 256)
#define nnVK_F2 (GLUT_KEY_F2 + 256)
#define nnVK_F3 (GLUT_KEY_F3 + 256)
#define nnVK_F4 (GLUT_KEY_F4 + 256)
#define nnVK_F5 (GLUT_KEY_F5 + 256)
#define nnVK_F6 (GLUT_KEY_F6 + 256)
#define nnVK_F7 (GLUT_KEY_F7 + 256)
#define nnVK_F8 (GLUT_KEY_F8 + 256)
#define nnVK_F9 (GLUT_KEY_F9 + 256)
#define nnVK_F10 (GLUT_KEY_F10 + 256)
#define nnVK_F11 (GLUT_KEY_F11 + 256)
#define nnVK_F12 (GLUT_KEY_F12 + 256)

#define nnVK_LEFT (GLUT_KEY_LEFT + 256)
#define nnVK_UP (GLUT_KEY_UP + 256)
#define nnVK_RIGHT (GLUT_KEY_RIGHT + 256)
#define nnVK_DOWN (GLUT_KEY_DOWN + 256)

#define nnVK_PAGE_UP (GLUT_KEY_PAGE_UP + 256)
#define nnVK_PAGE_DOWN (GLUT_KEY_PAGE_DOWN + 256)
#define nnVK_HOME (GLUT_KEY_HOME + 256)
#define nnVK_END (GLUT_KEY_END + 256)
#define nnVK_INSERT (GLUT_KEY_INSERT + 256)

#define nnVK_SHIFT_L (GLUT_KEY_SHIFT_L + 256)
#define nnVK_SHIFT_R (GLUT_KEY_SHIFT_R + 256)
#define nnVK_CTRL_L (GLUT_KEY_CTRL_L + 256)
#define nnVK_CTRL_R (GLUT_KEY_CTRL_R + 256)
#define nnVK_ALT_L (GLUT_KEY_ALT_L + 256)
#define nnVK_ALT_R (GLUT_KEY_ALT_R + 256)

#define nnVK_NUM_LOCK (GLUT_KEY_NUM_LOCK + 256)
#define nnVK_BEGIN (GLUT_KEY_BEGIN + 256)
// #define nnVK_DELETE (GLUT_KEY_DELETE + 256)
#define nnVK_DELETE (0x7f) // Dont know why freeGLUT does not recognize this key as special key but as normal key. So freeGLUT invokes 'glutKeyboardFunc' callback instead of 'glutSpecialFunc'...

// Alphanumeric Keys
#define nnVK_A 0x61
#define nnVK_B 0x62
#define nnVK_C 0x63
#define nnVK_D 0x64
#define nnVK_E 0x65
#define nnVK_F 0x66
#define nnVK_G 0x67
#define nnVK_H 0x68
#define nnVK_I 0x69
#define nnVK_J 0x6A
#define nnVK_K 0x6B
#define nnVK_L 0x6C
#define nnVK_M 0x6D
#define nnVK_N 0x6E
#define nnVK_O 0x6F
#define nnVK_P 0x70
#define nnVK_Q 0x71
#define nnVK_R 0x72
#define nnVK_S 0x73
#define nnVK_T 0x74
#define nnVK_U 0x75
#define nnVK_V 0x76
#define nnVK_W 0x77
#define nnVK_X 0x78
#define nnVK_Y 0x79
#define nnVK_Z 0x7A

#define nnVK_0 0x30
#define nnVK_1 0x31
#define nnVK_2 0x32
#define nnVK_3 0x33
#define nnVK_4 0x34
#define nnVK_5 0x35
#define nnVK_6 0x36
#define nnVK_7 0x37
#define nnVK_8 0x38
#define nnVK_9 0x39

#define nnVK_ESCAPE 0x1B
#define nnVK_BACKSPACE 0x08
#define nnVK_TAB 0x09
#define nnVK_ENTER 0x0D
#define nnVK_SPACE 0x20

#define nnVK_SEMICOLON 0xBA    // ';'
#define nnVK_EQUAL 0xBB        // '='
#define nnVK_COMMA 0xBC        // ','
#define nnVK_MINUS 0xBD        // '-'
#define nnVK_PERIOD 0xBE       // '.'
#define nnVK_SLASH 0xBF        // '/'
#define nnVK_BACKTICK 0xC0     // '`'
#define nnVK_LEFTBRACKET 0xDB  // '['
#define nnVK_BACKSLASH 0xDC    // '\'
#define nnVK_RIGHTBRACKET 0xDD // ']'
#define nnVK_QUOTE 0xDE        // '\''

// Mouse
#define nnVK_MOUSE_LEFT GLUT_LEFT_BUTTON
#define nnVK_MOUSE_MIDDLE GLUT_MIDDLE_BUTTON
#define nnVK_MOUSE_RIGHT GLUT_RIGHT_BUTTON

#endif // NN_KEY_DEFINES_H
