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
in vec3 TexCoord;

uniform mat4 vMVP;
uniform mat4 texTransform = mat4(1.0);
uniform sampler3D gSampler;

out vec3 texPos;

void main(void) {
    texPos = TexCoord;
    gl_Position = vMVP * vec4(vVertex, 1.0);
}
