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
//       Filename:  Font.cpp
// 
//    Description:  Class that generates an SDL compatible font library.
// 
//        Version:  1.0
//        Created:  05/09/2011 09:56:28 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <iostream>
#include    "Font.h"

//-----------------------------------------------------------------------------
//       Class:  Font
//      Method:  Font
// Description:  constructor
//-----------------------------------------------------------------------------
Font::Font ()
{
    w_ = 0;
    h_ = 0;
    charW_ = 0;
    charH_ = 0;
}  // -----  end of method Font::Font  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Font
//      Method:  Font
// Description:  constructor
//-----------------------------------------------------------------------------
Font::Font ( std::string fileName ) : fileName_(fileName)
{
    w_ = 0;
    h_ = 0;
    charW_ = 0;
    charH_ = 0;
    init ();
}  // -----  end of method Font::Font  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Font
//      Method:  ~Font
// Description:  destructor
//-----------------------------------------------------------------------------
Font::~Font ()
{
    TTF_CloseFont(font_);
    SDL_FreeSurface(bitmap_);
    if ( tex_ != NULL ) delete tex_;
}  // -----  end of method Font::~Font  (destructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Font
//      Method:  init
// Description:  Initializes the Font object.
//-----------------------------------------------------------------------------
    void
Font::init ()
{
    TTF_Init();
    if ( !loadFont() ) {
        std::cerr << "Error loading font: " << TTF_GetError() << "\n";
        exit(1);
    }
    // First we need to define that dimensions of the bitmap. For that, we loop
    // through every character in ASCII.
    for ( int i = 0; i < 16; i += 1 ) {
        // For every line we need to see how big it is, and compare that to
        // previous values. The largest width is the width of the bitmap.
        int temp = 0;
        for ( int j = 0; j < 16; j += 1 ) {
            // Get the character from the loop and convert it to a string.
            std::string string(1, (char)(i*16+j));
            // Define variables for the character width and height.
            int charW(0), charH(0);
            // Get the size of the character.
            TTF_SizeText(font_, string.c_str(), &charW, &charH);
            // If the character size is bigger than any previous size, update
            // it. This may not even be needed.
            if ( charH > h_ ) h_ = charH;
            temp += charW;
        }
        // If the current line width is bigger than the previous values, update.
        if ( temp > w_ ) w_ = temp;
    }
    // The height of the map is the number of lines (16) multiplied by the
    // highest character size.
    h_ *= 16;
    // Create an empty surface with the dimensions we just generated.
    Uint32 rmask = 0x00FF0000;
    Uint32 gmask = 0x0000FF00;
    Uint32 bmask = 0x000000FF;
    Uint32 amask = 0xFF000000;
    bitmap_ = SDL_CreateRGBSurface(SDL_HWSURFACE, w_, h_, 32, rmask, gmask, 
                                   bmask, amask);
    // Now we stuff the surface with characters.
    generateBitmap();
    generateTexture(bitmap_);
    TTF_Quit();
}		// -----  end of method Font::init  -----

//-----------------------------------------------------------------------------
//       Class:  Font
//      Method:  loadFont
// Description:  Loads a font file.
//-----------------------------------------------------------------------------
    bool
Font::loadFont ()
{
    font_ = TTF_OpenFont(fileName_.c_str(), 12);
    if ( font_ == NULL )
        return false;
    else
        return true;
}		// -----  end of method Font::loadFont  -----

//-----------------------------------------------------------------------------
//       Class:  Font
//      Method:  generateBitmap
// Description:  Loops through every character in ASCII and adds them to the
//               surface.
//-----------------------------------------------------------------------------
    void
Font::generateBitmap ()
{
    SDL_Color color = { 255, 255, 255, 0 };
    // We have to loop through every ASCII letter again.
    for ( int i = 0; i < 16; i += 1 ) {
        for ( int j = 0; j < 16; j += 1 ) {
            // Get the current character as a string.
            std::string string(1, (char)(i*16+j));
            // We need to get the width and height of the current character.
            int charW(0), charH(0);
            // Set those variables.
            TTF_SizeText(font_, string.c_str(), &charW, &charH);
            if ( charW == 0 ) string = " ";
            // We create a temporary surface by rendering the character.
            SDL_Surface *charSurface = TTF_RenderText_Blended(font_,
                                                       string.c_str(),
                                                       color);
            if ( charSurface == NULL ) {
                std::cerr << "Error: " << TTF_GetError() << "\n";
                exit(1);
            }
            // We now have a surface containing the character. All we need to
            // do now is get the x,y positions and stuff that and the dimensions
            // of the character in a rect to be stored in a character map.
            SDL_Rect *rect = new SDL_Rect;
            rect->w = charW;
            rect->h = charH;
            // The y position is always the line number multiplied by a
            // character's height, since the height is always constant.
            rect->y = i*(h_/16);
            // The x position is slightly more complicated. Since we're dealing
            // with variable length widths, we need to get the distance from the
            // beginning of the line to the actual character. To do that, we get
            // the widths of all preceding characters on the same line, and add
            // them together to form the variable distance.
            int distance = 0;
            // Begin at the line start until the current character.
            for ( int k = (i*16); k < (i*16+j); k += 1 ) {
                // For every character preceding the current one, get its value
                // as a string.
                std::string dString(1, (char)k);
                // Now we define a variable to hold the current char's width.
                int dCharW = 0;
                // We set it.
                TTF_SizeText(font_, dString.c_str(), &dCharW, NULL);
                // And add that width to the distance.
                distance += dCharW;
            }
            // End result is the distance from the line beginning to the current
            // character; in other words, its x position.
            rect->x = distance;
            // We set the rectangle in memory.
            charMap_[(char)(i*16+j)] = *rect;
            // And blit that character to the bitmap with its rectangle.
            SDL_SetAlpha(charSurface, 0, 0);
            SDL_BlitSurface(charSurface, NULL, bitmap_, rect);
            // We don't need the character surface anymore, so free it.
            SDL_FreeSurface(charSurface);
        }
    }
}		// -----  end of method Font::generateBitmap  -----

