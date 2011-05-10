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
//       Filename:  Font.h
// 
//    Description:  Class that generates an SDL compatible font library.
// 
//        Version:  1.0
//        Created:  05/09/2011 09:56:53 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  FONT_H
#define  FONT_H

#include    <string>
#include    <map>
#include    <SDL/SDL_ttf.h>
#include    "Texture.h"

// ============================================================================
//        Class:  Font
//  Description:  Class that loads a font file and generates a bitmap from it,
//                after which it maps surfaces to each leter.
// ============================================================================
class Font
{
    public:

        // ====================  LIFECYCLE     ================================
        Font ();
        Font ( std::string fileName );
        ~Font ();

        // ====================  ACCESSORS     ================================
        int getWidth ();
        int getHeight ();
        SDL_Rect* getCharPos ( char& c );

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

        friend class Text;
    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        void init ();
        bool loadFont ();
        void generateBitmap ();
        void generateTexture ( SDL_Surface *map );

        std::string fileName_;
        TTF_Font *font_;
        int w_;
        int h_;
        int charW_;
        int charH_;

        SDL_Surface *bitmap_;
        std::map<char, SDL_Rect> charMap_;
        Texture *tex_;

}; // -----  end of class Font  -----

//-----------------------------------------------------------------------------
//       Class:  Font
//      Method:  generateTexture
// Description:  Creates a texture from the SDL_Surface.
//-----------------------------------------------------------------------------
    inline void
Font::generateTexture ( SDL_Surface *map )
{
    tex_ = new Texture(GL_TEXTURE_2D, map);
}		// -----  end of method Font::generateTexture  -----

//-----------------------------------------------------------------------------
//       Class:  Font
//      Method:  getCharPos
// Description:  Returns the position and size of the character on the bitmap.
//-----------------------------------------------------------------------------
    inline SDL_Rect*
Font::getCharPos ( char& c )
{
    return &charMap_[c];
}		// -----  end of method Font::getCharPos  -----

//-----------------------------------------------------------------------------
//       Class:  Font
//      Method:  getWidth
// Description:  Returns the width of the bitmap.
//-----------------------------------------------------------------------------
    inline int
Font::getWidth ()
{
    return w_;
}		// -----  end of method Font::getWidth  -----

//-----------------------------------------------------------------------------
//       Class:  Font
//      Method:  getHeight
// Description:  Returns the height of the bitmap.
//-----------------------------------------------------------------------------
    inline int
Font::getHeight ()
{
    return h_;
}		// -----  end of method Font::getHeight  -----

#endif   // ----- #ifndef FONT_H  -----
