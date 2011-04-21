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
#include    <sstream>
#include    <cstdlib>
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
Texture::Texture ( GLenum textureTarget, const std::string& png )
{
    textureTarget_ = textureTarget;
    bits_ = NULL;
    width_ = 0;
    height_ = 0;
    textureSet = false;
    this->init(png);
}  // -----  end of method Texture::Texture  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Texture
//      Method:  Texture
// Description:  destructor
//-----------------------------------------------------------------------------
Texture::~Texture ()
{
}		// -----  end of method Texture::Texture  -----

    void
Texture::init ( const std::string& png )
{
    std::istringstream iss(png);
    std::string line;
    iss >> width_;
    iss >> height_;
    iss >> scanWidth_;
    bits_ = new BYTE[height_*scanWidth_];
    for ( long i = 0; i < height_*scanWidth_; i += 1 ) {
        short val;
        iss >> val;
        bits_[i] = (BYTE)val;
    }
}		// -----  end of method Texture::init  -----

//-----------------------------------------------------------------------------
//       Class:  Texture
//      Method:  load
// Description:  Loads a targa image from the file.
//-----------------------------------------------------------------------------
    void
Texture::load ()
{
    FreeImage_Initialise();
    FIBITMAP *dib = FreeImage_ConvertFromRawBits(bits_, width_, height_,
                                                scanWidth_, 32,
                                                FI_RGBA_RED_MASK,
                                                FI_RGBA_GREEN_MASK,
                                                FI_RGBA_BLUE_MASK,
                                                FALSE);
    BYTE* newBits = FreeImage_GetBits(dib);
    FreeImage_Save(FIF_PNG, dib, "test.png");
    glGenTextures(1, &texID);
    glBindTexture(textureTarget_, texID);
    glTexParameteri(textureTarget_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(textureTarget_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(textureTarget_, 0, GL_RGBA, width_, height_, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, newBits);
    
    // Unbind everything
    glBindTexture(textureTarget_, 0);
    FreeImage_DeInitialise();
}		// -----  end of method Texture::load  -----

//-----------------------------------------------------------------------------
//       Class:  Texture
//      Method:  bind
// Description:  Binds the texture data into the shaders
//-----------------------------------------------------------------------------
    void
Texture::bind ( const GLuint& shader )
{
    glBindTexture(textureTarget_, texID);
    GLuint gSampler = glGetUniformLocation(shader, "gSampler");
    glUniform1i(gSampler, 0);
}		// -----  end of method Texture::bind  -----

