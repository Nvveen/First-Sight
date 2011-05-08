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
