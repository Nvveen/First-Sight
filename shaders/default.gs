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

layout(triangles, invocations=3) in;
layout(triangle_strip, max_vertices=3) out;

out vec3 pos[3];
out vec4 color[3];

void main(void) {
    gl_Position = gl_in[0].gl_Position;
    pos[0] = gl_Position.xyz;
    color[0] = vec4(1, 0, 0, 1);
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    pos[1] = gl_Position.xyz;
    color[1] = vec4(1, 0, 0, 1);
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    pos[2] = gl_Position.xyz;
    color[2] = vec4(1, 0, 0, 1);
    EmitVertex();
    EndPrimitive();
}
