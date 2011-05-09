// ============================================================================
// 
//       Filename:  Text.cpp
// 
//    Description:  Class that generates an object for text displaying.
// 
//        Version:  1.0
//        Created:  05/07/2011 09:27:49 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <png++/png.hpp>
#include    <glm/gtc/matrix_transform.hpp>
#include    "Text.h"

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  Text
// Description:  constructor
//-----------------------------------------------------------------------------
Text::Text ( std::string textString ) :
    textString_(textString), fontFile_("arial.ttf")
{
    color_ = {255, 255, 255, 0};
    font_ = NULL;
    textSurface_ = NULL;
    TTF_Init();
    init();
    TTF_Quit();
}  // -----  end of method Text::Text  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  init
// Description:  Initializes the Text object.
//-----------------------------------------------------------------------------
    void
Text::init ()
{
    if ( !openFont() ) {
        std::cerr << "Error loading font: " << TTF_GetError() << "\n";
        exit(1);
    }
    textSurface_ = TTF_RenderText_Solid(font_, textString_.c_str(), color_);
    if ( textSurface_ == NULL ) {
        std::cerr << "Error rendering text: " << TTF_GetError() << "\n";
        exit(1);
    }
    SDL_Surface *inmed = SDL_CreateRGBSurface(SDL_SWSURFACE, textSurface_->w,
                                              textSurface_->h, 32, 0, 0, 0, 0);
    SDL_BlitSurface(textSurface_, NULL, inmed, NULL);
    this->modelData_ = createVertexData();
    texture_ = new Texture(GL_TEXTURE_2D, inmed);
    projection_ = glm::ortho(0.0f, 1024.0f, 768.0f, 0.0f);

    this->Object::init();

    TTF_CloseFont(font_);
}		// -----  end of method Text::init  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  openFont
// Description:  Tries to open the font file.
//-----------------------------------------------------------------------------
    bool
Text::openFont ()
{
    font_ = TTF_OpenFont(fontFile_.c_str(), 18);
    if ( font_ == NULL )
        return false;
    else
        return true;
}		// -----  end of method Text::openFont  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  createVertexData
// Description:  Creates an array of vertex data for the texture.
//-----------------------------------------------------------------------------
    std::vector<GLfloat>
Text::createVertexData ()
{
    GLfloat w = textSurface_->w;
    GLfloat h = textSurface_->h;
    GLfloat x = 0.0f;
    GLfloat y = 0.0f;
    GLfloat data[48] = { x,   y,   1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                         x,   y+h, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                         x+w, y+h, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                         x,   y,   1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                         x+w, y+h, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                         x+w, y,   1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f};
    std::vector<GLfloat> vectorData;
    vectorData.assign(data, data+48);
    return vectorData;
}		// -----  end of method Text::createVertexData  -----

