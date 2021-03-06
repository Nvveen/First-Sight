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

in vec3 texPos;

uniform sampler3D gSampler;
uniform mat4 texTransform = mat4(1.0);

out vec4 fragColor;

void main(void) {
    vec3 t = (texTransform * vec4(floor(texPos), 1)).xyz;
    vec4 color = texelFetch(gSampler, ivec3(round(t)), 0);
    if ( color.a == 0.0 ) discard;
    fragColor = color;
}
