// ============================================================================
// 
//       Filename:  Text.cpp
// 
//    Description:  Class for displaying text in the window.
// 
//        Version:  1.0
//        Created:  05/06/2011 11:29:55 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <iostream>
#include    <glm/gtc/matrix_transform.hpp>
#include    "Text.h" 

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  Text
// Description:  constructor
//-----------------------------------------------------------------------------
Text::Text ( std::string string, std::string fontName, unsigned char red, 
             unsigned char green, unsigned char blue, Shader shader ) :
    string_(string), font_(NULL), shader_(shader), x_(0.0f), y_(0.0f)
{
    cam_ = NULL;
    init();
    loadFont(fontName);
    color_ = { (Uint8)red, (Uint8)green, (Uint8)blue, 0 };
    render();
}  // -----  end of method Text::Text  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  ~Text
// Description:  destructor
//-----------------------------------------------------------------------------
Text::~Text ()
{
    TTF_CloseFont(font_);
    TTF_Quit();
}  // -----  end of method Text::~Text  (destructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  init
// Description:  Initiates the text-renderer.
//-----------------------------------------------------------------------------
    void
Text::init ()
{
}		// -----  end of method Text::init  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  loadFont
// Description:  Loads the font from the filename and sets it in memory.
//-----------------------------------------------------------------------------
    bool
Text::loadFont ( std::string font )
{
    font_ = TTF_OpenFont(font.c_str(), 30);
    if ( font_ == NULL ) {
        std::cerr << "Err loading font\n";
        return false;
    }
    else {
        return true;
    }
}		// -----  end of method Text::loadFont  -----

//-----------------------------------------------------------------------------
//       Class:  Text
//      Method:  render
// Description:  Renders the text as an OpenGL texture on a 2D surface.
//-----------------------------------------------------------------------------
    void
Text::render ()
{
}		// -----  end of method Text::render  -----

