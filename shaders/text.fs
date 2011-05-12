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

varying vec2 Tex;
uniform sampler2D gSampler;
uniform vec4 varyingColor;
uniform vec4 backgroundColor;

void main(void) {
    gl_FragColor = texture2D(gSampler, Tex);
    if ( gl_FragColor.a != 0.0 ) {
        gl_FragColor = varyingColor;
    }
    else {
        gl_FragColor = backgroundColor;
    }
}
