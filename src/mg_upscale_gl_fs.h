#ifndef __MINIGFX_UPSCALE_GL_FS_H__
#define __MINIGFX_UPSCALE_GL_FS_H__


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy Start  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


// clang-format off
const char mg_upscale_gl_fs[] = {
    "#version 330 core\n"
    "in vec2 uv;"
    "out vec4 color;"
    "uniform sampler2D image;"
    "uniform vec4 parameters;"
    "void fxShader(out vec4 color, in vec2 coord);"
    "void main()"
    "{"
    "   fxShader(color, uv);"
    "}\n"
};
// clang-format on

const int mg_upscale_gl_fs_size = (int)sizeof(mg_upscale_gl_fs) - 1;

// clang-format off
const char mg_default_fx_gl_fs[] = {
    "void fxShader(out vec4 color, in vec2 uv) {"
    "   vec2 tex_size = vec2(textureSize(image, 0));"
    "   vec2 uv_blur = mix(floor(uv * tex_size) + 0.5, uv * tex_size, parameters.xy) / tex_size;"
    "   vec4 c = texture(image, uv_blur);"
    "   c.rgb *= mix(0.5, 1.0 - fract(uv.y * tex_size.y), parameters.z) * 2.0; //scanline\n"
    "   c = mix(vec4(0.5), c, parameters.w); //contrast\n"
    "   color = c;"
    "}"
};
// clang-format on

const int mg_default_fx_gl_fs_size = (int)sizeof(mg_default_fx_gl_fs) - 1;


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   Copy End  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


#endif
