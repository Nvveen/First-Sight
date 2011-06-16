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

layout(points) in;
layout(triangle_strip, max_vertices=24) out;

in vec4 fColor[1];
out vec4 color;
in mat4 vTransform[1];

void main(void) {
    /* Front */
    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(0.0, 1.0, 0.0, 0.0));
    EmitVertex();
    
    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(1.0, 0.0, 0.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(1.0, 1.0, 0.0, 0.0));
    EmitVertex();
    EndPrimitive();

    /* Left */
    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(0.0, 0.0, 1.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(0.0, 1.0, 1.0, 0.0));
    EmitVertex();
    
    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(0.0, 1.0, 0.0, 0.0));
    EmitVertex();
    EndPrimitive();

    /* Bottom */
    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(0.0, 0.0, 1.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(1.0, 0.0, 1.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(1.0, 0.0, 0.0, 0.0));
    EmitVertex();
    EndPrimitive();
    
    /* Back */
    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(1.0, 0.0, 1.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(1.0, 1.0, 1.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(0.0, 0.0, 1.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(0.0, 1.0, 1.0, 0.0));
    EmitVertex();
    EndPrimitive();

    /* Right */
    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(1.0, 0.0, 0.0, 0.0));
    EmitVertex();
    
    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(1.0, 1.0, 0.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(1.0, 0.0, 1.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(1.0, 1.0, 1.0, 0.0));
    EmitVertex();
    EndPrimitive();
    
    /* Top */
    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(0.0, 1.0, 0.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(0.0, 1.0, 1.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(1.0, 1.0, 0.0, 0.0));
    EmitVertex();

    color = fColor[0];
    gl_Position = vTransform[0] * (gl_in[0].gl_Position + vec4(1.0, 1.0, 1.0, 0.0));
    EmitVertex();
    EndPrimitive();
}
