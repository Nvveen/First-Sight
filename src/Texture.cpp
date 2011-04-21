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
Texture::Texture ( GLenum textureTarget, const sf::Image& png ) :
    textureTarget_(textureTarget), png_(png)
{
    width_ = 0;
    height_ = 0;
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
// Description:  Loads a targa image from the file.
//-----------------------------------------------------------------------------
    void
Texture::load ()
{
    glGenTextures(1, &texID);
    glBindTexture(textureTarget_, texID);
    glTexParameteri(textureTarget_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(textureTarget_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(textureTarget_, 0, GL_RGBA, width_, height_, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, png_.GetPixelsPtr());
    
    // Unbind everything
    glBindTexture(textureTarget_, 0);
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

