#ifndef __MINIGFX_UPSCALE_GL_VS_H__
#define __MINIGFX_UPSCALE_GL_VS_H__


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy Start  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


// clang-format off
const char mg_upscale_gl_vs[] = {
    "#version 330 core\n"
    "layout (location = 0) in vec2 pos_in;"
    "layout (location = 1) in vec2 uv_in;"
    "out vec2 uv;"
    "uniform mat4 model;"
    "uniform mat4 projection;"
    "void main()"
    "{"
    "   uv = uv_in;"
    "   gl_Position = projection * model * vec4(pos_in, 0.0, 1.0);"
    "}"
};
// clang-format on

const int mg_upscale_gl_vs_size = (int)sizeof(mg_upscale_gl_vs) - 1;


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy End  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


#endif
