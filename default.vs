#version 330

layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec2 TexCoord;

uniform mat4 vProjection;
uniform mat4 vCamera;
uniform mat4 vTranslate;
uniform mat4 vRotate;
uniform mat4 vScale;

out vec2 TexCoord0;

void main(void) {
    gl_Position = vProjection * vCamera * vTranslate * vRotate * vScale *
                  vec4(vVertex, 1.0);
    TexCoord0 = TexCoord;
}
