// ============================================================================
// 
//       Filename:  Text.h
// 
//    Description:  Class that generates an object for text displaying.
// 
//        Version:  1.0
//        Created:  05/07/2011 09:28:19 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  TEXT_H
#define  TEXT_H

#include    <string>
#include    <SDL/SDL_ttf.h>
#include    "Object.h"

// ============================================================================
//        Class:  Text
//  Description:  Class that is derived from an object and is displayed as 
//                a 2D surface for projection the text texture on it.
// ============================================================================
class Text : public Object
{
    public:

        // ====================  LIFECYCLE     ================================
        Text ( std::string textString );

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================

        // ====================  OPERATORS     ================================

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        std::string textString_;
        std::string fontFile_;

        TTF_Font *font_;
        SDL_Color color_;
        SDL_Surface *textSurface_;

        void init ();
        bool openFont ();
}; // -----  end of class Text  -----


#endif   // ----- #ifndef TEXT_H  -----
