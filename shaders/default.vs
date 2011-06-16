/* This file is part of First Sight.
 
First Sight is free software: you can redistribute it and/or modify it under 
the terms of the GNU General Public License as published by the 
Free Software Foundation, either version 3 of the License, or 
(at your option) any later version.

First Sight is distributed in the hope that it will be useful, but WITHOUT 
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
more details.
 
You should have received a copy of the GNU General Public License along with 
First Sight. If not, see <http://www.gnu.org/licenses/>. */
#version 150

in vec3 vVertex;

layout (std140) uniform Projection {
    mat4 vProjection, vCamera;
};

layout (std140) uniform Model {
    mat4 vTranslate, vRotate, vScale;
};

uniform sampler2D gSampler;

out vec4 pos;
out vec4 fColor;
out mat4 vTransform;

ivec3 size;

bool hasNoNeighbors ( ivec3 coord ) {
    return true;
}

void main(void) {
    int size = textureSize(gSampler, 0).x;
    ivec2 mapPos = ivec2((gl_InstanceID*2 % size),
                         (gl_InstanceID*2 / size) % size);
    fColor = texelFetch(gSampler, mapPos, 0);
    vec4 offsetPos = texelFetch(gSampler, ivec2(mapPos.x+1, mapPos.y), 0);
    offsetPos *= 255.0f;
    vTransform = vProjection * vCamera * vTranslate * vRotate * vScale;
    pos = vec4(vVertex, 1.0);
    pos += vec4(offsetPos.xyz, 0.0);
    gl_Position = vTransform * pos;
}
