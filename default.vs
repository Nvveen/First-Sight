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
#version 120

/*layout(location = 0) in vec3 vVertex;
layout(location = 1) in vec2 TexCoord;*/
in vec3 vVertex;
in vec2 TexCoord;

uniform mat4 vProjection;
uniform mat4 vCamera;
uniform mat4 vTranslate;
uniform mat4 vRotate;
uniform mat4 vScale;

varying vec2 TexCoord0;

void main(void) {
    gl_Position = vProjection * vCamera * vTranslate * vRotate * vScale * 
                  vec4(vVertex, 1.0);
    TexCoord0 = TexCoord;
}
