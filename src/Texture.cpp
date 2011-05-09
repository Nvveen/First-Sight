// This file is part of First Sight.
// 
// First Sight is free software: you can redistribute it and/or modify it under 
// the terms of the GNU General Public License as published by the 
// Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.
// 
// First Sight is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
// more details.
// 
// You should have received a copy of the GNU General Public License along with 
// First Sight. If not, see <http://www.gnu.org/licenses/>.
// ============================================================================
// 
//       Filename:  Texture.cpp
// 
//    Description:  A class that makes a texture object and binds it.
// 
//        Version:  1.0
//        Created:  04/12/2011 04:54:46 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <iostream>
#include    <GL/glew.h>
#include    "Texture.h"

//-----------------------------------------------------------------------------
//       Class:  Texture
//      Method:  Texture
// Description:  constructor
//-----------------------------------------------------------------------------
Texture::Texture ()
{
}  // -----  end of method Texture::Texture  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Texture
//      Method:  Texture
// Description:  constructor
//-----------------------------------------------------------------------------
Texture::Texture ( GLenum textureTarget, 
                   const png::image<png::rgba_pixel>& png ) :
    textureTarget_(textureTarget)
{
    width_ = png.get_width();
    height_ = png.get_height();
    load();
    setFromPNG(png);
}  // -----  end of method Texture::Texture  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Texture
//      Method:  Texture
// Description:  constructor
//-----------------------------------------------------------------------------
Texture::Texture ( GLenum textureTarget, SDL_Surface *surface ) :
    textureTarget_(textureTarget)
{
    width_ = surface->w;
    height_ = surface->h;
    load();
    setFromSDL_Surface(surface);
}  // -----  end of method Texture::Texture  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Texture
//      Method:  Texture
// Description:  destructor
//-----------------------------------------------------------------------------
Texture::~Texture ()
{
}		// -----  end of method Texture::Texture  -----

//-----------------------------------------------------------------------------
//       Class:  Texture
//      Method:  load
// Description:  Loads a png image from the file.
//-----------------------------------------------------------------------------
    void
Texture::load ()
{
    glGenTextures(1, &texID);
    glBindTexture(textureTarget_, texID);
    glTexParameteri(textureTarget_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(textureTarget_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}		// -----  end of method Texture::load  -----

//-----------------------------------------------------------------------------
//       Class:  Texture
//      Method:  setFromPNG
// Description:  Sets the bytes in an array when passing a PNG object.
//-----------------------------------------------------------------------------
    void
Texture::setFromPNG ( const png::image<png::rgba_pixel>& png )
{
    std::vector<unsigned char> bytes;
    for ( int i = 0; i < height_; i += 1 ) {
        for ( int j = 0; j < width_; j += 1 ) {
            bytes.push_back(png[i][j].red);
            bytes.push_back(png[i][j].green);
            bytes.push_back(png[i][j].blue);
            bytes.push_back(png[i][j].alpha);
        }
    }
    glTexImage2D(textureTarget_, 0, GL_RGBA, width_, height_, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, &bytes[0]);
}		// -----  end of method Texture::setFromPNG  -----

//-----------------------------------------------------------------------------
//       Class:  Texture
//      Method:  setFromSDL_Surface
// Description:  Sets the texture data from an SDL Surface.
//-----------------------------------------------------------------------------
    void
Texture::setFromSDL_Surface ( SDL_Surface *surface )
{
    SDL_LockSurface(surface);
    glTexImage2D(textureTarget_, 0, GL_RGBA, width_, height_, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    SDL_UnlockSurface(surface);
}		// -----  end of method Texture::setFromSDL_Surface  -----

//-----------------------------------------------------------------------------
//       Class:  Texture
//      Method:  bind
// Description:  Binds the texture data into the shaders
//-----------------------------------------------------------------------------
    void
Texture::bind ()
{
    glBindTexture(textureTarget_, texID);
}		// -----  end of method Texture::bind  -----

//-----------------------------------------------------------------------------
//       Class:  Texture
//      Method:  unbind
// Description:  Unbinds the texture.
//-----------------------------------------------------------------------------
    void
Texture::unbind ()
{
    glBindTexture(textureTarget_, 0);
}		// -----  end of method Texture::unbind  -----

